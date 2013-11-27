#include "Base.h"
#include "ScriptController.h"
#include "lua_SceneRenderer.h"
#include "Base.h"
#include "SceneRenderer.h"

namespace gameplay
{

void luaRegister_SceneRenderer()
{
    const luaL_Reg lua_members[] = 
    {
        {"render", lua_SceneRenderer_render},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("SceneRenderer", lua_members, NULL, lua_SceneRenderer__gc, lua_statics, scopePath);
}

static SceneRenderer* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SceneRenderer");
    luaL_argcheck(state, userdata != NULL, 1, "'SceneRenderer' expected.");
    return (SceneRenderer*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_SceneRenderer__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "SceneRenderer");
                luaL_argcheck(state, userdata != NULL, 1, "'SceneRenderer' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    SceneRenderer* instance = (SceneRenderer*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_SceneRenderer__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_SceneRenderer_render(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<VisibleSet> param1 = gameplay::ScriptUtil::getObjectPointer<VisibleSet>(2, "VisibleSet", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'VisibleSet'.");
                    lua_error(state);
                }

                SceneRenderer* instance = getInstance(state);
                unsigned int result = instance->render(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SceneRenderer_render - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<VisibleSet> param1 = gameplay::ScriptUtil::getObjectPointer<VisibleSet>(2, "VisibleSet", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'VisibleSet'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 3);

                SceneRenderer* instance = getInstance(state);
                unsigned int result = instance->render(param1, param2);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SceneRenderer_render - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
