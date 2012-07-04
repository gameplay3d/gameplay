#include "Base.h"
#include "ScriptController.h"
#include "MathUtil.h"
#include "lua_MathUtil.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_MathUtil()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("MathUtil", lua_members, NULL, lua_MathUtil__gc, lua_statics, scopePath);
}

static MathUtil* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MathUtil");
    luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
    return (MathUtil*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_MathUtil__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "MathUtil");
                luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    MathUtil* instance = (MathUtil*)object->instance;
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
