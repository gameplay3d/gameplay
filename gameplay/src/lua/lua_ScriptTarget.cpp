#include "Base.h"
#include "ScriptController.h"
#include "lua_ScriptTarget.h"
#include "Base.h"
#include "ScriptController.h"
#include "ScriptTarget.h"

namespace gameplay
{

void luaRegister_ScriptTarget()
{
    const luaL_Reg lua_members[] = 
    {
        {"addScript", lua_ScriptTarget_addScript},
        {"clearScripts", lua_ScriptTarget_clearScripts},
        {"removeScript", lua_ScriptTarget_removeScript},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("ScriptTarget", lua_members, NULL, NULL, lua_statics, scopePath);
}

static ScriptTarget* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptTarget");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptTarget' expected.");
    return (ScriptTarget*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_ScriptTarget_addScript(lua_State* state)
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

                ScriptTarget* instance = getInstance(state);
                int result = instance->addScript(param1);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_ScriptTarget_addScript - Failed to match the given parameters to a valid function signature.");
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

int lua_ScriptTarget_clearScripts(lua_State* state)
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
                ScriptTarget* instance = getInstance(state);
                instance->clearScripts();
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptTarget_clearScripts - Failed to match the given parameters to a valid function signature.");
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

int lua_ScriptTarget_removeScript(lua_State* state)
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

                ScriptTarget* instance = getInstance(state);
                bool result = instance->removeScript(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_ScriptTarget_removeScript - Failed to match the given parameters to a valid function signature.");
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

}
