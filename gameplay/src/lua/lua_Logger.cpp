#include "Base.h"
#include "ScriptController.h"
#include "lua_Logger.h"
#include "Logger.h"
#include "Base.h"
#include "Game.h"
#include "ScriptController.h"
#include "lua_LoggerLevel.h"

namespace gameplay
{

void luaRegister_Logger()
{
    const luaL_Reg* lua_members = NULL;
    const luaL_Reg lua_statics[] = 
    {
        {"isEnabled", lua_Logger_static_isEnabled},
        {"log", lua_Logger_static_log},
        {"set", lua_Logger_static_set},
        {"setEnabled", lua_Logger_static_setEnabled},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Logger", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Logger* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Logger");
    luaL_argcheck(state, userdata != NULL, 1, "'Logger' expected.");
    return (Logger*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Logger_static_isEnabled(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Logger::Level param1 = (Logger::Level)lua_enumFromString_LoggerLevel(luaL_checkstring(state, 1));

                bool result = Logger::isEnabled(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Logger_static_isEnabled - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Logger_static_log(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Logger::Level param1 = (Logger::Level)lua_enumFromString_LoggerLevel(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(2, false);

                Logger::log(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Logger_static_log - Failed to match the given parameters to a valid function signature.");
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

int lua_Logger_static_set(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Logger::Level param1 = (Logger::Level)lua_enumFromString_LoggerLevel(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(2, false);

                Logger::set(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Logger_static_set - Failed to match the given parameters to a valid function signature.");
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

int lua_Logger_static_setEnabled(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                Logger::Level param1 = (Logger::Level)lua_enumFromString_LoggerLevel(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Logger::setEnabled(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Logger_static_setEnabled - Failed to match the given parameters to a valid function signature.");
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

}
