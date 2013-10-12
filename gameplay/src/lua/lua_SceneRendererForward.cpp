#include "Base.h"
#include "ScriptController.h"
#include "lua_SceneRendererForward.h"
#include "Base.h"
#include "Form.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "SceneRenderer.h"
#include "SceneRendererForward.h"
#include "Terrain.h"

namespace gameplay
{

void luaRegister_SceneRendererForward()
{
    const luaL_Reg lua_members[] = 
    {
        {"isWireframe", lua_SceneRendererForward_isWireframe},
        {"render", lua_SceneRendererForward_render},
        {"setWireframe", lua_SceneRendererForward_setWireframe},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_SceneRendererForward_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("SceneRendererForward", lua_members, NULL, NULL, lua_statics, scopePath);
}

static SceneRendererForward* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SceneRendererForward");
    luaL_argcheck(state, userdata != NULL, 1, "'SceneRendererForward' expected.");
    return (SceneRendererForward*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_SceneRendererForward_isWireframe(lua_State* state)
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
                SceneRendererForward* instance = getInstance(state);
                bool result = instance->isWireframe();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SceneRendererForward_isWireframe - Failed to match the given parameters to a valid function signature.");
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

int lua_SceneRendererForward_render(lua_State* state)
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

                SceneRendererForward* instance = getInstance(state);
                unsigned int result = instance->render(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SceneRendererForward_render - Failed to match the given parameters to a valid function signature.");
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

int lua_SceneRendererForward_setWireframe(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                SceneRendererForward* instance = getInstance(state);
                instance->setWireframe(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SceneRendererForward_setWireframe - Failed to match the given parameters to a valid function signature.");
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

int lua_SceneRendererForward_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)SceneRendererForward::create();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "SceneRendererForward");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
