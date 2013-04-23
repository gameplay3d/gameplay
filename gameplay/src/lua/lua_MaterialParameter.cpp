#include "Base.h"
#include "ScriptController.h"
#include "lua_MaterialParameter.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "Game.h"
#include "MaterialParameter.h"
#include "Node.h"
#include "Ref.h"
#include "lua_CurveInterpolationType.h"

namespace gameplay
{

void luaRegister_MaterialParameter()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_MaterialParameter_addRef},
        {"bindValue", lua_MaterialParameter_bindValue},
        {"createAnimation", lua_MaterialParameter_createAnimation},
        {"createAnimationFromBy", lua_MaterialParameter_createAnimationFromBy},
        {"createAnimationFromTo", lua_MaterialParameter_createAnimationFromTo},
        {"destroyAnimation", lua_MaterialParameter_destroyAnimation},
        {"getAnimation", lua_MaterialParameter_getAnimation},
        {"getAnimationPropertyComponentCount", lua_MaterialParameter_getAnimationPropertyComponentCount},
        {"getAnimationPropertyValue", lua_MaterialParameter_getAnimationPropertyValue},
        {"getName", lua_MaterialParameter_getName},
        {"getRefCount", lua_MaterialParameter_getRefCount},
        {"getSampler", lua_MaterialParameter_getSampler},
        {"release", lua_MaterialParameter_release},
        {"setAnimationPropertyValue", lua_MaterialParameter_setAnimationPropertyValue},
        {"setFloat", lua_MaterialParameter_setFloat},
        {"setFloatArray", lua_MaterialParameter_setFloatArray},
        {"setInt", lua_MaterialParameter_setInt},
        {"setIntArray", lua_MaterialParameter_setIntArray},
        {"setMatrix", lua_MaterialParameter_setMatrix},
        {"setMatrixArray", lua_MaterialParameter_setMatrixArray},
        {"setSampler", lua_MaterialParameter_setSampler},
        {"setValue", lua_MaterialParameter_setValue},
        {"setVector2", lua_MaterialParameter_setVector2},
        {"setVector2Array", lua_MaterialParameter_setVector2Array},
        {"setVector3", lua_MaterialParameter_setVector3},
        {"setVector3Array", lua_MaterialParameter_setVector3Array},
        {"setVector4", lua_MaterialParameter_setVector4},
        {"setVector4Array", lua_MaterialParameter_setVector4Array},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"ANIMATE_UNIFORM", lua_MaterialParameter_static_ANIMATE_UNIFORM},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("MaterialParameter", lua_members, NULL, lua_MaterialParameter__gc, lua_statics, scopePath);
}

