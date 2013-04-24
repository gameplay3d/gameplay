#include "Base.h"
#include "FileSystem.h"
#include "ScriptController.h"

#ifndef NO_LUA_BINDINGS
#include "lua/lua_all_bindings.h"
#endif

#define GENERATE_LUA_GET_POINTER(type, checkFunc) \
    ScriptController* sc = Game::getInstance()->getScriptController(); \
    /* Check that the parameter is the correct type. */ \
    if (!lua_istable(sc->_lua, index)) \
    { \
        if (lua_islightuserdata(sc->_lua, index)) \
            return LuaArray<type>((type*)lua_touserdata(sc->_lua, index)); \
        lua_pushfstring(sc->_lua, "Expected a " #type " pointer (an array represented as a Lua table), got '%s' instead.", \
            luaL_typename(sc->_lua, index)); \
        lua_error(sc->_lua); \
        return LuaArray<type>((type*)NULL); \
    } \
    \
    /* Get the size of the array. */ \
    lua_len(sc->_lua, index); \
    int size = luaL_checkint(sc->_lua, -1); \
    if (size <= 0) \
        return LuaArray<type>((type*)NULL); \
    \
    /* Declare a LuaArray to store the values. */ \
    LuaArray<type> arr(size); \
    \
    /* Push the first key. */ \
    lua_pushnil(sc->_lua); \
    int i = 0; \
    for (; lua_next(sc->_lua, index) != 0 && i < size; i++) \
    { \
        arr[i] = (checkFunc(sc->_lua, -1)); \
        \
        /* Remove the value we just retrieved, but leave the key for the next iteration. */ \
        lua_pop(sc->_lua, 1); \
    } \
    \
    return arr

#define PUSH_NESTED_VARIABLE(name, defaultValue) \
    int top = lua_gettop(_lua); \
    if (!getNestedVariable(_lua, (name))) \
    { \
        lua_settop(_lua, top); \
        return (defaultValue); \
    }

#define POP_NESTED_VARIABLE() \
    lua_settop(_lua, top)

/**
 * Pushes onto the stack, the value of the global 'name' or the nested table value if 'name' is a '.' separated 
 * list of tables of the form "A.B.C.D", where A, B and C are tables and D is a variable name in the table C.
 * 
 * If 'name' does not contain any '.' then it is assumed to be the name of a global variable.
 * 
 * This function will not restore the stack if there is an error.
 * 
 * @param lua  The Lua state.
 * @param name The name of a global variable or a '.' separated list of nested tables ending with a variable name.
 *             The name value may be in the format "A.B.C.D" where A is a table and B, C are child tables.
 *             D is any type, which will be accessed by the calling function.
 * 
 * @return True if the tables were pushed on the stack or the global variable was pushed. Returns false on error.
 */
static bool getNestedVariable(lua_State* lua, const char* name)
{
    if (strchr(name, '.') == NULL)
    {
        lua_getglobal(lua, name);
        return true;
    }
    static std::string str;
    // Copy the input string to a std::string so we can modify it because 
    // some of the Lua functions require NULL terminated c-strings.
    str.assign(name);

    // Find the first table, which will be a global variable.
    char* start = const_cast<char*>(str.c_str());
    char* end = strchr(start, '.');
    if (end == NULL)
    {
        return false;
    }
    ++end;
    *(end - 1) = '\0';
    lua_getglobal(lua, start);
    *(end - 1) = '.';
    if (!lua_istable(lua, -1))
    {
        return false;
    }
    // Push the nested tables
    for (;;)
    {
        start = end;
        end = strchr(start, '.');
        if (end == NULL || *end == '\0')
        {
            // push the last variable
            lua_pushstring(lua, start);
            lua_gettable(lua, -2);
            return true;
        }
        else
        {
            // Push the next table
            *end = '\0';
            lua_pushstring(lua, start);
            *end = '.';
            lua_gettable(lua, -2);
            if (!lua_istable(lua, -1))
            {
                return false;
            }
            ++end;
            if (*end == '.')
            {
                return false;
            }
        }
    }
    return false;
}

