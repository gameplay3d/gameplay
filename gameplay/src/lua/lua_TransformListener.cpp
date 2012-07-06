#include "Base.h"
#include "ScriptController.h"
#include "Transform.h"
#include "lua_TransformListener.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_TransformListener()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"transformChanged", lua_TransformListener_transformChanged},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Transform");

    sc->registerClass("TransformListener", lua_members, NULL, lua_TransformListener__gc, lua_statics, scopePath);
}

static Transform::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "TransformListener");
    luaL_argcheck(state, userdata != NULL, 1, "'TransformListener' expected.");
    return (Transform::Listener*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_TransformListener__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "TransformListener");
                luaL_argcheck(state, userdata != NULL, 1, "'TransformListener' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Transform::Listener* instance = (Transform::Listener*)object->instance;
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

int lua_TransformListener_transformChanged(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Transform* param1 = ScriptController::getInstance()->getObjectPointer<Transform>(2, "Transform", false);

                // Get parameter 2 off the stack.
                long param2 = (long)luaL_checklong(state, 3);

                Transform::Listener* instance = getInstance(state);
                instance->transformChanged(param1, param2);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