static MaterialParameter* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MaterialParameter");
    luaL_argcheck(state, userdata != NULL, 1, "'MaterialParameter' expected.");
    return (MaterialParameter*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_MaterialParameter__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "MaterialParameter");
                luaL_argcheck(state, userdata != NULL, 1, "'MaterialParameter' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    MaterialParameter* instance = (MaterialParameter*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_addRef(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_bindValue(lua_State* state)
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
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Node> param1 = gameplay::ScriptUtil::getObjectPointer<Node>(2, "Node", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Node'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(3, false);

                MaterialParameter* instance = getInstance(state);
                instance->bindValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_bindValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_createAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    const char* param2 = gameplay::ScriptUtil::getString(3, false);

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Properties> param2 = gameplay::ScriptUtil::getObjectPointer<Properties>(3, "Properties", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    Curve::InterpolationType param6 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 7));

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TTABLE || lua_type(state, 7) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 8) == LUA_TTABLE || lua_type(state, 8) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 9) == LUA_TSTRING || lua_type(state, 9) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param6 = gameplay::ScriptUtil::getFloatPointer(7);

                    // Get parameter 7 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param7 = gameplay::ScriptUtil::getFloatPointer(8);

                    // Get parameter 8 off the stack.
                    Curve::InterpolationType param8 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 9));

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6, param7, param8);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_createAnimationFromBy(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromBy(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_createAnimationFromBy - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_createAnimationFromTo(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromTo(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_createAnimationFromTo - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_destroyAnimation(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                instance->destroyAnimation();
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_destroyAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                MaterialParameter* instance = getInstance(state);
                instance->destroyAnimation(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_destroyAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_getAnimation(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_getAnimationPropertyComponentCount(lua_State* state)
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
                int param1 = (int)luaL_checkint(state, 2);

                MaterialParameter* instance = getInstance(state);
                unsigned int result = instance->getAnimationPropertyComponentCount(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getAnimationPropertyComponentCount - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_getAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                MaterialParameter* instance = getInstance(state);
                instance->getAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_getAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_getName(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                const char* result = instance->getName();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getName - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_getRefCount(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_getSampler(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->getSampler();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "TextureSampler");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getSampler - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                MaterialParameter* instance = getInstance(state);
                void* returnPtr = (void*)instance->getSampler(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "TextureSampler");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MaterialParameter_getSampler - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_release(lua_State* state)
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
                MaterialParameter* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_release - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                MaterialParameter* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setFloat(lua_State* state)
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

                MaterialParameter* instance = getInstance(state);
                instance->setFloat(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setFloat - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setFloatArray(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setFloatArray(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setFloatArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setFloatArray(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setFloatArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setInt(lua_State* state)
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
                int param1 = (int)luaL_checkint(state, 2);

                MaterialParameter* instance = getInstance(state);
                instance->setInt(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setInt - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setIntArray(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<int> param1 = gameplay::ScriptUtil::getIntPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setIntArray(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setIntArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<int> param1 = gameplay::ScriptUtil::getIntPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setIntArray(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setIntArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setMatrix(lua_State* state)
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

                MaterialParameter* instance = getInstance(state);
                instance->setMatrix(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setMatrixArray(lua_State* state)
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
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setMatrixArray(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setMatrixArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setMatrixArray(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setMatrixArray - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setSampler(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Texture::Sampler> param1 = gameplay::ScriptUtil::getObjectPointer<Texture::Sampler>(2, "TextureSampler", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setSampler(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_setSampler - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->setSampler(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "TextureSampler");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_setSampler - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    int param1 = (int)luaL_checkint(state, 2);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<int> param1 = gameplay::ScriptUtil::getIntPointer(2);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Texture::Sampler> param1 = gameplay::ScriptUtil::getObjectPointer<Texture::Sampler>(2, "TextureSampler", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_setValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<int> param1 = gameplay::ScriptUtil::getIntPointer(2);

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    instance->setValue(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 3);

                    MaterialParameter* instance = getInstance(state);
                    void* returnPtr = (void*)instance->setValue(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "TextureSampler");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MaterialParameter_setValue - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setVector2(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector2'.");
                    lua_error(state);
                }

                MaterialParameter* instance = getInstance(state);
                instance->setVector2(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector2 - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setVector2Array(lua_State* state)
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
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setVector2Array(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector2Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setVector2Array(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector2Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setVector3(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                MaterialParameter* instance = getInstance(state);
                instance->setVector3(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector3 - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setVector3Array(lua_State* state)
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
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setVector3Array(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector3Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setVector3Array(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector3Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_setVector4(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                MaterialParameter* instance = getInstance(state);
                instance->setVector4(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector4 - Failed to match the given parameters to a valid function signature.");
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

int lua_MaterialParameter_setVector4Array(lua_State* state)
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
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MaterialParameter* instance = getInstance(state);
                instance->setVector4Array(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector4Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                MaterialParameter* instance = getInstance(state);
                instance->setVector4Array(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MaterialParameter_setVector4Array - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_MaterialParameter_static_ANIMATE_UNIFORM(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = MaterialParameter::ANIMATE_UNIFORM;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

}
