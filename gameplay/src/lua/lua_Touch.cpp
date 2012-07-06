#include "Base.h"
#include "ScriptController.h"
#include "Touch.h"
#include "lua_Touch.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Touch()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("Touch", lua_members, NULL, lua_Touch__gc, lua_statics, scopePath);
}

static Touch* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Touch");
    luaL_argcheck(state, userdata != NULL, 1, "'Touch' expected.");
    return (Touch*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Touch__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Touch");
                luaL_argcheck(state, userdata != NULL, 1, "'Touch' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Touch* instance = (Touch*)object->instance;
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
