#include "Base.h"
#include "ScriptController.h"
#include "lua_Keyboard.h"
#include "Keyboard.h"

namespace gameplay
{

void luaRegister_Keyboard()
{
    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Keyboard", lua_members, NULL, lua_Keyboard__gc, lua_statics, scopePath);
}

static Keyboard* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Keyboard");
    luaL_argcheck(state, userdata != NULL, 1, "'Keyboard' expected.");
    return (Keyboard*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Keyboard__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Keyboard");
                luaL_argcheck(state, userdata != NULL, 1, "'Keyboard' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Keyboard* instance = (Keyboard*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Keyboard__gc - Failed to match the given parameters to a valid function signature.");
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
