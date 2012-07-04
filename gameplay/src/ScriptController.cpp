#include "Base.h"
#include "ScriptController.h"
#include "lua/lua_all_bindings.h"

namespace gameplay
{

ScriptController* ScriptController::__instance = NULL;

#define GENERATE_LUA_GET_POINTER(type, checkFunc) \
    /* Check that the parameter is the correct type. */ \
    if (!lua_istable(_lua, index)) \
    { \
        if (lua_islightuserdata(_lua, index)) \
            return (type*)lua_touserdata(_lua, index); \
        lua_pushfstring(_lua, "Expected a " #type " pointer (an array represented as a Lua table), got '%s' instead.", luaL_typename(_lua, index)); \
        lua_error(_lua); \
        return NULL; \
    } \
    \
    /* Create a vector to store the values. */ \
    std::vector<type> values; \
    \
    /* Push the first key. */ \
    lua_pushnil(_lua); \
    while (lua_next(_lua, index) != 0) \
    { \
        values.push_back(checkFunc(_lua, -1)); \
        \
        /* Remove the value we just retrieved, but leave the key for the next iteration. */ \
        lua_pop(_lua, 1); \
    } \
    \
    /* Copy the values into an array. */ \
    if (values.size() > 0) \
    { \
        type* ptr = new type[values.size()]; \
        std::copy(values.begin(), values.end(), ptr); \
        return ptr; \
    } \
    return NULL


ScriptController* ScriptController::getInstance()
{
    return __instance;
}

bool* ScriptController::getBoolPointer(int index)
{
    GENERATE_LUA_GET_POINTER(bool, luaCheckBool);
}

short* ScriptController::getShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(short, (short)luaL_checkint);
}

int* ScriptController::getIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(int, (int)luaL_checkint);
}

long* ScriptController::getLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(long, (long)luaL_checkint);
}

unsigned char* ScriptController::getUnsignedCharPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned char, (unsigned char)luaL_checkunsigned);
}

unsigned short* ScriptController::getUnsignedShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned short, (unsigned short)luaL_checkunsigned);
}

unsigned int* ScriptController::getUnsignedIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned int, (unsigned int)luaL_checkunsigned);
}

unsigned long* ScriptController::getUnsignedLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned long, (unsigned long)luaL_checkunsigned);
}

float* ScriptController::getFloatPointer(int index)
{
    GENERATE_LUA_GET_POINTER(float, (float)luaL_checknumber);
}

double* ScriptController::getDoublePointer(int index)
{
    GENERATE_LUA_GET_POINTER(double, (double)luaL_checknumber);
}

const char* ScriptController::getString(int index, bool isStdString)
{
    if (lua_type(_lua, index) == LUA_TSTRING)
        return luaL_checkstring(_lua, index);
    else if (lua_type(_lua, index) == LUA_TNIL && !isStdString)
        return NULL;
    else
    {
        GP_ERROR("Invalid string parameter (index = %d).", index);
        return NULL;
    }
}

void ScriptController::loadScript(const char* path)
{
    if (luaL_dofile(_lua, path))
        GP_ERROR("Failed to run Lua script with error: '%s'.", lua_tostring(_lua, -1));
}

bool ScriptController::getBool(const char* name)
{
    lua_getglobal(_lua, name);
    return luaCheckBool(_lua, -1);
}

char ScriptController::getChar(const char* name)
{
    lua_getglobal(_lua, name);
    return (char)luaL_checkint(_lua, -1);
}

short ScriptController::getShort(const char* name)
{
    lua_getglobal(_lua, name);
    return (short)luaL_checkint(_lua, -1);
}

int ScriptController::getInt(const char* name)
{
    lua_getglobal(_lua, name);
    return luaL_checkint(_lua, -1);
}

long ScriptController::getLong(const char* name)
{
    lua_getglobal(_lua, name);
    return luaL_checklong(_lua, -1);
}

unsigned char ScriptController::getUnsignedChar(const char* name)
{
    lua_getglobal(_lua, name);
    return (unsigned char)luaL_checkunsigned(_lua, -1);
}

unsigned short ScriptController::getUnsignedShort(const char* name)
{
    lua_getglobal(_lua, name);
    return (unsigned short)luaL_checkunsigned(_lua, -1);
}

