#include "ClassBinding.h"
#include "Generator.h"

static void generateInstanceGetter(ostream& o, const string& classname, const string& uniquename)
{
    o << "static " << classname << "* getInstance(lua_State* state)\n";
    o << "{\n";
    o << "    void* userdata = luaL_checkudata(state, 1, \"" << uniquename << "\");\n";
    o << "    luaL_argcheck(state, userdata != NULL, 1, \"\'" << uniquename << "\' expected.\");\n";
    o << "    return (" << classname << "*)((" << LUA_OBJECT << "*)userdata)->instance;\n";
    o << "}\n\n";
}

ClassBinding::ClassBinding(string classname, string refId) : classname(classname),
    refId(refId), inaccessibleConstructor(false), inaccessibleDestructor(false)
{
    // Get the class's name and namespace.
    this->classname = Generator::getInstance()->getClassNameAndNamespace(classname, &ns);

    // Calculate the unique name for the class.
    uniquename = Generator::getUniqueName(this->classname);
}

void ClassBinding::write(string dir, const set<string>& includes, string* bindingNS)
{
    // Calculate the constructor string.
    size_t index = uniquename.rfind(SCOPE_REPLACEMENT);
    string constructorString = "";
    string* constructorUniqueName = NULL;
    if (index != uniquename.npos && index != uniquename.length())
        constructorString += uniquename.substr(index + SCOPE_REPLACEMENT_SIZE);
    else
        constructorString += uniquename;

    // Generate a constructor function if there isn't one
    // and the class doesn't have an inaccessible constructor.
    FunctionBinding b(classname, uniquename);
    b.name = constructorString;
    b.returnParam = FunctionBinding::Param(FunctionBinding::Param::TYPE_CONSTRUCTOR, FunctionBinding::Param::KIND_POINTER, refId);
    b.type = FunctionBinding::MEMBER_FUNCTION;
    b.own = true;

    map<string, vector<FunctionBinding> >::iterator iter = bindings.find(b.getFunctionName());
    if (iter == bindings.end())
    {
        if (!inaccessibleConstructor)
        {
            bindings[b.getFunctionName()].push_back(b);
            constructorUniqueName = new string(b.getFunctionName());
        }
    }
    else
    {
        constructorUniqueName = new string(iter->second[0].getFunctionName());
    }

    // Calculate the destructor string.
    index = uniquename.rfind(SCOPE_REPLACEMENT);
    string destructorString = "~";
    string* destructorUniqueName = NULL;
    if (index != uniquename.npos && index != uniquename.length())
        destructorString += uniquename.substr(index + SCOPE_REPLACEMENT_SIZE);
    else
        destructorString += uniquename;

    // Generate a destructor function if there isn't one
    // and the class doesn't have an inaccessible destructor
    // or the class is derived from Ref.
    b = FunctionBinding(classname, uniquename);
    b.name = destructorString;
    b.returnParam = FunctionBinding::Param(FunctionBinding::Param::TYPE_DESTRUCTOR);
    b.type = FunctionBinding::MEMBER_FUNCTION;

    iter = bindings.find(b.getFunctionName());
    if (iter == bindings.end())
    {
        if (!inaccessibleDestructor || Generator::getInstance()->isRef(classname))
        {
            bindings[b.getFunctionName()].push_back(b);
            destructorUniqueName = new string(b.getFunctionName());
        }
    }
    else
    {
        destructorUniqueName = new string(iter->second[0].getFunctionName());
    }

    // Write out the header.
    {
        string path = dir + string("lua_") + uniquename + string(".h");
        ostringstream o;
        if (!o)
        {
            GP_ERROR("Failed to open file '%s' for generating Lua bindings.", path.c_str());
            return;
        }

        string includeGuard = string("lua_") + uniquename + string("_H_");
        transform(includeGuard.begin(), includeGuard.end(), includeGuard.begin(), ::toupper);
        o << "#ifndef " << includeGuard << "\n";
        o << "#define " << includeGuard << "\n\n";

        if (bindingNS)
        {
            o << "namespace " << *bindingNS << "\n";
            o << "{\n\n";
        }

        // Write out the signature of the function used to register the class with Lua.
        o << "void luaRegister_" << uniquename << "();\n\n";

        if (bindingNS)
            o << "}\n\n";
        o << "#endif\n";

        writeFile(path, o.str());
    }

    // Write out the implementation.
    {
        string path = dir + string("lua_") + uniquename + string(".cpp");
        ostringstream o;
        if (!o)
        {
            GP_ERROR("Failed to open file '%s' for generating Lua bindings.", path.c_str());
            return;
        }

        o << "#include \"Base.h\"\n";
        o << "#include \"ScriptController.h\"\n";
        o << "#include \"lua_" << uniquename << ".h\"\n";

        // Ensure we include the original class header, even 
        // if the list of includes doesn't have it.
        if (includes.find(include) == includes.end())
            o << "#include \"" << include << "\"\n";

        for (set<string>::iterator includeIter = includes.begin(); includeIter != includes.end(); includeIter++)
        {
            o << "#include \"" << *includeIter << "\"\n";
        }

        // Add relative includes (if they aren't already included)
        const set<string>& relatives = Generator::getInstance()->getClassRelatives(classname);
        for (set<string>::iterator itr = relatives.begin(); itr != relatives.end(); ++itr)
        {
            string relativeInclude = Generator::getInstance()->getInclude(*itr);
            if (!relativeInclude.empty() && includes.find(relativeInclude) != includes.end())
            {
                o << "#include \"" << relativeInclude << "\"\n";
            }
        }

        o << "\n";

        // If the original class is part of a namespace and we aren't generating into that namespace,
        // include its member with a 'using' statement.
        if (ns.length() > 0 && (!bindingNS || (*bindingNS != ns)))
        {
            o << "using " << ns << ";\n\n";
        }

        if (bindingNS)
        {
            o << "namespace " << *bindingNS << "\n";
            o << "{\n\n";
        }

        bool hasMembers = false;

        // If this class has any relatives, forward delcare conversion function
        if (relatives.size() > 0)
        {
            // Need to declare the extern conversion registration function
            o << "extern void " << LUA_GLOBAL_REGISTER_CONVERSION_FUNCTION << "(const char* className, void*(*func)(void*, const char*));\n\n";

            hasMembers = true;
        }

        // Write out the function used to get the instance for
        // calling member functions and variables.
        generateInstanceGetter(o, classname, uniquename);

        // Write out the binding functions.
        iter = bindings.begin();
        for (; iter != bindings.end(); iter++)
        {
            FunctionBinding::write(o, iter->second);
        }

        if (relatives.size() > 0)
        {
            // Write out our special conversion function used by the framework
            o << "// Provides support for conversion to all known relative types of " << classname << "\n";
            o << "static void* __convertTo(void* ptr, const char* typeName)\n";
            o << "{\n";
            o << "    " << classname << "* ptrObject = reinterpret_cast<" << classname << "*>(ptr);\n\n";
            int i = 0;
            for (set<string>::const_iterator itr = relatives.begin(); itr != relatives.end(); ++itr, ++i)
            {
                o << "    " << (i == 0 ? "if" : "else if") << " (strcmp(typeName, \"" << *itr << "\") == 0)\n";
                o << "    {\n";
                o << "        return reinterpret_cast<void*>(static_cast<" << *itr << "*>(ptrObject));\n";
                o << "    }\n";
            }
            o << "\n";
            o << "    // No conversion available for 'typeName'\n";
            o << "    return NULL;\n";
            o << "}\n\n";

            // Write out public "to(typeName)" method for this type, to support converting
            // from this type to other relatives
            o << "static int lua_" << uniquename << "_to(lua_State* state)\n";
            o << "{\n";
            o << "    // There should be only a single parameter (this instance)\n";
            o << "    if (lua_gettop(state) != 2 || lua_type(state, 1) != LUA_TUSERDATA || lua_type(state, 2) != LUA_TSTRING)\n";
            o << "    {\n";
            o << "        lua_pushstring(state, \"lua_" << uniquename << "_to - Invalid number of parameters (expected 2).\");\n";
            o << "        lua_error(state);\n";
            o << "        return 0;\n";
            o << "    }\n\n";
            o << "    " << classname << "* instance = getInstance(state);\n";
            o << "    const char* typeName = gameplay::ScriptUtil::getString(2, false);\n";
            o << "    void* result = __convertTo((void*)instance, typeName);\n\n";
            o << "    if (result)\n";
            o << "    {\n";
            o << "        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));\n";
            o << "        object->instance = (void*)result;\n";
            o << "        object->owns = false;\n";
            o << "        luaL_getmetatable(state, typeName);\n";
            o << "        lua_setmetatable(state, -2);\n";
            o << "    }\n";
            o << "    else\n";
            o << "    {\n";
            o << "        lua_pushnil(state);\n";
            o << "    }\n\n";
            o << "    return 1;\n";
            o << "}\n\n";
        }

        // Write out the function used to register the class with Lua.
        o << "void luaRegister_" << uniquename << "()\n";
        o << "{\n";

        // Get the member functions ready to bind.
        iter = bindings.begin();
        for (; iter != bindings.end(); iter++)
        {
            for (unsigned int i = 0, count = iter->second.size(); i < count; i++)
            {
                if (iter->second[i].type == FunctionBinding::MEMBER_FUNCTION ||
                    iter->second[i].type == FunctionBinding::MEMBER_CONSTANT ||
                    iter->second[i].type == FunctionBinding::MEMBER_VARIABLE)
                {
                    hasMembers = true;
                    break;
                }
            }

            if (hasMembers)
                break;
        }
        if (hasMembers)
        {
            o << "    const luaL_Reg lua_members[] = \n";
            o << "    {\n";
            iter = bindings.begin();
            for (; iter != bindings.end(); iter++)
            {
                for (unsigned int i = 0, count = iter->second.size(); i < count; i++)
                {
                    if ((iter->second[i].type == FunctionBinding::MEMBER_FUNCTION ||
                        iter->second[i].type == FunctionBinding::MEMBER_CONSTANT ||
                        iter->second[i].type == FunctionBinding::MEMBER_VARIABLE) &&
                        iter->second[i].returnParam.type != FunctionBinding::Param::TYPE_CONSTRUCTOR &&
                        iter->second[i].returnParam.type != FunctionBinding::Param::TYPE_DESTRUCTOR)
                    {
                        o << "        {\"" << iter->second[i].name << "\", " << iter->second[i].getFunctionName() << "},\n";
                        break;
                    }
                }
            }
            if (relatives.size() > 0)
            {
                // Register 'to' conversion function
                o << "        {\"to\", lua_" << uniquename << "_to},\n";
            }
            o << "        {NULL, NULL}\n";
            o << "    };\n";
        }
        else
        {
            o << "    const luaL_Reg* lua_members = NULL;\n";
        }

        // Get the static functions ready to bind.
        iter = bindings.begin();
        bool hasStatics = false;
        for (; iter != bindings.end(); iter++)
        {
            for (unsigned int i = 0, count = iter->second.size(); i < count; i++)
            {
                if (iter->second[i].type == FunctionBinding::STATIC_FUNCTION ||
                    iter->second[i].type == FunctionBinding::STATIC_CONSTANT ||
                    iter->second[i].type == FunctionBinding::STATIC_VARIABLE)
                {
                    hasStatics = true;
                    break;
                }
            }

            if (hasStatics)
                break;
        }
        if (hasStatics)
        {
            o << "    const luaL_Reg lua_statics[] = \n";
            o << "    {\n";
            iter = bindings.begin();
            for (; iter != bindings.end(); iter++)
            {
                for (unsigned int i = 0, count = iter->second.size(); i < count; i++)
                {
                    if (iter->second[i].type == FunctionBinding::STATIC_FUNCTION ||
                        iter->second[i].type == FunctionBinding::STATIC_CONSTANT ||
                        iter->second[i].type == FunctionBinding::STATIC_VARIABLE)
                    {
                        o << "        {\"" << iter->second[i].name << "\", " << iter->second[i].getFunctionName() << "},\n";
                        break;
                    }
                }
            }
            o << "        {NULL, NULL}\n";
            o << "    };\n";
        }
        else
        {
            o << "    const luaL_Reg* lua_statics = NULL;\n";
        }

        // Output the scope path for the class (used for inner classes).
        vector<string> scopePath = Generator::getScopePath(classname, ns);
        o << "    std::vector<std::string> scopePath;\n";
        for (vector<string>::iterator scopeIter = scopePath.begin(); scopeIter != scopePath.end(); scopeIter++)
        {
            o << "    scopePath.push_back(\"" << *scopeIter << "\");\n";
        }

        // Register the class (its member and static functions and constructor and destructor).
        o << "\n    gameplay::ScriptUtil::registerClass(\"" << uniquename << "\", lua_members, ";
        o << ((constructorUniqueName) ? *constructorUniqueName : "NULL") << ", ";
        o << ((destructorUniqueName) ? *destructorUniqueName : "NULL") << ", ";
        o << "lua_statics, scopePath);\n\n";

        // Register conversion function for this type
        if (relatives.size() > 0)
        {
            o << "    " << LUA_GLOBAL_REGISTER_CONVERSION_FUNCTION << "(\"" << uniquename << "\", __convertTo);\n";
        }

        o << "}\n\n";

        if (bindingNS)
            o << "}\n";

        writeFile(path, o.str());
    }

    SAFE_DELETE(constructorUniqueName);
    SAFE_DELETE(destructorUniqueName);
}
