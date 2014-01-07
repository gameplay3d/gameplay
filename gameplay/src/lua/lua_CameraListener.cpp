#include "Base.h"
#include "ScriptController.h"
#include "lua_CameraListener.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "Camera.h"
#include "Game.h"
#include "Node.h"
#include "PhysicsController.h"
#include "Ref.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Transform.h"
#include "lua_CameraType.h"
#include "lua_CurveInterpolationType.h"

namespace gameplay
{

void luaRegister_CameraListener()
{
    const luaL_Reg lua_members[] = 
    {
        {"cameraChanged", lua_CameraListener_cameraChanged},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Camera");

    gameplay::ScriptUtil::registerClass("CameraListener", lua_members, NULL, lua_CameraListener__gc, lua_statics, scopePath);
}

static Camera::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "CameraListener");
    luaL_argcheck(state, userdata != NULL, 1, "'CameraListener' expected.");
    return (Camera::Listener*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_CameraListener__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "CameraListener");
                luaL_argcheck(state, userdata != NULL, 1, "'CameraListener' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Camera::Listener* instance = (Camera::Listener*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_CameraListener__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_CameraListener_cameraChanged(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Camera> param1 = gameplay::ScriptUtil::getObjectPointer<Camera>(2, "Camera", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Camera'.");
                    lua_error(state);
                }

                Camera::Listener* instance = getInstance(state);
                instance->cameraChanged(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_CameraListener_cameraChanged - Failed to match the given parameters to a valid function signature.");
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
