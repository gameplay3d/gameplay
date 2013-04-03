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

        o << "// Lua bindings for " << classname << ".\n";
            
        // Write out the binding functions declarations.
        iter = bindings.begin();
        for (; iter != bindings.end(); iter++)
        {
            o << "int " << iter->second[0].getFunctionName() << "(lua_State* state);\n";
        }
        o << "\n";

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

        // Write out the function used to register the class with Lua.
        o << "void luaRegister_" << uniquename << "()\n";
        o << "{\n";
        
        // Get the member functions ready to bind.
        iter = bindings.begin();
        bool hasMembers = false;
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
        o << "lua_statics, scopePath);\n";
        o << "}\n\n";

        // Write out the function used to get the instance for
        // calling member functions and variables.
        generateInstanceGetter(o, classname, uniquename);

        // Write out the binding functions.
        iter = bindings.begin();
        for (; iter != bindings.end(); iter++)
        {
            FunctionBinding::write(o, iter->second);
        }

        if (bindingNS)
            o << "}\n";

        writeFile(path, o.str());
    }

    SAFE_DELETE(constructorUniqueName);
    SAFE_DELETE(destructorUniqueName);
}