#include "Base.h"
#include "ScriptController.h"
#include "lua_AIController.h"
#include "AIController.h"
#include "Base.h"
#include "Game.h"

namespace gameplay
{

void luaRegister_AIController()
{
    const luaL_Reg lua_members[] = 
    {
        {"findAgent", lua_AIController_findAgent},
        {"sendMessage", lua_AIController_sendMessage},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("AIController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static AIController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "AIController");
    luaL_argcheck(state, userdata != NULL, 1, "'AIController' expected.");
    return (AIController*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_AIController_findAgent(lua_State* state)
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

                AIController* instance = getInstance(state);
                void* returnPtr = (void*)instance->findAgent(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "AIAgent");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_AIController_findAgent - Failed to match the given parameters to a valid function signature.");
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

int lua_AIController_sendMessage(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<AIMessage> param1 = gameplay::ScriptUtil::getObjectPointer<AIMessage>(2, "AIMessage", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'AIMessage'.");
                    lua_error(state);
                }

                AIController* instance = getInstance(state);
                instance->sendMessage(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_AIController_sendMessage - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<AIMessage> param1 = gameplay::ScriptUtil::getObjectPointer<AIMessage>(2, "AIMessage", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'AIMessage'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                AIController* instance = getInstance(state);
                instance->sendMessage(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_AIController_sendMessage - Failed to match the given parameters to a valid function signature.");
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

}
