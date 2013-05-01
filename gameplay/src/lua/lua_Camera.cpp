#include "Base.h"
#include "ScriptController.h"
#include "lua_Camera.h"
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

void luaRegister_Camera()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_Camera_addRef},
        {"getAspectRatio", lua_Camera_getAspectRatio},
        {"getCameraType", lua_Camera_getCameraType},
        {"getFarPlane", lua_Camera_getFarPlane},
        {"getFieldOfView", lua_Camera_getFieldOfView},
        {"getFrustum", lua_Camera_getFrustum},
        {"getInverseViewMatrix", lua_Camera_getInverseViewMatrix},
        {"getInverseViewProjectionMatrix", lua_Camera_getInverseViewProjectionMatrix},
        {"getNearPlane", lua_Camera_getNearPlane},
        {"getNode", lua_Camera_getNode},
        {"getProjectionMatrix", lua_Camera_getProjectionMatrix},
        {"getRefCount", lua_Camera_getRefCount},
        {"getViewMatrix", lua_Camera_getViewMatrix},
        {"getViewProjectionMatrix", lua_Camera_getViewProjectionMatrix},
        {"getZoomX", lua_Camera_getZoomX},
        {"getZoomY", lua_Camera_getZoomY},
        {"pickRay", lua_Camera_pickRay},
        {"project", lua_Camera_project},
        {"release", lua_Camera_release},
        {"resetProjectionMatrix", lua_Camera_resetProjectionMatrix},
        {"setAspectRatio", lua_Camera_setAspectRatio},
        {"setFarPlane", lua_Camera_setFarPlane},
        {"setFieldOfView", lua_Camera_setFieldOfView},
        {"setNearPlane", lua_Camera_setNearPlane},
        {"setProjectionMatrix", lua_Camera_setProjectionMatrix},
        {"setZoomX", lua_Camera_setZoomX},
        {"setZoomY", lua_Camera_setZoomY},
        {"unproject", lua_Camera_unproject},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_Camera_static_create},
        {"createOrthographic", lua_Camera_static_createOrthographic},
        {"createPerspective", lua_Camera_static_createPerspective},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Camera", lua_members, NULL, lua_Camera__gc, lua_statics, scopePath);
}

static Camera* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Camera");
    luaL_argcheck(state, userdata != NULL, 1, "'Camera' expected.");
    return (Camera*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Camera__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Camera");
                luaL_argcheck(state, userdata != NULL, 1, "'Camera' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Camera* instance = (Camera*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_addRef(lua_State* state)
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
                Camera* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getAspectRatio(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getAspectRatio();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getAspectRatio - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getCameraType(lua_State* state)
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
                Camera* instance = getInstance(state);
                Camera::Type result = instance->getCameraType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_CameraType(result));

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getCameraType - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getFarPlane(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getFarPlane();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getFarPlane - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getFieldOfView(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getFieldOfView();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getFieldOfView - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getFrustum(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getFrustum());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Frustum");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getFrustum - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getInverseViewMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getInverseViewMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getInverseViewMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getInverseViewProjectionMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getInverseViewProjectionMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getInverseViewProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getNearPlane(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getNearPlane();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getNearPlane - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getNode(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)instance->getNode();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Node");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getNode - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getProjectionMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getProjectionMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getRefCount(lua_State* state)
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
                Camera* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getViewMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getViewMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getViewMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getViewProjectionMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getViewProjectionMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getViewProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getZoomX(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getZoomX();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getZoomX - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_getZoomY(lua_State* state)
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
                Camera* instance = getInstance(state);
                float result = instance->getZoomY();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Camera_getZoomY - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_pickRay(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Ray> param4 = gameplay::ScriptUtil::getObjectPointer<Ray>(5, "Ray", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Ray'.");
                    lua_error(state);
                }

                Camera* instance = getInstance(state);
                instance->pickRay(*param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_pickRay - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Camera_project(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param3 = gameplay::ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Camera* instance = getInstance(state);
                    instance->project(*param1, *param2, param3);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Camera* instance = getInstance(state);
                    instance->project(*param1, *param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Camera_project - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Camera_release(lua_State* state)
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
                Camera* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_release - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_resetProjectionMatrix(lua_State* state)
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
                Camera* instance = getInstance(state);
                instance->resetProjectionMatrix();
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_resetProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setAspectRatio(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setAspectRatio(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setAspectRatio - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setFarPlane(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setFarPlane(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setFarPlane - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setFieldOfView(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setFieldOfView(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setFieldOfView - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setNearPlane(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setNearPlane(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setNearPlane - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setProjectionMatrix(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                Camera* instance = getInstance(state);
                instance->setProjectionMatrix(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setZoomX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setZoomX(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setZoomX - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_setZoomY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Camera* instance = getInstance(state);
                instance->setZoomY(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_setZoomY - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Properties> param1 = gameplay::ScriptUtil::getObjectPointer<Properties>(1, "Properties", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Properties'.");
                    lua_error(state);
                }

                void* returnPtr = (void*)Camera::create(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Camera");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_static_create - Failed to match the given parameters to a valid function signature.");
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

int lua_Camera_static_createOrthographic(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 3);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 4);

                // Get parameter 5 off the stack.
                float param5 = (float)luaL_checknumber(state, 5);

                void* returnPtr = (void*)Camera::createOrthographic(param1, param2, param3, param4, param5);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Camera");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_static_createOrthographic - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Camera_static_createPerspective(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 3);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 4);

                void* returnPtr = (void*)Camera::createPerspective(param1, param2, param3, param4);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Camera");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Camera_static_createPerspective - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Camera_unproject(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param5 = gameplay::ScriptUtil::getObjectPointer<Vector3>(6, "Vector3", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Vector3'.");
                    lua_error(state);
                }

                Camera* instance = getInstance(state);
                instance->unproject(*param1, param2, param3, param4, param5);
                
                return 0;
            }

            lua_pushstring(state, "lua_Camera_unproject - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
