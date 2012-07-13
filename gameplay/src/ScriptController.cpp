#include "Base.h"
#include "FileSystem.h"
#include "ScriptController.h"
#include "lua/lua_all_bindings.h"

#define GENERATE_LUA_GET_POINTER(type, checkFunc) \
    /* Check that the parameter is the correct type. */ \
    if (!lua_istable(ScriptController::__instance->_lua, index)) \
    { \
        if (lua_islightuserdata(ScriptController::__instance->_lua, index)) \
            return (type*)lua_touserdata(ScriptController::__instance->_lua, index); \
        lua_pushfstring(ScriptController::__instance->_lua, "Expected a " #type " pointer (an array represented as a Lua table), got '%s' instead.", \
            luaL_typename(ScriptController::__instance->_lua, index)); \
        lua_error(ScriptController::__instance->_lua); \
        return NULL; \
    } \
    \
    /* Get the size of the array. */ \
    lua_len(ScriptController::__instance->_lua, index); \
    int size = luaL_checkint(ScriptController::__instance->_lua, -1); \
    if (size <= 0) \
        return NULL; \
    \
    /* Create an array to store the values. */ \
    type* values = (type*)malloc(sizeof(type)*size); \
    \
    /* Push the first key. */ \
    lua_pushnil(ScriptController::__instance->_lua); \
    int i = 0; \
    for (; lua_next(ScriptController::__instance->_lua, index) != 0 && i < size; i++) \
    { \
        values[i] = (checkFunc(ScriptController::__instance->_lua, -1)); \
        \
        /* Remove the value we just retrieved, but leave the key for the next iteration. */ \
        lua_pop(ScriptController::__instance->_lua, 1); \
    } \
    \
    return values

