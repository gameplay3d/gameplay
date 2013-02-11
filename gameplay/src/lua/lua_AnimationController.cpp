#include "Base.h"
#include "ScriptController.h"
#include "lua_AnimationController.h"
#include "AnimationController.h"
#include "Base.h"
#include "Curve.h"
#include "Game.h"

namespace gameplay
{

void luaRegister_AnimationController()
{
    const luaL_Reg lua_members[] = 
    {
        {"stopAllAnimations", lua_AnimationController_stopAllAnimations},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("AnimationController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static AnimationController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "AnimationController");
    luaL_argcheck(state, userdata != NULL, 1, "'AnimationController' expected.");
    return (AnimationController*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_AnimationController_stopAllAnimations(lua_State* state)
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
                AnimationController* instance = getInstance(state);
                instance->stopAllAnimations();
                
                return 0;
            }

            lua_pushstring(state, "lua_AnimationController_stopAllAnimations - Failed to match the given parameters to a valid function signature.");
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