namespace gameplay
{

extern void splitURL(const std::string& url, std::string* file, std::string* id);

void ScriptUtil::registerLibrary(const char* name, const luaL_Reg* functions)
{
    ScriptController* sc = Game::getInstance()->getScriptController();
    lua_newtable(sc->_lua);

    // Go through the list of functions and add them to the table.
    const luaL_Reg* iter = functions;
    for (; iter && iter->name; iter++)
    {
        lua_pushcfunction(sc->_lua, iter->func);
        lua_setfield(sc->_lua, -2, iter->name);
    }

    lua_setglobal(sc->_lua, name);
}

void ScriptUtil::registerConstantBool(const std::string& name, bool value, const std::vector<std::string>& scopePath)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (!scopePath.empty())
    {
        lua_getglobal(sc->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(sc->_lua, scopePath[i].c_str());
            lua_gettable(sc->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushboolean(sc->_lua, value);
        lua_setfield(sc->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = (int)scopePath.size();
        lua_pop(sc->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushboolean(sc->_lua, value);
        lua_pushvalue(sc->_lua, -1);
        lua_setglobal(sc->_lua, name.c_str());
    }
}

void ScriptUtil::registerConstantNumber(const std::string& name, double value, const std::vector<std::string>& scopePath)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (!scopePath.empty())
    {
        lua_getglobal(sc->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(sc->_lua, scopePath[i].c_str());
            lua_gettable(sc->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushnumber(sc->_lua, value);
        lua_setfield(sc->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = (int)scopePath.size();
        lua_pop(sc->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushnumber(sc->_lua, value);
        lua_pushvalue(sc->_lua, -1);
        lua_setglobal(sc->_lua, name.c_str());
    }
}

void ScriptUtil::registerConstantString(const std::string& name, const std::string& value, const std::vector<std::string>& scopePath)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (!scopePath.empty())
    {
        lua_getglobal(sc->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(sc->_lua, scopePath[i].c_str());
            lua_gettable(sc->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushstring(sc->_lua, value.c_str());
        lua_setfield(sc->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = (int)scopePath.size();
        lua_pop(sc->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushstring(sc->_lua, value.c_str());
        lua_pushvalue(sc->_lua, -1);
        lua_setglobal(sc->_lua, name.c_str());
    }
}

void ScriptUtil::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, 
    lua_CFunction deleteFunction, const luaL_Reg* statics,  const std::vector<std::string>& scopePath)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // If the type is an inner type, get the correct parent 
    // table on the stack before creating the table for the class.
    if (!scopePath.empty())
    {
        std::string tablename = name;

        // Strip off the scope path part of the name.
        lua_getglobal(sc->_lua, scopePath[0].c_str());
        std::size_t index = tablename.find(scopePath[0]);
        if (index != std::string::npos)
            tablename = tablename.substr(index + scopePath[0].size());
        
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(sc->_lua, scopePath[i].c_str());
            lua_gettable(sc->_lua, -2);

            index = tablename.find(scopePath[i]);
            if (index != std::string::npos)
                tablename = tablename.substr(index + scopePath[i].size());
        }

        lua_pushstring(sc->_lua, tablename.c_str());
        lua_newtable(sc->_lua);
    }
    else
    {
        // If the type is not an inner type, set it as a global table.
        lua_newtable(sc->_lua);
        lua_pushvalue(sc->_lua, -1);
        lua_setglobal(sc->_lua, name);
    }
    
    // Create the metatable and populate it with the member functions.
    lua_pushliteral(sc->_lua, "__metatable");
    luaL_newmetatable(sc->_lua, name);
    if (members)
        luaL_setfuncs(sc->_lua, members, 0);
    lua_pushstring(sc->_lua, "__index");
    lua_pushvalue(sc->_lua, -2);
    lua_settable(sc->_lua, -3);

    // Add the delete function if it was specified.
    if (deleteFunction)
    {
        lua_pushstring(sc->_lua, "__gc");
        lua_pushcfunction(sc->_lua, deleteFunction);
        lua_settable(sc->_lua, -3);
    }

    // Set the metatable on the main table.
    lua_settable(sc->_lua, -3);
    
    // Populate the main table with the static functions.
    if (statics)
        luaL_setfuncs(sc->_lua, statics, 0);

    // Set the new function(s) for the class.
    if (newFunction)
    {
        lua_pushliteral(sc->_lua, "new");
        lua_pushcfunction(sc->_lua, newFunction);
        lua_settable(sc->_lua, -3);
    }

    // Set the table we just created within the correct parent table.
    if (!scopePath.empty())
    {
        lua_settable(sc->_lua, -3);

        // Pop all the parent tables off the stack.
        int size = (int)scopePath.size();
        lua_pop(sc->_lua, size);
    }
    else
    {
        // Pop the main table off the stack.
        lua_pop(sc->_lua, 1);
    }
}

void ScriptUtil::registerFunction(const char* luaFunction, lua_CFunction cppFunction)
{
    lua_pushcfunction(Game::getInstance()->getScriptController()->_lua, cppFunction);
    lua_setglobal(Game::getInstance()->getScriptController()->_lua, luaFunction);
}

void ScriptUtil::setGlobalHierarchyPair(const std::string& base, const std::string& derived)
{
    Game::getInstance()->getScriptController()->_hierarchy[base].push_back(derived);
}

void ScriptUtil::addStringFromEnumConversionFunction(luaStringEnumConversionFunction stringFromEnum)
{
    Game::getInstance()->getScriptController()->_stringFromEnum.push_back(stringFromEnum);
}

ScriptUtil::LuaArray<bool> ScriptUtil::getBoolPointer(int index)
{
    GENERATE_LUA_GET_POINTER(bool, luaCheckBool);
}

ScriptUtil::LuaArray<short> ScriptUtil::getShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(short, (short)luaL_checkint);
}

ScriptUtil::LuaArray<int> ScriptUtil::getIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(int, (int)luaL_checkint);
}

ScriptUtil::LuaArray<long> ScriptUtil::getLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(long, (long)luaL_checkint);
}

ScriptUtil::LuaArray<unsigned char> ScriptUtil::getUnsignedCharPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned char, (unsigned char)luaL_checkunsigned);
}

ScriptUtil::LuaArray<unsigned short> ScriptUtil::getUnsignedShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned short, (unsigned short)luaL_checkunsigned);
}

