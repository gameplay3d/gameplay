#include "Base.h"
#include "ScriptController.h"
#include "Transform.h"
#include "lua_Transform.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Transform()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"addListener", lua_Transform_addListener},
        {"createAnimation", lua_Transform_createAnimation},
        {"createAnimationFromBy", lua_Transform_createAnimationFromBy},
        {"createAnimationFromTo", lua_Transform_createAnimationFromTo},
        {"destroyAnimation", lua_Transform_destroyAnimation},
        {"getAnimation", lua_Transform_getAnimation},
        {"getAnimationPropertyComponentCount", lua_Transform_getAnimationPropertyComponentCount},
        {"getAnimationPropertyValue", lua_Transform_getAnimationPropertyValue},
        {"getBackVector", lua_Transform_getBackVector},
        {"getDownVector", lua_Transform_getDownVector},
        {"getForwardVector", lua_Transform_getForwardVector},
        {"getLeftVector", lua_Transform_getLeftVector},
        {"getMatrix", lua_Transform_getMatrix},
        {"getRightVector", lua_Transform_getRightVector},
        {"getRotation", lua_Transform_getRotation},
        {"getScale", lua_Transform_getScale},
        {"getScaleX", lua_Transform_getScaleX},
        {"getScaleY", lua_Transform_getScaleY},
        {"getScaleZ", lua_Transform_getScaleZ},
        {"getTranslation", lua_Transform_getTranslation},
        {"getTranslationX", lua_Transform_getTranslationX},
        {"getTranslationY", lua_Transform_getTranslationY},
        {"getTranslationZ", lua_Transform_getTranslationZ},
        {"getUpVector", lua_Transform_getUpVector},
        {"removeListener", lua_Transform_removeListener},
        {"rotate", lua_Transform_rotate},
        {"rotateX", lua_Transform_rotateX},
        {"rotateY", lua_Transform_rotateY},
        {"rotateZ", lua_Transform_rotateZ},
        {"scale", lua_Transform_scale},
        {"scaleX", lua_Transform_scaleX},
        {"scaleY", lua_Transform_scaleY},
        {"scaleZ", lua_Transform_scaleZ},
        {"set", lua_Transform_set},
        {"setAnimationPropertyValue", lua_Transform_setAnimationPropertyValue},
        {"setIdentity", lua_Transform_setIdentity},
        {"setRotation", lua_Transform_setRotation},
        {"setScale", lua_Transform_setScale},
        {"setScaleX", lua_Transform_setScaleX},
        {"setScaleY", lua_Transform_setScaleY},
        {"setScaleZ", lua_Transform_setScaleZ},
        {"setTranslation", lua_Transform_setTranslation},
        {"setTranslationX", lua_Transform_setTranslationX},
        {"setTranslationY", lua_Transform_setTranslationY},
        {"setTranslationZ", lua_Transform_setTranslationZ},
        {"transformPoint", lua_Transform_transformPoint},
        {"transformVector", lua_Transform_transformVector},
        {"translate", lua_Transform_translate},
        {"translateForward", lua_Transform_translateForward},
        {"translateLeft", lua_Transform_translateLeft},
        {"translateUp", lua_Transform_translateUp},
        {"translateX", lua_Transform_translateX},
        {"translateY", lua_Transform_translateY},
        {"translateZ", lua_Transform_translateZ},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"ANIMATE_ROTATE", lua_Transform_static_ANIMATE_ROTATE},
        {"ANIMATE_ROTATE_TRANSLATE", lua_Transform_static_ANIMATE_ROTATE_TRANSLATE},
        {"ANIMATE_SCALE", lua_Transform_static_ANIMATE_SCALE},
        {"ANIMATE_SCALE_ROTATE_TRANSLATE", lua_Transform_static_ANIMATE_SCALE_ROTATE_TRANSLATE},
        {"ANIMATE_SCALE_UNIT", lua_Transform_static_ANIMATE_SCALE_UNIT},
        {"ANIMATE_SCALE_X", lua_Transform_static_ANIMATE_SCALE_X},
        {"ANIMATE_SCALE_Y", lua_Transform_static_ANIMATE_SCALE_Y},
        {"ANIMATE_SCALE_Z", lua_Transform_static_ANIMATE_SCALE_Z},
        {"ANIMATE_TRANSLATE", lua_Transform_static_ANIMATE_TRANSLATE},
        {"ANIMATE_TRANSLATE_X", lua_Transform_static_ANIMATE_TRANSLATE_X},
        {"ANIMATE_TRANSLATE_Y", lua_Transform_static_ANIMATE_TRANSLATE_Y},
        {"ANIMATE_TRANSLATE_Z", lua_Transform_static_ANIMATE_TRANSLATE_Z},
        {"isTransformChangedSuspended", lua_Transform_static_isTransformChangedSuspended},
        {"resumeTransformChanged", lua_Transform_static_resumeTransformChanged},
        {"suspendTransformChanged", lua_Transform_static_suspendTransformChanged},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("Transform", lua_members, lua_Transform__init, lua_Transform__gc, lua_statics, scopePath);
}

