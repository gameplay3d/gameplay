#include "Base.h"
#include "FileSystem.h"
#include "ScriptController.h"

#ifndef GP_NO_LUA_BINDINGS
#include "lua/lua_all_bindings.h"
#else
// Need to define global functions expoed by lua bindings that are used by ScriptController
#define luaConvertObjectPointer(ptr, fromType, toType) NULL
static const std::vector<std::string>& luaGetClassRelatives(const char* type)
{
    static std::vector<std::string> empty;
    return empty;
}
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
    lua_pop(sc->_lua, 1); \
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

#define PUSH_NESTED_VARIABLE(name, defaultValue, script) \
    int top = lua_gettop(_lua); \
    if (!getNestedVariable(_lua, name, script ? script->_env : 0)) \
    { \
        lua_settop(_lua, top); \
        return (defaultValue); \
    }

#define POP_NESTED_VARIABLE() \
    lua_settop(_lua, top)

namespace gameplay
{

extern void splitURL(const std::string& url, std::string* file, std::string* id);

/**
 * Pushes onto the stack, the value of the variable 'name' or the nested table value if 'name' is a '.' separated 
 * list of tables of the form "A.B.C.D", where A, B and C are tables and D is a variable name in the table C.
 * 
 * This function will not restore the stack if there is an error.
 * 
 * @param lua  The Lua state.
 * @param name The name of a variable or a '.' separated list of nested tables ending with a variable name.
 *             The name value may be in the format "A.B.C.D" where A is a table and B, C are child tables.
 *             D is any type, which will be accessed by the calling function.
 * @param env Optional script environment ID, or zero for the global script environment.
 * 
 * @return True if the tables were pushed on the stack or the variable was pushed. Returns false on error.
 */
static bool getNestedVariable(lua_State* lua, const char* name, int env = 0)
{
    if (strchr(name, '.') == NULL)
    {
        // Just a field name, no nested tables
        if (env)
        {
            lua_rawgeti(lua, LUA_REGISTRYINDEX, env);
            lua_pushstring(lua, name);
            lua_rawget(lua, -2);
        }
        else
        {
            lua_getglobal(lua, name);
        }
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

    if (env)
    {
        lua_rawgeti(lua, LUA_REGISTRYINDEX, env);
        lua_pushstring(lua, name);
        lua_rawget(lua, -2);
    }
    else
    {
        lua_getglobal(lua, start);
    }
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

Script* ScriptController::loadScript(const char* path, Script::Scope scope, bool forceReload)
{
    GP_ASSERT(path);

    Script* script = NULL;

    // For global scripts, check if a script with the same path and scope is already loaded.
    // Protected scripts are always loaded into a new instance.
    if (scope == Script::GLOBAL)
    {
        std::map<std::string, std::vector<Script*>>::iterator itr = _scripts.find(path);
        if (itr != _scripts.end())
        {

            for (size_t i = 0, count = itr->second.size(); i < count; ++i)
            {
                if (itr->second[i]->_scope == scope)
                {
                    // Found a script with the same path and scope that's already loaded
                    script = itr->second[i];
                    break;
                }
            }
        }
    }

    // If the script is already loaded and forceReload was not specified, increase its
    // reference count and return it
    if (script && !forceReload)
    {
        script->addRef();
        return script;
    }

    // Create a new script object if neccessary
    if (script == NULL)
    {
        script = new Script();
        script->_path = path;
        script->_scope = scope;
    }

    // Attempt to load the script into the given scope
    if (!loadScript(script))
    {
        // Freeing the script will cause it to be removed from _scripts
        SAFE_RELEASE(script);
    }

    return script;
}

bool ScriptController::loadScript(Script* script)
{
    GP_ASSERT(script);

    if (!FileSystem::fileExists(script->_path.c_str()))
    {
        GP_WARN("Failed to load script: %s. File does not exist.", script->_path.c_str());
        return false;
    }

    // Insert an entry into _scripts before loading the script, to prevent load recursion
    std::vector<Script*>& scripts = _scripts[script->_path];
    scripts.push_back(script);

    // Load the contents of the script, but don't execute it yet
    const char* scriptSource = FileSystem::readAll(script->_path.c_str());
    int ret = luaL_loadstring(_lua, scriptSource); // [chunk]
    SAFE_DELETE_ARRAY(scriptSource);

    if (ret == LUA_OK)
    {
        // If the requested scope is protected, create a new script env table to execute
        // the script within, using a metatable to fallback to the global table (_G)
        if (script->_scope == Script::PROTECTED)
        {
            // Create a new table as an environment for the new script
            lua_newtable(_lua); // new ENV for script [chunk, env]

            // Store a ref to the table in the registry (this pops the table) [chunk]
            script->_env = luaL_ref(_lua, LUA_REGISTRYINDEX);

            // Put the env table back on top of the stack
            lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env); // [chunk, env]

            // Create a metatable that forwards missed lookups to global table _G
            lua_newtable(_lua); // metatable [chunk, env, meta]
            lua_pushglobaltable(_lua); // pushes _G, which will be the __index metatable entry [chunk, env, meta, _G]

            // Set the __index property of the metatable to _G
            lua_setfield(_lua, -2, "__index"); // metatable on top [chunk, env, meta]

            // Set the metatable for our new environment table
            lua_setmetatable(_lua, -2); // [chunk, env]

            // Store a pointer to the ENV table in the table itself, so it can refer to itself.
            // This is similar to how the _G field works for accessing the global table, and
            // how _G._G is valid.
            lua_pushvalue(_lua, -1); // [chunk, env, env]
            lua_setfield(_lua, -2, "_THIS"); // [chunk, env]

            // Set the first upvalue (_ENV) for our chunk to the new environment table
            if (lua_setupvalue(_lua, -2, 1) == NULL) // [chunk]
            {
                GP_WARN("Error setting environment table for script: %s.", script->_path.c_str());
            }
        }

        // Execute the script
        ret = lua_pcall(_lua, 0, 0, 0);
    }

    if (ret != LUA_OK)
    {
        script->_env = 0; // clear _env on failure
        GP_WARN("Failed to load script: %s. %s.", script->_path.c_str(), lua_tostring(_lua, -1));
        return false;
    }

    return true;
}

void ScriptController::unloadScript(Script* script)
{
    if (script->_env != 0)
    {
        // Release the reference to the environment table for this non-global script
        luaL_unref(_lua, LUA_REGISTRYINDEX, script->_env);
        script->_env = 0;
    }

    // TODO: What else can we clean up here?
    // Can we test this with manual GC and breaking on gameplay object constructors that were delcared in the script?

    // Remove the script from our managed list
    std::map<std::string, std::vector<Script*>>::iterator itr = _scripts.find(script->_path);
    if (itr != _scripts.end())
    {
        std::vector<Script*>& scripts = itr->second;
        for (size_t i = 0, count = scripts.size(); i < count; ++i)
        {
            if (scripts[i] == script)
            {
                scripts.erase(scripts.begin() + i);
                break;
            }
        }

        if (scripts.empty())
        {
            _scripts.erase(itr);
        }
    }
}

bool ScriptController::getBool(const char* name, bool defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    bool b = lua_isboolean(_lua, -1) ? ScriptUtil::luaCheckBool(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return b;
}

char ScriptController::getChar(const char* name, char defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    char c = lua_isnumber(_lua, -1) ?  (char)luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return c;
}

short ScriptController::getShort(const char* name, short defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    short n = lua_isnumber(_lua, -1) ? (short)luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

int ScriptController::getInt(const char* name, int defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    int n = lua_isnumber(_lua, -1) ? luaL_checkint(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

long ScriptController::getLong(const char* name, long defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    long n = lua_isnumber(_lua, -1) ? luaL_checklong(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned char ScriptController::getUnsignedChar(const char* name, unsigned char defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    unsigned char c = lua_isnumber(_lua, -1) ? (unsigned char)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return c;
}

unsigned short ScriptController::getUnsignedShort(const char* name, unsigned short defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    unsigned short n = lua_isnumber(_lua, -1) ? (unsigned short)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned int ScriptController::getUnsignedInt(const char* name, unsigned int defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    unsigned int n = lua_isnumber(_lua, -1) ? (unsigned int)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

unsigned long ScriptController::getUnsignedLong(const char* name, unsigned long defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    unsigned long n = lua_isnumber(_lua, -1) ? (unsigned long)luaL_checkunsigned(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

float ScriptController::getFloat(const char* name, float defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    float f = lua_isnumber(_lua, -1) ? (float)luaL_checknumber(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return f;
}

double ScriptController::getDouble(const char* name, double defaultValue, Script* script)
{
    PUSH_NESTED_VARIABLE(name, defaultValue, script);
    double n = lua_isnumber(_lua, -1) ? (double)luaL_checknumber(_lua, -1) : defaultValue;
    POP_NESTED_VARIABLE();
    return n;
}

const char* ScriptController::getString(const char* name, Script* script)
{
    PUSH_NESTED_VARIABLE(name, NULL, script);
    const char* s = lua_isstring(_lua, -1) ? luaL_checkstring(_lua, -1) : NULL;
    POP_NESTED_VARIABLE();
    return s;
}

void* ScriptController::getObjectPointer(const char* type, const char* name, Script* script)
{
    PUSH_NESTED_VARIABLE(name, NULL, script);
    void* ptr = lua_islightuserdata(_lua, -1) ? lua_touserdata(_lua, -1) : ScriptUtil::getUserDataObjectPointer(-1, type);
    POP_NESTED_VARIABLE();
    return ptr;
}

void ScriptController::setBool(const char* name, bool v, Script* script)
{
    // TODO: Support setting variables in nested tables. Should just need to execute code similar to
    // that in getNestedVariable, except we only need to push the parent tables onto the stack,
    // NOT the actual field/variable (since we're going to set it).

    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushboolean(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushboolean(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setChar(const char* name, char v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushinteger(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushinteger(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setShort(const char* name, short v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushinteger(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushinteger(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setInt(const char* name, int v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushinteger(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushinteger(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setLong(const char* name, long v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushinteger(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushinteger(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setUnsignedChar(const char* name, unsigned char v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushunsigned(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushunsigned(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setUnsignedShort(const char* name, unsigned short v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushunsigned(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushunsigned(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setUnsignedInt(const char* name, unsigned int v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushunsigned(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushunsigned(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setUnsignedLong(const char* name, unsigned long v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushunsigned(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushunsigned(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setFloat(const char* name, float v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushnumber(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushnumber(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setDouble(const char* name, double v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushnumber(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushnumber(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setString(const char* name, const char* v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
        lua_pushstring(_lua, v);
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_pushstring(_lua, v);
        lua_setglobal(_lua, name);
    }
}

void ScriptController::setObjectPointer(const char* type, const char* name, void* v, Script* script)
{
    if (script && script->_env)
    {
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, script->_env);
    }

    // Push a new user data onto the stack (and setup its metatable)
    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(_lua, sizeof(ScriptUtil::LuaObject));
    object->instance = v;
    object->owns = false;
    luaL_getmetatable(_lua, type);
    lua_setmetatable(_lua, -2);

    if (script && script->_env)
    {
        lua_setfield(_lua, -2, name);
    }
    else
    {
        lua_setglobal(_lua, name);
    }
}

bool ScriptController::functionExists(const char* name, const Script* script) const
{
    PUSH_NESTED_VARIABLE(name, false, script);

    bool result = lua_isfunction(_lua, -1) ? true : false;

    POP_NESTED_VARIABLE();

    return result;
}

Script* ScriptController::getCurrentScript() const
{
    return _envStack.empty() ? NULL : _envStack.back();
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

#ifndef GP_NO_LUA_BINDINGS
    lua_RegisterAllBindings();
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

void ScriptController::finalize()
{
    // Cleanup any outstanding time listeners
    for (std::list<ScriptTimeListener*>::iterator itr = _timeListeners.begin(); itr != _timeListeners.end(); ++itr)
    {
        SAFE_DELETE(*itr);
    }
    _timeListeners.clear();

    if (_lua)
    {
        // Perform a full garbage collection cycle.
        // Note that this does NOT free any global variables declared in scripts, since 
        // they are stored in the global state and are still referenced. Only after 
        // closing the state (lua_close) will those variables be released.
        lua_gc(_lua, LUA_GCCOLLECT, 0);

        lua_close(_lua);
        _lua = NULL;
    }
}

bool ScriptController::executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list, Script* script)
{
    if (!_lua)
        return false; // handles calling this method after script is finalized

    if (func == NULL)
    {
        GP_ERROR("Lua function name must be non-null.");
        return false;
    }

    // If script was not specified and we are currently executing another function,
    // call this function in the previous call's environment (stored in _envStack).
    // This allows gameplay script callbacks (such as Scene.visit) to work locally when called
    // from an sandboxed script.
    if (!script && !_envStack.empty())
    {
        // Execute in the currently running script's environment
        script = _envStack.back();
    }
    int env = script ? script->_env : 0;

    if (!getNestedVariable(_lua, func, env))
    {
        GP_WARN("Failed to call function '%s'", func);
        return false;
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

                // We simply push enums as the integer values they represent
                lua_pushnumber(_lua, va_arg(*list, int));
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

    pushScript(script);

    // Perform the function call.
    // This will push 'resultCount' values onto the stack if it succeeds.
    // Otherwise (if it fails) it will push an error string onto the stack.
    bool success = lua_pcall(_lua, argumentCount, resultCount, 0) == 0;
    if (!success)
    {
        GP_WARN("Failed to call function '%s' with error '%s'.", func, lua_tostring(_lua, -1));
        lua_pop(_lua, 1); // pop the error
    }

    popScript();

    return success;
}

void ScriptController::schedule(float timeOffset, const char* function)
{
    // Get the currently execute script
    Script* script = _envStack.empty() ? NULL : _envStack.back();
    if (script)
    {
        // Increase the reference count of the script while we hold it so it doesn't
        // get destroyed while waiting for the event to fire.
        script->addRef();
    }

    ScriptTimeListener* listener = new ScriptTimeListener(script, function);
    _timeListeners.push_back(listener);

    Game::getInstance()->schedule(timeOffset, listener, NULL);
}

void ScriptController::pushScript(Script* script)
{
    // Increase the reference count of the script while it's pushed,
    // to prevent it from being destroyed during this time.
    if (script)
        script->addRef();

    _envStack.push_back(script);
}

void ScriptController::popScript()
{
    GP_ASSERT(!_envStack.empty());

    Script* script = _envStack.back();

    _envStack.pop_back();

    SAFE_RELEASE(script);
}

ScriptController::ScriptTimeListener::ScriptTimeListener(Script* script, const char* function) : script(script), function(function)
{
}

ScriptController::ScriptTimeListener::~ScriptTimeListener()
{
    // Release 
    SAFE_RELEASE(script);
}

void ScriptController::ScriptTimeListener::timeEvent(long timeDiff, void* cookie)
{
    // Remove ourself from the script controller's list
    std::list<ScriptTimeListener*>& list = Game::getInstance()->getScriptController()->_timeListeners;
    std::list<ScriptTimeListener*>::iterator itr = std::find(list.begin(), list.end(), this);
    if (itr != list.end())
        list.erase(itr);

    // Call the script function
    Game::getInstance()->getScriptController()->executeFunction<void>(script, function.c_str(), "l", NULL, timeDiff);

    // Free ourself.
    // IMPORTANT: Don't do anything else after this line!!
    delete this;
}

// Helper macros.
#define SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, type, checkfunc) \
    int top = lua_gettop(_lua); \
    bool success = executeFunctionHelper(1, func, NULL, NULL, script); \
    if (out && success) \
        *out = (type)checkfunc(_lua, -1); \
    lua_settop(_lua, top); \
    return success;

#define SCRIPT_EXECUTE_FUNCTION_PARAM(script, type, checkfunc) \
    int top = lua_gettop(_lua); \
    va_list list; \
    va_start(list, out); \
    bool success = executeFunctionHelper(1, func, args, &list, script); \
    if (out && success) \
        *out = (type)checkfunc(_lua, -1); \
    va_end(list); \
    lua_settop(_lua, top); \
    return success;

#define SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, type, checkfunc) \
    int top = lua_gettop(_lua); \
    bool success = executeFunctionHelper(1, func, args, list, script); \
    if (out && success) \
        *out = (type)checkfunc(_lua, -1); \
    lua_settop(_lua, top); \
    return success;

template<> bool ScriptController::executeFunction<void>(const char* func, void* out)
{
    return executeFunction<void>((Script*)NULL, func, out);
}

template<> bool ScriptController::executeFunction<bool>(const char* func, bool* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, bool, ScriptUtil::luaCheckBool);
}

template<> bool ScriptController::executeFunction<char>(const char* func, char* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, char, luaL_checkint);
}

template<> bool ScriptController::executeFunction<short>(const char* func, short* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, short, luaL_checkint);
}

template<> bool ScriptController::executeFunction<int>(const char* func, int* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, int, luaL_checkint);
}

template<> bool ScriptController::executeFunction<long>(const char* func, long* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, long, luaL_checklong);
}

template<> bool ScriptController::executeFunction<unsigned char>(const char* func, unsigned char* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, unsigned char, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned short>(const char* func, unsigned short* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, unsigned short, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned int>(const char* func, unsigned int* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, unsigned int, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned long>(const char* func, unsigned long* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, unsigned long, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<float>(const char* func, float* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, float, luaL_checknumber);
}

template<> bool ScriptController::executeFunction<double>(const char* func, double* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, double, luaL_checknumber);
}

template<> bool ScriptController::executeFunction<std::string>(const char* func, std::string* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(NULL, std::string, luaL_checkstring);
}

template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, void* out)
{
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(0, func, NULL, NULL, script);
    lua_settop(_lua, top);
    return success;
}

template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, bool* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, bool, ScriptUtil::luaCheckBool);
}

template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, char* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, char, luaL_checkint);
}

template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, short* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, short, luaL_checkint);
}

template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, int* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, int, luaL_checkint);
}

template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, long* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, long, luaL_checklong);
}

template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, unsigned char* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, unsigned char, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, unsigned short* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, unsigned short, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, unsigned int* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, unsigned int, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, unsigned long* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, unsigned long, luaL_checkunsigned);
}

template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, float* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, float, luaL_checknumber);
}

template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, double* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, double, luaL_checknumber);
}

template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, std::string* out)
{
    SCRIPT_EXECUTE_FUNCTION_NO_PARAM(script, std::string, luaL_checkstring);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(const char* func, const char* args, void* out, ...)
{
    int top = lua_gettop(_lua);
    va_list list;
    va_start(list, out);
    bool success = executeFunctionHelper(0, func, args, &list, NULL);
    va_end(list);
    lua_settop(_lua, top);
    return success;
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, bool* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(const char* func, const char* args, char* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, char, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(const char* func, const char* args, short* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, short, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(const char* func, const char* args, int* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, int, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(const char* func, const char* args, long* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, long, luaL_checklong);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(const char* func, const char* args, unsigned char* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(const char* func, const char* args, unsigned short* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(const char* func, const char* args, unsigned int* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(const char* func, const char* args, unsigned long* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(const char* func, const char* args, float* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, float, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(const char* func, const char* args, double* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, double, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(const char* func, const char* args, std::string* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(NULL, std::string, luaL_checkstring);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, const char* args, void* out, ...)
{
    int top = lua_gettop(_lua);
    va_list list;
    va_start(list, out);
    bool success = executeFunctionHelper(0, func, args, &list, script);
    va_end(list);
    lua_settop(_lua, top);
    return success;
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, const char* args, bool* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, const char* args, char* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, char, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, const char* args, short* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, short, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, const char* args, int* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, int, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, const char* args, long* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, long, luaL_checklong);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, const char* args, unsigned char* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, const char* args, unsigned short* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, const char* args, unsigned int* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, const char* args, unsigned long* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, const char* args, float* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, float, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, const char* args, double* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, double, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, const char* args, std::string* out, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(script, std::string, luaL_checkstring);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(const char* func, const char* args, void* out, va_list* list)
{
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(0, func, args, list, NULL);
    lua_settop(_lua, top);
    return success;
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, bool* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(const char* func, const char* args, char* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, char, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(const char* func, const char* args, short* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, short, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(const char* func, const char* args, int* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, int, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(const char* func, const char* args, long* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, long, luaL_checklong);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(const char* func, const char* args, unsigned char* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(const char* func, const char* args, unsigned short* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(const char* func, const char* args, unsigned int* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(const char* func, const char* args, unsigned long* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(const char* func, const char* args, float* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, float, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(const char* func, const char* args, double* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, double, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(const char* func, const char* args, std::string* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(NULL, std::string, luaL_checkstring);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, const char* args, void* out, va_list* list)
{
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(0, func, args, list, script);
    lua_settop(_lua, top);
    return success;
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, const char* args, bool* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, bool, ScriptUtil::luaCheckBool);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, const char* args, char* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, char, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, const char* args, short* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, short, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, const char* args, int* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, int, luaL_checkint);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, const char* args, long* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, long, luaL_checklong);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, const char* args, unsigned char* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, unsigned char, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, const char* args, unsigned short* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, unsigned short, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, const char* args, unsigned int* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, unsigned int, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, const char* args, unsigned long* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, unsigned long, luaL_checkunsigned);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, const char* args, float* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, float, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, const char* args, double* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, double, luaL_checknumber);
}

/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, const char* args, std::string* out, va_list* list)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM_LIST(script, std::string, luaL_checkstring);
}

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

void ScriptUtil::registerEnumValue(int enumValue, const std::string& enumValueString, const std::vector<std::string>& scopePath)
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

        // Add the enum value to the parent table.
        lua_pushnumber(sc->_lua, enumValue);
        lua_setfield(sc->_lua, -2, enumValueString.c_str());

        // Pop all the parent tables off the stack.
        int size = (int)scopePath.size();
        lua_pop(sc->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushnumber(sc->_lua, enumValue);
        lua_pushvalue(sc->_lua, -1);
        lua_setglobal(sc->_lua, enumValueString.c_str());
    }
}

void ScriptUtil::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction,
    lua_CFunction deleteFunction, const luaL_Reg* statics, const std::vector<std::string>& scopePath)
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

void* ScriptUtil::getUserDataObjectPointer(int index, const char* type)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // Get the base raw userdata pointer
    void* p = lua_touserdata(sc->_lua, index);
    if (p == NULL)
    {
        // No userdata at the given index
        return NULL;
    }

    // Push metatable of userdata object.
    if (!lua_getmetatable(sc->_lua, index))
    {
        // No metatable associated with user object, might simply be lightuserdata
        return NULL;
    }

    // Push metatable for the specified type.
    luaL_getmetatable(sc->_lua, type);

    // If the metatables are equal, the object is exactly the type specified
    if (lua_rawequal(sc->_lua, -1, -2))
    {
        // Found a match : pop both metatables.
        lua_pop(sc->_lua, 2);

        // Return the address as-is
        return ((ScriptUtil::LuaObject*)p)->instance;
    }

    // Pop metatable for the given type : instead, we'll check other types
    // in the inheritance tree of the type.
    lua_pop(sc->_lua, 1);

    const std::vector<std::string>& types = luaGetClassRelatives(type);
    for (size_t i = 0, count = types.size(); i < count; i++)
    {
        const std::string& relatedType = types[i];

        // Push relative type metatable
        luaL_getmetatable(sc->_lua, relatedType.c_str());

        // Compare the object's metatable to this relative's metatable
        if (lua_rawequal(sc->_lua, -1, -2))
        {
            // Found a match : pop both metatables.
            lua_pop(sc->_lua, 2);

            // Need to convert the raw userdata pointer to a valid object pointer of the given type
            return luaConvertObjectPointer(((ScriptUtil::LuaObject*)p)->instance, relatedType.c_str(), type);
        }
        // Pop relative type metatable
        lua_pop(sc->_lua, 1);
    }

    // Pop metatable of userdata object
    lua_pop(sc->_lua, 1);

    return NULL;
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

}