ScriptUtil::LuaArray<unsigned int> ScriptUtil::getUnsignedIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned int, (unsigned int)luaL_checkunsigned);
}

ScriptUtil::LuaArray<unsigned long> ScriptUtil::getUnsignedLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned long, (unsigned long)luaL_checkunsigned);
}

ScriptUtil::LuaArray<float> ScriptUtil::getFloatPointer(int index)
{
    GENERATE_LUA_GET_POINTER(float, (float)luaL_checknumber);
}

ScriptUtil::LuaArray<double> ScriptUtil::getDoublePointer(int index)
{
    GENERATE_LUA_GET_POINTER(double, (double)luaL_checknumber);
}

const char* ScriptUtil::getString(int index, bool isStdString)
{
    if (lua_type(Game::getInstance()->getScriptController()->_lua, index) == LUA_TSTRING)
        return luaL_checkstring(Game::getInstance()->getScriptController()->_lua, index);
    else if (lua_type(Game::getInstance()->getScriptController()->_lua, index) == LUA_TNIL && !isStdString)
        return NULL;
    else
    {
        GP_ERROR("Invalid string parameter (index = %d).", index);
        return NULL;
    }
}

bool ScriptUtil::luaCheckBool(lua_State* state, int n)
{
    if (!lua_isboolean(state, n))
    {
        const char* msg = lua_pushfstring(state, "%s expected, got %s", lua_typename(state, LUA_TBOOLEAN), luaL_typename(state, n));
        luaL_argerror(state, n, msg);
        return false;
    }
    return (lua_toboolean(state, n) != 0);
}