static Transform* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Transform");
    luaL_argcheck(state, userdata != NULL, 1, "'Transform' expected.");
    return (Transform*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Transform__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                void* userdata = luaL_checkudata(state, 1, "Transform");
                luaL_argcheck(state, userdata != NULL, 1, "'Transform' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Transform* instance = (Transform*)object->instance;
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

int lua_Transform__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new Transform();
            object->owns = true;
            luaL_getmetatable(state, "Transform");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Transform");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Transform' for parameter 1.");
                    lua_error(state);
                }
                Transform* param1 = (Transform*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Transform(*param1);
                object->owns = true;
                luaL_getmetatable(state, "Transform");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Vector3");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 1.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Transform(*param1, *param2, *param3);
                object->owns = true;
                luaL_getmetatable(state, "Transform");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Vector3");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 1.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Matrix");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 2.");
                    lua_error(state);
                }
                Matrix* param2 = (Matrix*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Transform(*param1, *param2, *param3);
                object->owns = true;
                luaL_getmetatable(state, "Transform");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_addListener(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "TransformListener");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Transform::Listener' for parameter 2.");
                    lua_error(state);
                }
                Transform::Listener* param1 = (Transform::Listener*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->addListener(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "TransformListener");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Transform::Listener' for parameter 2.");
                    lua_error(state);
                }
                Transform::Listener* param1 = (Transform::Listener*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                long param2 = (long)luaL_checklong(state, 3);

                Transform* instance = getInstance(state);
                instance->addListener(param1, param2);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_createAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 3);

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimation(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Properties");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Properties' for parameter 3.");
                    lua_error(state);
                }
                Properties* param2 = (Properties*)((ScriptController::LuaObject*)userdata3)->instance;

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimation(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 7) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                unsigned long* param4 = ScriptController::getInstance()->getUnsignedLongPointer(5);

                // Get parameter 5 off the stack.
                float* param5 = ScriptController::getInstance()->getFloatPointer(6);

                // Get parameter 6 off the stack.
                Curve::InterpolationType param6 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 7));

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 9:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 7) == LUA_TTABLE || lua_type(state, 7) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 8) == LUA_TTABLE || lua_type(state, 8) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 9) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                unsigned long* param4 = ScriptController::getInstance()->getUnsignedLongPointer(5);

                // Get parameter 5 off the stack.
                float* param5 = ScriptController::getInstance()->getFloatPointer(6);

                // Get parameter 6 off the stack.
                float* param6 = ScriptController::getInstance()->getFloatPointer(7);

                // Get parameter 7 off the stack.
                float* param7 = ScriptController::getInstance()->getFloatPointer(8);

                // Get parameter 8 off the stack.
                Curve::InterpolationType param8 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 9));

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6, param7, param8);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 7 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_createAnimationFromBy(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                float* param3 = ScriptController::getInstance()->getFloatPointer(4);

                // Get parameter 4 off the stack.
                float* param4 = ScriptController::getInstance()->getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimationFromBy(param1, param2, param3, param4, param5, param6);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_createAnimationFromTo(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                float* param3 = ScriptController::getInstance()->getFloatPointer(4);

                // Get parameter 4 off the stack.
                float* param4 = ScriptController::getInstance()->getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createAnimationFromTo(param1, param2, param3, param4, param5, param6);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_destroyAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                instance->destroyAnimation();
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                Transform* instance = getInstance(state);
                instance->destroyAnimation(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getAnimation();
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getAnimation(param1);
                object->owns = false;
                luaL_getmetatable(state, "Animation");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getAnimationPropertyComponentCount(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                Transform* instance = getInstance(state);
                unsigned int result = instance->getAnimationPropertyComponentCount(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "AnimationValue");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'AnimationValue' for parameter 3.");
                    lua_error(state);
                }
                AnimationValue* param2 = (AnimationValue*)((ScriptController::LuaObject*)userdata3)->instance;

                Transform* instance = getInstance(state);
                instance->getAnimationPropertyValue(param1, param2);
                
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

int lua_Transform_getBackVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getBackVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getBackVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getDownVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getDownVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getDownVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getForwardVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getForwardVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getForwardVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getLeftVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getLeftVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getLeftVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getMatrix(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getMatrix());
                object->owns = false;
                luaL_getmetatable(state, "Matrix");
                lua_setmetatable(state, -2);

                return 1;
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

int lua_Transform_getRightVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getRightVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getRightVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getRotation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getRotation());
                object->owns = false;
                luaL_getmetatable(state, "Quaternion");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getRotation(param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Matrix");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 2.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getRotation(param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                float result = instance->getRotation(param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getScale(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getScale());
                object->owns = false;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getScale(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getScaleX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getScaleX();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getScaleY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getScaleY();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getScaleZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getScaleZ();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getTranslation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getTranslation());
                object->owns = false;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getTranslation(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_getTranslationX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getTranslationX();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getTranslationY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getTranslationY();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getTranslationZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                float result = instance->getTranslationZ();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
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

int lua_Transform_getUpVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getUpVector());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->getUpVector(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_removeListener(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "TransformListener");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Transform::Listener' for parameter 2.");
                    lua_error(state);
                }
                Transform::Listener* param1 = (Transform::Listener*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->removeListener(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_rotate(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->rotate(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Matrix");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 2.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->rotate(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Transform* instance = getInstance(state);
                instance->rotate(*param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                Transform* instance = getInstance(state);
                instance->rotate(param1, param2, param3, param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_rotateX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->rotateX(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_rotateY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->rotateY(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_rotateZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->rotateZ(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_scale(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->scale(param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->scale(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Transform* instance = getInstance(state);
                instance->scale(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_scaleX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->scaleX(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_scaleY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->scaleY(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_scaleZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->scaleZ(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_set(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Transform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Transform' for parameter 2.");
                    lua_error(state);
                }
                Transform* param1 = (Transform*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                Transform* instance = getInstance(state);
                instance->set(*param1, *param2, *param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Matrix");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 3.");
                    lua_error(state);
                }
                Matrix* param2 = (Matrix*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                Transform* instance = getInstance(state);
                instance->set(*param1, *param2, *param3);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector3");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 5.");
                    lua_error(state);
                }
                Vector3* param4 = (Vector3*)((ScriptController::LuaObject*)userdata5)->instance;

                Transform* instance = getInstance(state);
                instance->set(*param1, *param2, param3, *param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "AnimationValue");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'AnimationValue' for parameter 3.");
                    lua_error(state);
                }
                AnimationValue* param2 = (AnimationValue*)((ScriptController::LuaObject*)userdata3)->instance;

                Transform* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "AnimationValue");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'AnimationValue' for parameter 3.");
                    lua_error(state);
                }
                AnimationValue* param2 = (AnimationValue*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Transform* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setIdentity(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                Transform* instance = getInstance(state);
                instance->setIdentity();
                
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

int lua_Transform_setRotation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->setRotation(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Matrix");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 2.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->setRotation(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Transform* instance = getInstance(state);
                instance->setRotation(*param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                Transform* instance = getInstance(state);
                instance->setRotation(param1, param2, param3, param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setScale(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setScale(param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->setScale(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Transform* instance = getInstance(state);
                instance->setScale(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setScaleX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setScaleX(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setScaleY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setScaleY(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setScaleZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setScaleZ(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setTranslation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->setTranslation(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Transform* instance = getInstance(state);
                instance->setTranslation(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setTranslationX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setTranslationX(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setTranslationY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setTranslationY(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_setTranslationZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->setTranslationZ(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_static_ANIMATE_ROTATE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_ROTATE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_ROTATE_TRANSLATE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_ROTATE_TRANSLATE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE_ROTATE_TRANSLATE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE_ROTATE_TRANSLATE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE_UNIT(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE_UNIT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE_X(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE_X;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE_Y(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE_Y;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_SCALE_Z(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_SCALE_Z;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_TRANSLATE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_TRANSLATE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_TRANSLATE_X(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_TRANSLATE_X;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_TRANSLATE_Y(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_TRANSLATE_Y;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_ANIMATE_TRANSLATE_Z(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Transform::ANIMATE_TRANSLATE_Z;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Transform_static_isTransformChangedSuspended(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Transform::isTransformChangedSuspended();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Transform_static_resumeTransformChanged(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            Transform::resumeTransformChanged();
            
            return 0;
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

int lua_Transform_static_suspendTransformChanged(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            Transform::suspendTransformChanged();
            
            return 0;
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

int lua_Transform_transformPoint(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->transformPoint(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                Transform* instance = getInstance(state);
                instance->transformPoint(*param1, param2);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_transformVector(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->transformVector(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                Transform* instance = getInstance(state);
                instance->transformVector(*param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector3");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 6.");
                    lua_error(state);
                }
                Vector3* param5 = (Vector3*)((ScriptController::LuaObject*)userdata6)->instance;

                Transform* instance = getInstance(state);
                instance->transformVector(param1, param2, param3, param4, param5);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translate(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                Transform* instance = getInstance(state);
                instance->translate(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Transform* instance = getInstance(state);
                instance->translate(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateForward(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateForward(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateLeft(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateLeft(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateUp(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateUp(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateX(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateY(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Transform_translateZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Transform* instance = getInstance(state);
                instance->translateZ(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
