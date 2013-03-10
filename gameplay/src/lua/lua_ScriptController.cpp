#include "Base.h"
#include "ScriptController.h"
#include "lua_ScriptController.h"
#include "Base.h"
#include "FileSystem.h"
#include "ScriptController.h"

namespace gameplay
{

void luaRegister_ScriptController()
{
    const luaL_Reg lua_members[] = 
    {
        {"loadScript", lua_ScriptController_loadScript},
        {"loadUrl", lua_ScriptController_loadUrl},
        {"registerCallback", lua_ScriptController_registerCallback},
        {"unregisterCallback", lua_ScriptController_unregisterCallback},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"print", lua_ScriptController_static_print},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("ScriptController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static ScriptController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptController");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptController' expected.");
    return (ScriptController*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_ScriptController_loadScript(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                ScriptController* instance = getInstance(state);
                instance->loadScript(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptController_loadScript - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 3);

                ScriptController* instance = getInstance(state);
                instance->loadScript(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptController_loadScript - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_ScriptController_loadUrl(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                ScriptController* instance = getInstance(state);
                std::string result = instance->loadUrl(param1);

                // Push the return value onto the stack.
                lua_pushstring(state, result.c_str());

                return 1;
            }

            lua_pushstring(state, "lua_ScriptController_loadUrl - Failed to match the given parameters to a valid function signature.");
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

int lua_ScriptController_registerCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(3, false);

                ScriptController* instance = getInstance(state);
                instance->registerCallback(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptController_registerCallback - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_ScriptController_static_print(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    ScriptController::print(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_ScriptController_static_print - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    // Get parameter 2 off the stack.
                    const char* param2 = gameplay::ScriptUtil::getString(2, false);

                    ScriptController::print(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_ScriptController_static_print - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_ScriptController_unregisterCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(3, false);

                ScriptController* instance = getInstance(state);
                instance->unregisterCallback(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptController_unregisterCallback - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