void ScriptController::loadScript(const char* path, bool forceReload)
{
    GP_ASSERT(path);
    std::set<std::string>::iterator iter = _loadedScripts.find(path);
    if (iter == _loadedScripts.end() || forceReload)
    {
#ifdef __ANDROID__
        const char* scriptContents = FileSystem::readAll(path);
        if (luaL_dostring(_lua, scriptContents))
        {
            GP_WARN("Failed to run Lua script with error: '%s'.", lua_tostring(_lua, -1));
        }
        SAFE_DELETE_ARRAY(scriptContents);
#else
        std::string fullPath;
        if (!FileSystem::isAbsolutePath(path))
        {
            fullPath.append(FileSystem::getResourcePath());
        }
        fullPath.append(path);
        if (luaL_dofile(_lua, fullPath.c_str()))
        {
            GP_WARN("Failed to run Lua script with error: '%s'.", lua_tostring(_lua, -1));
        }
#endif
        if (iter == _loadedScripts.end())
        {
            _loadedScripts.insert(path);
        }
    }
}

std::string ScriptController::loadUrl(const char* url)
{
    std::string file;
    std::string id;
    splitURL(url, &file, &id);

    // Make sure the function isn't empty.
    if (id.size() <= 0)
    {
        GP_ERROR("Got an empty function name when parsing function url '%s'.", url);
        return std::string();
    }

    // Ensure the script is loaded.
    if (file.size() > 0)
        Game::getInstance()->getScriptController()->loadScript(file.c_str());

    // Return the function name.
    return id;
}