namespace gameplay
{

void ScriptUtil::registerLibrary(const char* name, const luaL_Reg* functions)
{
    lua_newtable(ScriptController::__instance->_lua);

    // Go through the list of functions and add them to the table.
    const luaL_Reg* iter = functions;
    for (; iter && iter->name; iter++)
    {
        lua_pushcfunction(ScriptController::__instance->_lua, iter->func);
        lua_setfield(ScriptController::__instance->_lua, -2, iter->name);
    }

    lua_setglobal(ScriptController::__instance->_lua, name);
}

void ScriptUtil::registerConstantBool(std::string name, bool value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(ScriptController::__instance->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(ScriptController::__instance->_lua, scopePath[i].c_str());
            lua_gettable(ScriptController::__instance->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushboolean(ScriptController::__instance->_lua, value);
        lua_setfield(ScriptController::__instance->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(ScriptController::__instance->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushboolean(ScriptController::__instance->_lua, value);
        lua_pushvalue(ScriptController::__instance->_lua, -1);
        lua_setglobal(ScriptController::__instance->_lua, name.c_str());
    }
}

void ScriptUtil::registerConstantNumber(std::string name, double value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(ScriptController::__instance->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(ScriptController::__instance->_lua, scopePath[i].c_str());
            lua_gettable(ScriptController::__instance->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushnumber(ScriptController::__instance->_lua, value);
        lua_setfield(ScriptController::__instance->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(ScriptController::__instance->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushnumber(ScriptController::__instance->_lua, value);
        lua_pushvalue(ScriptController::__instance->_lua, -1);
        lua_setglobal(ScriptController::__instance->_lua, name.c_str());
    }
}

void ScriptUtil::registerConstantString(std::string name, std::string value, std::vector<std::string> scopePath)
{
    // If the constant is within a scope, get the correct parent 
    // table on the stack before setting its value.
    if (scopePath.size() > 0)
    {
        lua_getglobal(ScriptController::__instance->_lua, scopePath[0].c_str());
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(ScriptController::__instance->_lua, scopePath[i].c_str());
            lua_gettable(ScriptController::__instance->_lua, -2);
        }
        
        // Add the constant to the parent table.
        lua_pushstring(ScriptController::__instance->_lua, value.c_str());
        lua_setfield(ScriptController::__instance->_lua, -2, name.c_str());

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(ScriptController::__instance->_lua, size);
    }
    else
    {
        // TODO: Currently unsupported (we don't parse for this yet).
        // If the constant is global, add it to the global table.
        lua_pushstring(ScriptController::__instance->_lua, value.c_str());
        lua_pushvalue(ScriptController::__instance->_lua, -1);
        lua_setglobal(ScriptController::__instance->_lua, name.c_str());
    }
}

void ScriptUtil::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, 
    lua_CFunction deleteFunction, const luaL_Reg* statics,  std::vector<std::string> scopePath)
{
    // If the type is an inner type, get the correct parent 
    // table on the stack before creating the table for the class.
    if (scopePath.size() > 0)
    {
        std::string tablename = name;

        // Strip off the scope path part of the name.
        lua_getglobal(ScriptController::__instance->_lua, scopePath[0].c_str());
        std::size_t index = tablename.find(scopePath[0]);
        if (index != tablename.npos)
            tablename = tablename.substr(index + scopePath[0].size());
        
        for (unsigned int i = 1; i < scopePath.size(); i++)
        {
            lua_pushstring(ScriptController::__instance->_lua, scopePath[i].c_str());
            lua_gettable(ScriptController::__instance->_lua, -2);

            index = tablename.find(scopePath[i]);
            if (index != tablename.npos)
                tablename = tablename.substr(index + scopePath[i].size());
        }

        lua_pushstring(ScriptController::__instance->_lua, tablename.c_str());
        lua_newtable(ScriptController::__instance->_lua);
    }
    else
    {
        // If the type is not an inner type, set it as a global table.
        lua_newtable(ScriptController::__instance->_lua);
        lua_pushvalue(ScriptController::__instance->_lua, -1);
        lua_setglobal(ScriptController::__instance->_lua, name);
    }
    
    // Create the metatable and populate it with the member functions.
    lua_pushliteral(ScriptController::__instance->_lua, "__metatable");
    luaL_newmetatable(ScriptController::__instance->_lua, name);
    if (members)
        luaL_setfuncs(ScriptController::__instance->_lua, members, 0);
    lua_pushstring(ScriptController::__instance->_lua, "__index");
    lua_pushvalue(ScriptController::__instance->_lua, -2);
    lua_settable(ScriptController::__instance->_lua, -3);

    // Add the delete function if it was specified.
    if (deleteFunction)
    {
        lua_pushstring(ScriptController::__instance->_lua, "__gc");
        lua_pushcfunction(ScriptController::__instance->_lua, deleteFunction);
        lua_settable(ScriptController::__instance->_lua, -3);
    }

    // Set the metatable on the main table.
    lua_settable(ScriptController::__instance->_lua, -3);
    
    // Populate the main table with the static functions.
    if (statics)
        luaL_setfuncs(ScriptController::__instance->_lua, statics, 0);

    // Set the new function(s) for the class.
    if (newFunction)
    {
        lua_pushliteral(ScriptController::__instance->_lua, "new");
        lua_pushcfunction(ScriptController::__instance->_lua, newFunction);
        lua_settable(ScriptController::__instance->_lua, -3);
    }

    // Set the table we just created within the correct parent table.
    if (scopePath.size() > 0)
    {
        lua_settable(ScriptController::__instance->_lua, -3);

        // Pop all the parent tables off the stack.
        int size = scopePath.size();
        lua_pop(ScriptController::__instance->_lua, size);
    }
    else
    {
        // Pop the main table off the stack.
        lua_pop(ScriptController::__instance->_lua, 1);
    }
}

void ScriptUtil::registerFunction(const char* luaFunction, lua_CFunction cppFunction)
{
    lua_pushcfunction(ScriptController::__instance->_lua, cppFunction);
    lua_setglobal(ScriptController::__instance->_lua, luaFunction);
}

void ScriptUtil::setGlobalHierarchy(std::map<std::string, std::vector<std::string> > hierarchy)
{
    ScriptController::__instance->_hierarchy = hierarchy;
}

bool* ScriptUtil::getBoolPointer(int index)
{
    GENERATE_LUA_GET_POINTER(bool, luaCheckBool);
}

short* ScriptUtil::getShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(short, (short)luaL_checkint);
}

int* ScriptUtil::getIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(int, (int)luaL_checkint);
}

long* ScriptUtil::getLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(long, (long)luaL_checkint);
}

unsigned char* ScriptUtil::getUnsignedCharPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned char, (unsigned char)luaL_checkunsigned);
}

unsigned short* ScriptUtil::getUnsignedShortPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned short, (unsigned short)luaL_checkunsigned);
}

unsigned int* ScriptUtil::getUnsignedIntPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned int, (unsigned int)luaL_checkunsigned);
}

unsigned long* ScriptUtil::getUnsignedLongPointer(int index)
{
    GENERATE_LUA_GET_POINTER(unsigned long, (unsigned long)luaL_checkunsigned);
}

float* ScriptUtil::getFloatPointer(int index)
{
    GENERATE_LUA_GET_POINTER(float, (float)luaL_checknumber);
}

double* ScriptUtil::getDoublePointer(int index)
{
    GENERATE_LUA_GET_POINTER(double, (double)luaL_checknumber);
}