unsigned int ScriptController::getUnsignedInt(const char* name)
{
    lua_getglobal(_lua, name);
    return (unsigned int)luaL_checkunsigned(_lua, -1);
}

unsigned long ScriptController::getUnsignedLong(const char* name)
{
    lua_getglobal(_lua, name);
    return (unsigned long)luaL_checkunsigned(_lua, -1);
}

float ScriptController::getFloat(const char* name)
{
    lua_getglobal(_lua, name);
    return (float)luaL_checknumber(_lua, -1);
}

double ScriptController::getDouble(const char* name)
{
    lua_getglobal(_lua, name);
    return (double)luaL_checknumber(_lua, -1);
}

const char* ScriptController::getString(const char* name)
{
    lua_getglobal(_lua, name);
    return luaL_checkstring(_lua, -1);
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

void ScriptController::registerLibrary(const char* name, const luaL_Reg* functions)
{
    lua_newtable(_lua);

    // Go through the list of functions and add them to the table.
    const luaL_Reg* iter = functions;
    for (; iter && iter->name; iter++)
    {
        lua_pushcfunction(_lua, iter->func);
        lua_setfield(_lua, -2, iter->name);
    }

    lua_setglobal(_lua, name);
}

void ScriptController::registerConstantBool(std::string name, bool value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(_lua, scopePath[i].c_str());
            lua_gettable(_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushboolean(_lua, value);
        lua_setfield(_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushboolean(_lua, value);
        lua_pushvalue(_lua, -1);
        lua_setglobal(_lua, name.c_str());
    }
}

void ScriptController::registerConstantNumber(std::string name, double value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(_lua, scopePath[i].c_str());
            lua_gettable(_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushnumber(_lua, value);
        lua_setfield(_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushnumber(_lua, value);
        lua_pushvalue(_lua, -1);
        lua_setglobal(_lua, name.c_str());
    }
}

void ScriptController::registerConstantString(std::string name, std::string value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(_lua, scopePath[i].c_str());
            lua_gettable(_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushstring(_lua, value.c_str());
        lua_setfield(_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushstring(_lua, value.c_str());
        lua_pushvalue(_lua, -1);
        lua_setglobal(_lua, name.c_str());
    }
}

void ScriptController::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, 
    lua_CFunction deleteFunction, const luaL_Reg* statics,  std::vector<std::string> scopePath)
{
    // If the type is an inner type, get the correct parent 
    // table on the stack before creating the table for the class.
    if (scopePath.size() > 0)
    {
        std::string tablename = name;

        // Strip off the scope path part of the name.
        lua_getglobal(_lua, scopePath[0].c_str());
        std::size_t index = tablename.find(scopePath[0]);
        if (index != tablename.npos)
            tablename = tablename.substr(index + scopePath[0].size());
        
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(_lua, scopePath[i].c_str());
            lua_gettable(_lua, -2);

            index = tablename.find(scopePath[i]);
            if (index != tablename.npos)
                tablename = tablename.substr(index + scopePath[i].size());
        }

        lua_pushstring(_lua, tablename.c_str());
        lua_newtable(_lua);
    }
    else
    {
        // If the type is not an inner type, set it as a global table.
        lua_newtable(_lua);
        lua_pushvalue(_lua, -1);
        lua_setglobal(_lua, name);
    }
    
    // Create the metatable and populate it with the member functions.
    lua_pushliteral(_lua, "__metatable");
    luaL_newmetatable(_lua, name);
    if (members)
        luaL_setfuncs(_lua, members, 0);
    lua_pushstring(_lua, "__index");
    lua_pushvalue(_lua, -2);
    lua_settable(_lua, -3);

    // Add the delete function if it was specified.
    if (deleteFunction)
    {
        lua_pushstring(_lua, "__gc");
        lua_pushcfunction(_lua, deleteFunction);
        lua_settable(_lua, -3);
    }

    // Set the metatable on the main table.
    lua_settable(_lua, -3);
    
    // Populate the main table with the static functions.
    if (statics)
        luaL_setfuncs(_lua, statics, 0);

    // Set the new function(s) for the class.
    if (newFunction)
    {
        lua_pushliteral(_lua, "new");
        lua_pushcfunction(_lua, newFunction);
        lua_settable(_lua, -3);
    }

    // Set the table we just created within the correct parent table.
    if (scopePath.size() > 0)
    {
        lua_settable(_lua, -3);

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(_lua, size);
    }
    else
    {
        // Pop the main table off the stack.
        lua_pop(_lua, 1);
    }
}

void ScriptController::registerFunction(const char* luaFunction, lua_CFunction cppFunction)
{
    lua_pushcfunction(_lua, cppFunction);
    lua_setglobal(_lua, luaFunction);
}

void ScriptController::setGlobalHierarchy(std::map<std::string, std::vector<std::string> > hierarchy)
{
    _hierarchy = hierarchy;
}

ScriptController::ScriptController()
{
    memset(_callbacks, 0, sizeof(std::string*) * CALLBACK_COUNT);
    __instance = this;
}

ScriptController::~ScriptController()
{
    __instance = NULL;
}

void ScriptController::initialize()
{
    _lua = luaL_newstate();
    if (!_lua)
        GP_ERROR("Failed to initialize Lua scripting engine.");
    luaL_openlibs(_lua);
    lua_RegisterAllBindings();
}

void ScriptController::initializeGame()
{
    if (_callbacks[INITIALIZE])
    {
        executeFunction<void>(_callbacks[INITIALIZE]->c_str(), NULL);
    }
}

void ScriptController::finalize()
{
    lua_close(_lua);
}

void ScriptController::finalizeGame()
{
    if (_callbacks[FINALIZE])
    {
        executeFunction<void>(_callbacks[FINALIZE]->c_str(), NULL);
    }
}

void ScriptController::update(long elapsedTime)
{
    if (_callbacks[UPDATE])
    {
        executeFunction<void>(_callbacks[UPDATE]->c_str(), "f", elapsedTime);
    }
}

void ScriptController::render(long elapsedTime)
{
    if (_callbacks[RENDER])
    {
        executeFunction<void>(_callbacks[RENDER]->c_str(), "f", elapsedTime);
    }
}

void ScriptController::executeFunctionHelper(int resultCount, const char* func, const char* args, va_list& list)
{
    const char* sig = args;

    int argumentCount = 0;
    lua_getglobal(_lua, func);

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
                lua_pushinteger(_lua, va_arg(list, int));
                break;
            // Unsigned integers.
            case 'u':
                // Skip past the actual type (long, int, short, char).
                sig++;
                lua_pushunsigned(_lua, va_arg(list, int));
                break;
            // Booleans.
            case 'b':
                lua_pushboolean(_lua, va_arg(list, int));
                break;
            // Floating point numbers.
            case 'f':
            case 'd':
                lua_pushnumber(_lua, va_arg(list, double));
                break;
            // Strings.
            case 's':
                lua_pushstring(_lua, va_arg(list, char*));
                break;
            // Pointers.
            case 'p':
                lua_pushlightuserdata(_lua, va_arg(list, void*));
                break;
            // Object references/pointers (Lua userdata).
            case '<':
            {
                std::string type = sig;
                type = type.substr(0, type.find(">"));

                // Skip past the closing '>' (the semi-colon here is intentional-do not remove).
                while (*sig++ != '>');

                void* ptr = va_arg(list, void*);
                if (ptr == NULL)
                {
                    lua_pushnil(_lua);
                }
                else
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(_lua, sizeof(ScriptController::LuaObject));
                    object->instance = ptr;
                    object->owns = false;
                    luaL_getmetatable(_lua, type.c_str());
                    lua_setmetatable(_lua, -2);
                }
                break;
            }
            default:
                GP_ERROR("Invalid argument type '%d'.", *(sig - 1));
            }

            argumentCount++;
            luaL_checkstack(_lua, 1, "Too many arguments.");
        }
    }

    // Perform the function call.
    if (lua_pcall(_lua, argumentCount, resultCount, 0) != 0)
        GP_ERROR("Failed to call function '%s' with error '%s'.", func, lua_tostring(_lua, -1));
}

void ScriptController::registerCallback(ScriptCallback callback, std::string function)
{
    SAFE_DELETE(_callbacks[callback]);
    _callbacks[callback] = new std::string(function);
}

bool ScriptController::luaCheckBool(lua_State* state, int n)
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
