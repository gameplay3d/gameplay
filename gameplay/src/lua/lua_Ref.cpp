#include "Base.h"
#include "ScriptController.h"
#include "lua_Ref.h"
#include "Base.h"
#include "Game.h"
#include "Ref.h"

namespace gameplay
{

void luaRegister_Ref()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_Ref_addRef},
        {"getRefCount", lua_Ref_getRefCount},
        {"release", lua_Ref_release},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Ref", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Ref* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Ref");
    luaL_argcheck(state, userdata != NULL, 1, "'Ref' expected.");
    return (Ref*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Ref_addRef(lua_State* state)
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
                Ref* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_Ref_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_Ref_getRefCount(lua_State* state)
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
                Ref* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Ref_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Ref_release(lua_State* state)
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
                Ref* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_Ref_release - Failed to match the given parameters to a valid function signature.");
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
