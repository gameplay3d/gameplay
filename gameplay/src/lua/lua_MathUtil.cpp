#include "Base.h"
#include "ScriptController.h"
#include "lua_MathUtil.h"
#include "MathUtil.h"

namespace gameplay
{

void luaRegister_MathUtil()
{
    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    ScriptUtil::registerClass("MathUtil", lua_members, NULL, lua_MathUtil__gc, lua_statics, scopePath);
}

static MathUtil* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MathUtil");
    luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
    return (MathUtil*)((ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "MathUtil");
                luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    MathUtil* instance = (MathUtil*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_MathUtil__gc - Failed to match the given parameters to a valid function signature.");
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
