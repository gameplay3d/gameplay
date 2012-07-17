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
        {"addCallback", lua_ScriptTarget_addCallback},
        {"removeCallback", lua_ScriptTarget_removeCallback},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    ScriptUtil::registerClass("ScriptTarget", lua_members, NULL, lua_ScriptTarget__gc, lua_statics, scopePath);
}

static ScriptTarget* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptTarget");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptTarget' expected.");
    return (ScriptTarget*)((ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_ScriptTarget__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "ScriptTarget");
                luaL_argcheck(state, userdata != NULL, 1, "'ScriptTarget' expected.");
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    ScriptTarget* instance = (ScriptTarget*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptTarget__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_ScriptTarget_addCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                std::string param1 = ScriptUtil::getString(2, true);

                // Get parameter 2 off the stack.
                std::string param2 = ScriptUtil::getString(3, true);

                ScriptTarget* instance = getInstance(state);
                instance->addCallback(param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptTarget_addCallback - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_ScriptTarget_removeCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                std::string param1 = ScriptUtil::getString(2, true);

                // Get parameter 2 off the stack.
                std::string param2 = ScriptUtil::getString(3, true);

                ScriptTarget* instance = getInstance(state);
                instance->removeCallback(param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptTarget_removeCallback - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