const char* ScriptUtil::getString(int index, bool isStdString)
{
    if (lua_type(ScriptController::__instance->_lua, index) == LUA_TSTRING)
        return luaL_checkstring(ScriptController::__instance->_lua, index);
    else if (lua_type(ScriptController::__instance->_lua, index) == LUA_TNIL && !isStdString)
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


ScriptController* ScriptController::__instance = NULL;

ScriptController* ScriptController::getInstance()
{
    return __instance;
}

void ScriptController::loadScript(const char* path)
{
    const char* scriptContents = FileSystem::readAll(path);
    if (luaL_dostring(_lua, scriptContents))
        GP_ERROR("Failed to run Lua script with error: '%s'.", lua_tostring(_lua, -1));

    SAFE_DELETE_ARRAY(scriptContents);
}

bool ScriptController::getBool(const char* name)
{
    lua_getglobal(_lua, name);
    return ScriptUtil::luaCheckBool(_lua, -1);
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

ScriptController::ScriptController() : _lua(NULL)
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
        executeFunction<void>(_callbacks[INITIALIZE]->c_str());
    }
}

void ScriptController::finalize()
{
    if (_lua)
        lua_close(_lua);
}

void ScriptController::finalizeGame()
{
    if (_callbacks[FINALIZE])
    {
        executeFunction<void>(_callbacks[FINALIZE]->c_str());
    }
}

void ScriptController::update(float elapsedTime)
{
    if (_callbacks[UPDATE])
    {
        executeFunction<void>(_callbacks[UPDATE]->c_str(), "f", elapsedTime);
    }
}

void ScriptController::render(float elapsedTime)
{
    if (_callbacks[RENDER])
    {
        executeFunction<void>(_callbacks[RENDER]->c_str(), "f", elapsedTime);
    }
}

void ScriptController::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (_callbacks[KEY_EVENT])
    {
        executeFunction<void>(_callbacks[KEY_EVENT]->c_str(), "[Keyboard::KeyEvent][Keyboard::Key]", evt, key);
    }
}

void ScriptController::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (_callbacks[TOUCH_EVENT])
    {
        executeFunction<void>(_callbacks[TOUCH_EVENT]->c_str(), "[Touch::TouchEvent]iiui", evt, x, y, contactIndex);
    }
}

bool ScriptController::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (_callbacks[MOUSE_EVENT])
    {
        return executeFunction<bool>(_callbacks[MOUSE_EVENT]->c_str(), "[Mouse::MouseEvent]iiii", evt, x, y, wheelDelta);
    }
    return false;
}

void ScriptController::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    if (_callbacks[GAMEPAD_EVENT])
    {
        executeFunction<void>(_callbacks[GAMEPAD_EVENT]->c_str(), "[Gamepad::GamepadEvent]<Gamepad>", evt, gamepad);
    }
}

void ScriptController::executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list)
{
    if (func == NULL)
    {
        GP_ERROR("Lua function name must be non-null.");
        return;
    }

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

                lua_pushstring(_lua, lua_stringFromEnumGlobal(type, va_arg(*list, int)));
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
                while (i != type.npos)
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


// Helper macros.
#define SCRIPT_EXECUTE_FUNCTION_NO_PARAM(type, checkfunc) \
    executeFunctionHelper(1, func, NULL, NULL); \
    type value = (type)checkfunc(_lua, -1); \
    lua_pop(_lua, -1); \
    return value;

#define SCRIPT_EXECUTE_FUNCTION_PARAM(type, checkfunc) \
    va_list list; \
    va_start(list, args); \
    executeFunctionHelper(1, func, args, &list); \
    type value = (type)checkfunc(_lua, -1); \
    lua_pop(_lua, -1); \
    va_end(list); \
    return value;

template<> void ScriptController::executeFunction<void>(const char* func)
{
    executeFunctionHelper(0, func, NULL, NULL);
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

template<> void ScriptController::executeFunction<void>(const char* func, const char* args, ...)
{
    va_list list;
    va_start(list, args);
    executeFunctionHelper(0, func, args, &list);
    va_end(list);
}

template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(bool, ScriptUtil::luaCheckBool);
}

template<> char ScriptController::executeFunction<char>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(char, luaL_checkint);
}

template<> short ScriptController::executeFunction<short>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(short, luaL_checkint);
}

template<> int ScriptController::executeFunction<int>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(int, luaL_checkint);
}

template<> long ScriptController::executeFunction<long>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(long, luaL_checklong);
}

template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned char, luaL_checkunsigned);
}

template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned short, luaL_checkunsigned);
}

template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned int, luaL_checkunsigned);
}

template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(unsigned long, luaL_checkunsigned);
}

template<> float ScriptController::executeFunction<float>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(float, luaL_checknumber);
}

template<> double ScriptController::executeFunction<double>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(double, luaL_checknumber);
}

template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, ...)
{
    SCRIPT_EXECUTE_FUNCTION_PARAM(std::string, luaL_checkstring);
}

}
