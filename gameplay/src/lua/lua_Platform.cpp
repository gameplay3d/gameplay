#include "Base.h"
#include "ScriptController.h"
#include "lua_Platform.h"
#include "Platform.h"

namespace gameplay
{

void luaRegister_Platform()
{
    const luaL_Reg lua_members[] = 
    {
        {"enterMessagePump", lua_Platform_enterMessagePump},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Platform", lua_members, NULL, lua_Platform__gc, lua_statics, scopePath);
}

static Platform* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Platform");
    luaL_argcheck(state, userdata != NULL, 1, "'Platform' expected.");
    return (Platform*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Platform__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "Platform");
                luaL_argcheck(state, userdata != NULL, 1, "'Platform' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Platform* instance = (Platform*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_enterMessagePump(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Platform* instance = getInstance(state);
                int result = instance->enterMessagePump();

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_enterMessagePump - Failed to match the given parameters to a valid function signature.");
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

}
