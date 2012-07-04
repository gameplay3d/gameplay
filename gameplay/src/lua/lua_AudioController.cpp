#include "Base.h"
#include "ScriptController.h"
#include "AudioController.h"
#include "lua_AudioController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_AudioController()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("AudioController", lua_members, NULL, lua_AudioController__gc, lua_statics, scopePath);
}

static AudioController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "AudioController");
    luaL_argcheck(state, userdata != NULL, 1, "'AudioController' expected.");
    return (AudioController*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_AudioController__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
            {
                void* userdata = luaL_checkudata(state, 1, "AudioController");
                luaL_argcheck(state, userdata != NULL, 1, "'AudioController' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    AudioController* instance = (AudioController*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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
