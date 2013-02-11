#include "Base.h"
#include "ScriptController.h"
#include "lua_FlowLayout.h"
#include "Base.h"
#include "Container.h"
#include "Control.h"
#include "FlowLayout.h"
#include "Game.h"
#include "Layout.h"
#include "Ref.h"
#include "lua_LayoutType.h"

namespace gameplay
{

void luaRegister_FlowLayout()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_FlowLayout_addRef},
        {"getRefCount", lua_FlowLayout_getRefCount},
        {"getType", lua_FlowLayout_getType},
        {"release", lua_FlowLayout_release},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("FlowLayout", lua_members, NULL, lua_FlowLayout__gc, lua_statics, scopePath);
}

static FlowLayout* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "FlowLayout");
    luaL_argcheck(state, userdata != NULL, 1, "'FlowLayout' expected.");
    return (FlowLayout*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_FlowLayout__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "FlowLayout");
                luaL_argcheck(state, userdata != NULL, 1, "'FlowLayout' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    FlowLayout* instance = (FlowLayout*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_FlowLayout__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_FlowLayout_addRef(lua_State* state)
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
                FlowLayout* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_FlowLayout_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_FlowLayout_getRefCount(lua_State* state)
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
                FlowLayout* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_FlowLayout_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_FlowLayout_getType(lua_State* state)
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
                FlowLayout* instance = getInstance(state);
                Layout::Type result = instance->getType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_LayoutType(result));

                return 1;
            }

            lua_pushstring(state, "lua_FlowLayout_getType - Failed to match the given parameters to a valid function signature.");
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

int lua_FlowLayout_release(lua_State* state)
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
                FlowLayout* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_FlowLayout_release - Failed to match the given parameters to a valid function signature.");
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
