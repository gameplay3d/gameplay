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
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    ScriptUtil::registerClass("ScriptController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static ScriptController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptController");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptController' expected.");
    return (ScriptController*)((ScriptUtil::LuaObject*)userdata)->instance;
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
                const char* param1 = ScriptUtil::getString(2, false);

                ScriptController* instance = getInstance(state);
                instance->loadScript(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_loadScript - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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