bool ScriptController::getBool(const char* name, bool defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    bool b = lua_isboolean(_lua, -1) ? ScriptUtil::luaCheckBool(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return b;
}

char ScriptController::getChar(const char* name, char defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    char c = lua_isnumber(_lua, -1) ?  (char)luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return c;
}

short ScriptController::getShort(const char* name, short defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    short n = lua_isnumber(_lua, -1) ? (short)luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

int ScriptController::getInt(const char* name, int defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    int n = lua_isnumber(_lua, -1) ? luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

long ScriptController::getLong(const char* name, long defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    long n = lua_isnumber(_lua, -1) ? luaL_checklong(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned char ScriptController::getUnsignedChar(const char* name, unsigned char defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    unsigned char c = lua_isnumber(_lua, -1) ? (unsigned char)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return c;
}

unsigned short ScriptController::getUnsignedShort(const char* name, unsigned short defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    unsigned short n = lua_isnumber(_lua, -1) ? (unsigned short)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned int ScriptController::getUnsignedInt(const char* name, unsigned int defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    unsigned int n = lua_isnumber(_lua, -1) ? (unsigned int)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned long ScriptController::getUnsignedLong(const char* name, unsigned long defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    unsigned long n = lua_isnumber(_lua, -1) ? (unsigned long)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

float ScriptController::getFloat(const char* name, float defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    float f = lua_isnumber(_lua, -1) ? (float)luaL_checknumber(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return f;
}

double ScriptController::getDouble(const char* name, double defaultValue)
{
    PUSH_NESTED_VARIABLE(name, defaultValue);
    double n = lua_isnumber(_lua, -1) ? (double)luaL_checknumber(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

const char* ScriptController::getString(const char* name)
{
    PUSH_NESTED_VARIABLE(name, NULL);
    const char* s = lua_isstring(_lua, -1) ? luaL_checkstring(_lua, -1) : NULL;
    POP_NESTED_VARIABLE();
    return s;
}

void ScriptController::setBool(const char* name, bool v)
{
    lua_pushboolean(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setChar(const char* name, char v)
{
    lua_pushinteger(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setShort(const char* name, short v)
{
    lua_pushinteger(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setInt(const char* name, int v)
{
    lua_pushinteger(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setLong(const char* name, long v)
{
    lua_pushinteger(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setUnsignedChar(const char* name, unsigned char v)
{
    lua_pushunsigned(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setUnsignedShort(const char* name, unsigned short v)
{
    lua_pushunsigned(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setUnsignedInt(const char* name, unsigned int v)
{
    lua_pushunsigned(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setUnsignedLong(const char* name, unsigned long v)
{
    lua_pushunsigned(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setFloat(const char* name, float v)
{
    lua_pushnumber(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setDouble(const char* name, double v)
{
    lua_pushnumber(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::setString(const char* name, const char* v)
{
    lua_pushstring(_lua, v);
    lua_setglobal(_lua, name);
}

void ScriptController::print(const char* str)
{
    gameplay::print("%s", str);
}

void ScriptController::print(const char* str1, const char* str2)
{
    gameplay::print("%s%s", str1, str2);
}

ScriptController::ScriptController() : _lua(NULL)
{
}

ScriptController::~ScriptController()
{
}

static const char* lua_print_function = 
    "function print(...)\n"
    "    ScriptController.print(table.concat({...},\"\\t\"), \"\\n\")\n"
    "end\n";

static const char* lua_loadfile_function = 
    "do\n"
    "    local oldLoadfile = loadfile\n"
    "    loadfile = function(filename)\n"
    "        if filename ~= nil and not FileSystem.isAbsolutePath(filename) then\n"
    "            FileSystem.createFileFromAsset(filename)\n"
    "            filename = FileSystem.getResourcePath() .. filename\n"
    "        end\n"
    "        return oldLoadfile(filename)\n"
    "    end\n"
    "end\n";

static const char* lua_dofile_function = 
    "do\n"
    "    local oldDofile = dofile\n"
    "    dofile = function(filename)\n"
    "        if filename ~= nil and not FileSystem.isAbsolutePath(filename) then\n"
    "            FileSystem.createFileFromAsset(filename)\n"
    "            filename = FileSystem.getResourcePath() .. filename\n"
    "        end\n"
    "        return oldDofile(filename)\n"
    "    end\n"
    "end\n";

/**
 * @script{ignore}
 */
void appendLuaPath(lua_State* state, const char* path)
{
    lua_getglobal(state, "package");

    // Get the current path string from top of stack
    lua_getfield(state, -1, "path");
    std::string cur_path = lua_tostring(state, -1);
    lua_pop(state, 1);

    // Append our game resource path to the path
    cur_path += ';';
    cur_path += path;
    cur_path += "?.lua";

    // Push the new path
    lua_pushstring(state, cur_path.c_str());
    lua_setfield(state, -2, "path");
    lua_pop(state, 1);
}

void ScriptController::initialize()
{
    _lua = luaL_newstate();
    if (!_lua)
        GP_ERROR("Failed to initialize Lua scripting engine.");
    luaL_openlibs(_lua);

#ifndef NO_LUA_BINDINGS
    lua_RegisterAllBindings();
    ScriptUtil::registerFunction("convert", ScriptController::convert);
#endif

    // Append to the LUA_PATH to allow scripts to be found in the resource folder on all platforms
    appendLuaPath(_lua, FileSystem::getResourcePath());

    // Create our own print() function that uses gameplay::print.
    if (luaL_dostring(_lua, lua_print_function))
        GP_ERROR("Failed to load custom print() function with error: '%s'.", lua_tostring(_lua, -1));

    // Change the functions that read a file to use FileSystem.getResourcePath as their base path.
    if (luaL_dostring(_lua, lua_loadfile_function))
        GP_ERROR("Failed to load custom loadfile() function with error: '%s'.", lua_tostring(_lua, -1));
    if (luaL_dostring(_lua, lua_dofile_function))
        GP_ERROR("Failed to load custom dofile() function with error: '%s'.", lua_tostring(_lua, -1));

    // Write game command-line arguments to a global lua "arg" table
    std::ostringstream args;
    int argc;
    char** argv;
    Game::getInstance()->getArguments(&argc, &argv);
    args << "arg = { }\n";
    for (int i = 0; i < argc; ++i)
    {
        args << "arg[" << (i) << "] = [[" << argv[i] << "]]\n";
    }
    std::string argsStr = args.str();
    if (argsStr.length() > 0)
    {
        if (luaL_dostring(_lua, argsStr.c_str()))
            GP_ERROR("Failed to pass command-line arguments with error: '%s'.", lua_tostring(_lua, -1));
    }
}

void ScriptController::initializeGame()
{
    std::vector<std::string>& list = _callbacks[INITIALIZE];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str());
}

void ScriptController::finalize()
{
    if (_lua)
	{
        lua_close(_lua);
		_lua = NULL;
	}
}

void ScriptController::finalizeGame()
{
    std::vector<std::string> finalizeCallbacks = _callbacks[FINALIZE]; // no & : makes a copy of the vector

	// Remove any registered callbacks so they don't get called after shutdown
	for (unsigned int i = 0; i < CALLBACK_COUNT; i++)
        _callbacks[i].clear();

	// Fire script finalize callbacks
    for (size_t i = 0; i < finalizeCallbacks.size(); ++i)
        executeFunction<void>(finalizeCallbacks[i].c_str());

    // Perform a full garbage collection cycle.
	// Note that this does NOT free any global variables declared in scripts, since 
	// they are stored in the global state and are still referenced. Only after 
	// closing the state (lua_close) will those variables be released.
    lua_gc(_lua, LUA_GCCOLLECT, 0);
}

void ScriptController::update(float elapsedTime)
{
    std::vector<std::string>& list = _callbacks[UPDATE];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "f", elapsedTime);
}

void ScriptController::render(float elapsedTime)
{
    std::vector<std::string>& list = _callbacks[RENDER];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "f", elapsedTime);
}

void ScriptController::resizeEvent(unsigned int width, unsigned int height)
{
    std::vector<std::string>& list = _callbacks[RESIZE_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "uiui", width, height);
}

void ScriptController::keyEvent(Keyboard::KeyEvent evt, int key)
{
    std::vector<std::string>& list = _callbacks[KEY_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "[Keyboard::KeyEvent][Keyboard::Key]", evt, key);
}

void ScriptController::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    std::vector<std::string>& list = _callbacks[TOUCH_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "[Touch::TouchEvent]iiui", evt, x, y, contactIndex);
}

bool ScriptController::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    std::vector<std::string>& list = _callbacks[MOUSE_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (executeFunction<bool>(list[i].c_str(), "[Mouse::MouseEvent]iii", evt, x, y, wheelDelta))
            return true;
    }
    return false;
}

void ScriptController::gestureSwipeEvent(int x, int y, int direction)
{
    std::vector<std::string>& list = _callbacks[GESTURE_SWIPE_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "iii", x, y, direction);
}

void ScriptController::gesturePinchEvent(int x, int y, float scale)
{
    std::vector<std::string>& list = _callbacks[GESTURE_PINCH_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "iif", x, y, scale);
}

void ScriptController::gestureTapEvent(int x, int y)
{
    std::vector<std::string>& list = _callbacks[GESTURE_TAP_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "ii", x, y);
}

void ScriptController::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    std::vector<std::string>& list = _callbacks[GAMEPAD_EVENT];
    for (size_t i = 0; i < list.size(); ++i)
        executeFunction<void>(list[i].c_str(), "[Gamepad::GamepadEvent]<Gamepad>", evt, gamepad);
}

void ScriptController::executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list)
{
	if (!_lua)
		return; // handles calling this method after script is finalized

    if (func == NULL)
    {
        GP_ERROR("Lua function name must be non-null.");
        return;
    }

    if (!getNestedVariable(_lua, func))
    {
        GP_WARN("Failed to call function '%s'", func);
        return;
    }

    const char* sig = args;
    int argumentCount = 0;

    // Push the arguments to the Lua stack if there are any.
    if (sig)
    {
        while (true)
        {
            if (!(*sig))
                break;

            switch(*sig++)
            {
            // Signed integers.
            case 'c':
            case 'h':
            case 'i':
            case 'l':
                lua_pushinteger(_lua, va_arg(*list, int));
                break;
            // Unsigned integers.
            case 'u':
                // Skip past the actual type (long, int, short, char).
                sig++;
                lua_pushunsigned(_lua, va_arg(*list, int));
                break;
            // Booleans.
            case 'b':
                lua_pushboolean(_lua, va_arg(*list, int));
                break;
            // Floating point numbers.
            case 'f':
            case 'd':
                lua_pushnumber(_lua, va_arg(*list, double));
                break;
            // Strings.
            case 's':
                lua_pushstring(_lua, va_arg(*list, char*));
                break;
            // Pointers.
            case 'p':
                lua_pushlightuserdata(_lua, va_arg(*list, void*));
                break;
            // Enums.
            case '[':
            {
                std::string type = sig;
                type = type.substr(0, type.find("]"));

                // Skip past the closing ']' (the semi-colon here is intentional-do not remove).
                while (*sig++ != ']');

                unsigned int value = va_arg(*list, int);
                std::string enumStr = "";
                for (unsigned int i = 0; enumStr.size() == 0 && i < _stringFromEnum.size(); i++)
                {
                    enumStr = (*_stringFromEnum[i])(type, value);
                }

                lua_pushstring(_lua, enumStr.c_str());
                break;
            }
            // Object references/pointers (Lua userdata).
            case '<':
            {
                std::string type = sig;
                type = type.substr(0, type.find(">"));

                // Skip past the closing '>' (the semi-colon here is intentional-do not remove).
                while (*sig++ != '>');

                // Calculate the unique Lua type name.
                size_t i = type.find("::");
                while (i != std::string::npos)
                {
                    // We use "" as the replacement here-this must match the preprocessor
                    // define SCOPE_REPLACEMENT from the gameplay-luagen project.
                    type.replace(i, 2, "");
                    i = type.find("::");
                }

                void* ptr = va_arg(*list, void*);
                if (ptr == NULL)
                {
                    lua_pushnil(_lua);
                }
                else
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(_lua, sizeof(ScriptUtil::LuaObject));
                    object->instance = ptr;
                    object->owns = false;
                    luaL_getmetatable(_lua, type.c_str());
                    lua_setmetatable(_lua, -2);
                }
                break;
            }
            default:
                GP_ERROR("Invalid argument type '%d'.", *(sig - 1));
                break;
            }

            argumentCount++;
            luaL_checkstack(_lua, 1, "Too many arguments.");
        }
    }

    // Perform the function call.
    if (lua_pcall(_lua, argumentCount, resultCount, 0) != 0)
        GP_WARN("Failed to call function '%s' with error '%s'.", func, lua_tostring(_lua, -1));
}

void ScriptController::registerCallback(const char* callback, const char* function)
{
    ScriptCallback scb = toCallback(callback);
    if (scb < INVALID_CALLBACK)
    {
        _callbacks[scb].push_back(function);
    }
    else
    {
        GP_WARN("Invalid script callback function specified: %s", callback);
    }
}

void ScriptController::unregisterCallback(const char* callback, const char* function)
{
    ScriptCallback scb = toCallback(callback);
    if (scb < INVALID_CALLBACK)
    {
        std::vector<std::string>& list = _callbacks[scb];
        std::vector<std::string>::iterator itr = std::find(list.begin(), list.end(), std::string(function));
        if (itr != list.end())
            list.erase(itr);
    }
    else
    {
        GP_WARN("Invalid script callback function specified: %s", callback);
    }
}

ScriptController::ScriptCallback ScriptController::toCallback(const char* name)
{
    if (strcmp(name, "initialize") == 0)
        return ScriptController::INITIALIZE;
    else if (strcmp(name, "update") == 0)
        return ScriptController::UPDATE;
    else if (strcmp(name, "render") == 0)
        return ScriptController::RENDER;
    else if (strcmp(name, "finalize") == 0)
        return ScriptController::FINALIZE;
    else if (strcmp(name, "resizeEvent") == 0)
        return ScriptController::RESIZE_EVENT;
    else if (strcmp(name, "keyEvent") == 0)
        return ScriptController::KEY_EVENT;
    else if (strcmp(name, "touchEvent") == 0)
        return ScriptController::TOUCH_EVENT;
    else if (strcmp(name, "mouseEvent") == 0)
        return ScriptController::MOUSE_EVENT;
    else if (strcmp(name, "gestureSwipeEvent") == 0)
        return ScriptController::GESTURE_SWIPE_EVENT;
    else if (strcmp(name, "gesturePinchEvent") == 0)
        return ScriptController::GESTURE_PINCH_EVENT;
    else if (strcmp(name, "gestureTapEvent") == 0)
        return ScriptController::GESTURE_TAP_EVENT;
    else if (strcmp(name, "gamepadEvent") == 0)
        return ScriptController::GAMEPAD_EVENT;
    else
        return ScriptController::INVALID_CALLBACK;
}

int ScriptController::convert(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);
    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA && lua_type(state, 2) == LUA_TSTRING )
            {
                // Get parameter 2
                const char* param2 = ScriptUtil::getString(2, false);
                if (param2 != NULL)
                {
                    luaL_getmetatable(state, param2);
                    lua_setmetatable(state, -3);
                }
                return 0;
            }

            lua_pushstring(state, "lua_convert - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

// Helper macros.
#define SCRIPT_EXECUTE_FUNCTION_NO_PARAM(type, checkfunc) \
    int top = lua_gettop(_lua); \
    executeFunctionHelper(1, func, NULL, NULL); \
    type value = (type)checkfunc(_lua, -1); \
    lua_pop(_lua, -1); \
    lua_settop(_lua, top); \
    return value;

#define SCRIPT_EXECUTE_FUNCTION_PARAM(type, checkfunc) \
    int top = lua_gettop(_lua); \
    va_list list; \
    va_start(list, args); \
    executeFunctionHelper(1, func, args, &list); \
    type value = (type)checkfunc(_lua, -1); \
    lua_pop(_lua, -1); \
    va_end(list); \
    lua_settop(_lua, top); \
    return value;

#define SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(type, checkfunc) \
    int top = lua_gettop(_lua); \
    executeFunctionHelper(1, func, args, list); \
    type value = (type)checkfunc(_lua, -1); \
    lua_pop(_lua, -1); \
    lua_settop(_lua, top); \
    return value;

template<> void ScriptController::executeFunction<void>(const char* func)
{
    int top = lua_gettop(_lua);
    executeFunctionHelper(0, func, NULL, NULL);
    lua_settop(_lua, top);
}

template<> bool ScriptController::executeFunction<bool>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(bool, ScriptUtil::luaCheckBool);
}

template<> char ScriptController::executeFunction<char>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(char, luaL_checkint);
}

template<> short ScriptController::executeFunction<short>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(short, luaL_checkint);
}

template<> int ScriptController::executeFunction<int>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(int, luaL_checkint);
}

template<> long ScriptController::executeFunction<long>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(long, luaL_checklong);
}

template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(unsigned char, luaL_checkunsigned);
}

template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(unsigned short, luaL_checkunsigned);
}

template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(unsigned int, luaL_checkunsigned);
}

template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(unsigned long, luaL_checkunsigned);
}

template<> float ScriptController::executeFunction<float>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(float, luaL_checknumber);
}

template<> double ScriptController::executeFunction<double>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(double, luaL_checknumber);
}

template<> std::string ScriptController::executeFunction<std::string>(const char* func)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(std::string, luaL_checkstring);
}

/** Template specialization. */
template<> void ScriptController::executeFunction<void>(const char* func, const char* args, ...)
{
    int top = lua_gettop(_lua);
    va_list list;
    va_start(list, args);
    executeFunctionHelper(0, func, args, &list);
    va_end(list);
    lua_settop(_lua, top);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(char, luaL_checkint);
}

/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(short, luaL_checkint);
}

/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(int, luaL_checkint);
}

/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(long, luaL_checklong);
}

/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(float, luaL_checknumber);
}

/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(double, luaL_checknumber);
}

/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(std::string, luaL_checkstring);
}

/** Template specialization. */
template<> void ScriptController::executeFunction<void>(const char* func, const char* args, va_list* list)
{
    executeFunctionHelper(0, func, args, list);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(char, luaL_checkint);
}

/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(short, luaL_checkint);
}

/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(int, luaL_checkint);
}

/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(long, luaL_checklong);
}

/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(float, luaL_checknumber);
}

/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(double, luaL_checknumber);
}

/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(std::string, luaL_checkstring);
}

}
