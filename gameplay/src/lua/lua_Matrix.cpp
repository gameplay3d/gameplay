#include "Base.h"
#include "ScriptController.h"
#include "lua_Matrix.h"
#include "Base.h"
#include "MathUtil.h"
#include "Matrix.h"
#include "Plane.h"
#include "Quaternion.h"

namespace gameplay
{

void luaRegister_Matrix()
{
    const luaL_Reg lua_members[] = 
    {
        {"add", lua_Matrix_add},
        {"decompose", lua_Matrix_decompose},
        {"determinant", lua_Matrix_determinant},
        {"getBackVector", lua_Matrix_getBackVector},
        {"getDownVector", lua_Matrix_getDownVector},
        {"getForwardVector", lua_Matrix_getForwardVector},
        {"getLeftVector", lua_Matrix_getLeftVector},
        {"getRightVector", lua_Matrix_getRightVector},
        {"getRotation", lua_Matrix_getRotation},
        {"getScale", lua_Matrix_getScale},
        {"getTranslation", lua_Matrix_getTranslation},
        {"getUpVector", lua_Matrix_getUpVector},
        {"invert", lua_Matrix_invert},
        {"isIdentity", lua_Matrix_isIdentity},
        {"m", lua_Matrix_m},
        {"multiply", lua_Matrix_multiply},
        {"negate", lua_Matrix_negate},
        {"rotate", lua_Matrix_rotate},
        {"rotateX", lua_Matrix_rotateX},
        {"rotateY", lua_Matrix_rotateY},
        {"rotateZ", lua_Matrix_rotateZ},
        {"scale", lua_Matrix_scale},
        {"set", lua_Matrix_set},
        {"setIdentity", lua_Matrix_setIdentity},
        {"setZero", lua_Matrix_setZero},
        {"subtract", lua_Matrix_subtract},
        {"transformPoint", lua_Matrix_transformPoint},
        {"transformVector", lua_Matrix_transformVector},
        {"translate", lua_Matrix_translate},
        {"transpose", lua_Matrix_transpose},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"add", lua_Matrix_static_add},
        {"createBillboard", lua_Matrix_static_createBillboard},
        {"createLookAt", lua_Matrix_static_createLookAt},
        {"createOrthographic", lua_Matrix_static_createOrthographic},
        {"createOrthographicOffCenter", lua_Matrix_static_createOrthographicOffCenter},
        {"createPerspective", lua_Matrix_static_createPerspective},
        {"createReflection", lua_Matrix_static_createReflection},
        {"createRotation", lua_Matrix_static_createRotation},
        {"createRotationX", lua_Matrix_static_createRotationX},
        {"createRotationY", lua_Matrix_static_createRotationY},
        {"createRotationZ", lua_Matrix_static_createRotationZ},
        {"createScale", lua_Matrix_static_createScale},
        {"createTranslation", lua_Matrix_static_createTranslation},
        {"identity", lua_Matrix_static_identity},
        {"multiply", lua_Matrix_static_multiply},
        {"subtract", lua_Matrix_static_subtract},
        {"zero", lua_Matrix_static_zero},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Matrix", lua_members, lua_Matrix__init, lua_Matrix__gc, lua_statics, scopePath);
}

static Matrix* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Matrix");
    luaL_argcheck(state, userdata != NULL, 1, "'Matrix' expected.");
    return (Matrix*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Matrix__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Matrix");
                luaL_argcheck(state, userdata != NULL, 1, "'Matrix' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Matrix* instance = (Matrix*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new Matrix();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "Matrix");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TLIGHTUSERDATA))
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(1);

                    void* returnPtr = (void*)new Matrix(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Matrix");
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(1, "Matrix", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    void* returnPtr = (void*)new Matrix(*param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Matrix");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 16:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    lua_type(state, 11) == LUA_TNUMBER &&
                    lua_type(state, 12) == LUA_TNUMBER &&
                    lua_type(state, 13) == LUA_TNUMBER &&
                    lua_type(state, 14) == LUA_TNUMBER &&
                    lua_type(state, 15) == LUA_TNUMBER &&
                    lua_type(state, 16) == LUA_TNUMBER)
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

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 6);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 7);

                    // Get parameter 8 off the stack.
                    float param8 = (float)luaL_checknumber(state, 8);

                    // Get parameter 9 off the stack.
                    float param9 = (float)luaL_checknumber(state, 9);

                    // Get parameter 10 off the stack.
                    float param10 = (float)luaL_checknumber(state, 10);

                    // Get parameter 11 off the stack.
                    float param11 = (float)luaL_checknumber(state, 11);

                    // Get parameter 12 off the stack.
                    float param12 = (float)luaL_checknumber(state, 12);

                    // Get parameter 13 off the stack.
                    float param13 = (float)luaL_checknumber(state, 13);

                    // Get parameter 14 off the stack.
                    float param14 = (float)luaL_checknumber(state, 14);

                    // Get parameter 15 off the stack.
                    float param15 = (float)luaL_checknumber(state, 15);

                    // Get parameter 16 off the stack.
                    float param16 = (float)luaL_checknumber(state, 16);

                    void* returnPtr = (void*)new Matrix(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, param11, param12, param13, param14, param15, param16);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Matrix");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1 or 16).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_add(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->add(param1);
                    
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

                    Matrix* instance = getInstance(state);
                    instance->add(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_add - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->add(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_add - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_decompose(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
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
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Quaternion> param2 = gameplay::ScriptUtil::getObjectPointer<Quaternion>(3, "Quaternion", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Quaternion'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", false, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                bool result = instance->decompose(param1, param2, param3);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Matrix_decompose - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_determinant(lua_State* state)
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
                Matrix* instance = getInstance(state);
                float result = instance->determinant();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Matrix_determinant - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getBackVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getBackVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getBackVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getDownVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getDownVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getDownVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getForwardVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getForwardVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getForwardVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getLeftVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getLeftVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getLeftVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getRightVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getRightVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getRightVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getRotation(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Quaternion> param1 = gameplay::ScriptUtil::getObjectPointer<Quaternion>(2, "Quaternion", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Quaternion'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                bool result = instance->getRotation(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Matrix_getRotation - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getScale(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getScale(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getScale - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getTranslation(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getTranslation(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getTranslation - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_getUpVector(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                Matrix* instance = getInstance(state);
                instance->getUpVector(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_getUpVector - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_invert(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA))
                {
                    Matrix* instance = getInstance(state);
                    bool result = instance->invert();

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_invert - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
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

                    Matrix* instance = getInstance(state);
                    bool result = instance->invert(param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_invert - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_isIdentity(lua_State* state)
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
                Matrix* instance = getInstance(state);
                bool result = instance->isIdentity();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Matrix_isIdentity - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_m(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Matrix* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        gameplay::ScriptUtil::LuaArray<float> param2 = gameplay::ScriptUtil::getFloatPointer(2);

        memcpy(instance->m, param2, sizeof(float) * 16);
        return 0;
    }
    else
    {
        float* result = instance->m;

        // Push the return value onto the stack.
        lua_pushlightuserdata(state, result);
        return 1;
    }
}

int lua_Matrix_multiply(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->multiply(param1);
                    
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

                    Matrix* instance = getInstance(state);
                    instance->multiply(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_multiply - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->multiply(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_multiply - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_negate(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA))
                {
                    Matrix* instance = getInstance(state);
                    instance->negate();
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_negate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
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

                    Matrix* instance = getInstance(state);
                    instance->negate(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_negate - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_rotate(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Quaternion> param1 = gameplay::ScriptUtil::getObjectPointer<Quaternion>(2, "Quaternion", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotate(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Quaternion> param1 = gameplay::ScriptUtil::getObjectPointer<Quaternion>(2, "Quaternion", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotate(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    Matrix* instance = getInstance(state);
                    instance->rotate(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(4, "Matrix", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotate(*param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_rotateX(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->rotateX(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateX - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotateX(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateX - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_rotateY(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->rotateY(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateY - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotateY(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateY - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_rotateZ(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->rotateZ(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateZ - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->rotateZ(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_rotateZ - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_scale(lua_State* state)
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

                    Matrix* instance = getInstance(state);
                    instance->scale(param1);
                    
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

                    Matrix* instance = getInstance(state);
                    instance->scale(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_scale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->scale(param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->scale(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_scale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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

                    Matrix* instance = getInstance(state);
                    instance->scale(param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_scale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(5, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->scale(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_scale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_set(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                    Matrix* instance = getInstance(state);
                    instance->set(param1);
                    
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

                    Matrix* instance = getInstance(state);
                    instance->set(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 17:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    lua_type(state, 11) == LUA_TNUMBER &&
                    lua_type(state, 12) == LUA_TNUMBER &&
                    lua_type(state, 13) == LUA_TNUMBER &&
                    lua_type(state, 14) == LUA_TNUMBER &&
                    lua_type(state, 15) == LUA_TNUMBER &&
                    lua_type(state, 16) == LUA_TNUMBER &&
                    lua_type(state, 17) == LUA_TNUMBER)
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
                    float param5 = (float)luaL_checknumber(state, 6);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 8);

                    // Get parameter 8 off the stack.
                    float param8 = (float)luaL_checknumber(state, 9);

                    // Get parameter 9 off the stack.
                    float param9 = (float)luaL_checknumber(state, 10);

                    // Get parameter 10 off the stack.
                    float param10 = (float)luaL_checknumber(state, 11);

                    // Get parameter 11 off the stack.
                    float param11 = (float)luaL_checknumber(state, 12);

                    // Get parameter 12 off the stack.
                    float param12 = (float)luaL_checknumber(state, 13);

                    // Get parameter 13 off the stack.
                    float param13 = (float)luaL_checknumber(state, 14);

                    // Get parameter 14 off the stack.
                    float param14 = (float)luaL_checknumber(state, 15);

                    // Get parameter 15 off the stack.
                    float param15 = (float)luaL_checknumber(state, 16);

                    // Get parameter 16 off the stack.
                    float param16 = (float)luaL_checknumber(state, 17);

                    Matrix* instance = getInstance(state);
                    instance->set(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, param11, param12, param13, param14, param15, param16);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 17).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_setIdentity(lua_State* state)
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
                Matrix* instance = getInstance(state);
                instance->setIdentity();
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_setIdentity - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_setZero(lua_State* state)
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
                Matrix* instance = getInstance(state);
                instance->setZero();
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_setZero - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_add(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(1, "Matrix", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::add(*param1, *param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_add - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createBillboard(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(4, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix::createBillboard(*param1, *param2, *param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createBillboard - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param4 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    bool param5Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param5 = gameplay::ScriptUtil::getObjectPointer<Matrix>(5, "Matrix", false, &param5Valid);
                    if (!param5Valid)
                        break;

                    Matrix::createBillboard(*param1, *param2, *param3, *param4, param5);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createBillboard - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_static_createLookAt(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(4, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix::createLookAt(*param1, *param2, *param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createLookAt - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 10:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    (lua_type(state, 10) == LUA_TUSERDATA || lua_type(state, 10) == LUA_TTABLE || lua_type(state, 10) == LUA_TNIL))
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

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 6);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 7);

                    // Get parameter 8 off the stack.
                    float param8 = (float)luaL_checknumber(state, 8);

                    // Get parameter 9 off the stack.
                    float param9 = (float)luaL_checknumber(state, 9);

                    // Get parameter 10 off the stack.
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param10 = gameplay::ScriptUtil::getObjectPointer<Matrix>(10, "Matrix", false, &param10Valid);
                    if (!param10Valid)
                        break;

                    Matrix::createLookAt(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createLookAt - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4 or 10).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_static_createOrthographic(lua_State* state)
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
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
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
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param5 = gameplay::ScriptUtil::getObjectPointer<Matrix>(5, "Matrix", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createOrthographic(param1, param2, param3, param4, param5);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createOrthographic - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createOrthographicOffCenter(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TTABLE || lua_type(state, 7) == LUA_TNIL))
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

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 6);

                // Get parameter 7 off the stack.
                bool param7Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param7 = gameplay::ScriptUtil::getObjectPointer<Matrix>(7, "Matrix", false, &param7Valid);
                if (!param7Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 7 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createOrthographicOffCenter(param1, param2, param3, param4, param5, param6, param7);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createOrthographicOffCenter - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createPerspective(lua_State* state)
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
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
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
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param5 = gameplay::ScriptUtil::getObjectPointer<Matrix>(5, "Matrix", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createPerspective(param1, param2, param3, param4, param5);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createPerspective - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createReflection(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Plane> param1 = gameplay::ScriptUtil::getObjectPointer<Plane>(1, "Plane", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Plane'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createReflection(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createReflection - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createRotation(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Quaternion> param1 = gameplay::ScriptUtil::getObjectPointer<Quaternion>(1, "Quaternion", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix::createRotation(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createRotation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Matrix::createRotation(*param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createRotation - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createRotationX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createRotationX(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createRotationX - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createRotationY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createRotationY(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createRotationY - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createRotationZ(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::createRotationZ(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_createRotationZ - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_createScale(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix::createScale(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createScale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(4, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix::createScale(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createScale - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_Matrix_static_createTranslation(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix::createTranslation(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createTranslation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(4, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix::createTranslation(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_createTranslation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_Matrix_static_identity(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(Matrix::identity());
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

int lua_Matrix_static_multiply(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(1, "Matrix", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Matrix::multiply(*param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(1, "Matrix", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    Matrix::multiply(*param1, *param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_static_multiply - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_subtract(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(1, "Matrix", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Matrix'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param3 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Matrix'.");
                    lua_error(state);
                }

                Matrix::subtract(*param1, *param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_static_subtract - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_static_zero(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(Matrix::zero());
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

int lua_Matrix_subtract(lua_State* state)
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

                Matrix* instance = getInstance(state);
                instance->subtract(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Matrix_subtract - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_transformPoint(lua_State* state)
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
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformPoint(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transformPoint - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformPoint(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transformPoint - Failed to match the given parameters to a valid function signature.");
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

int lua_Matrix_transformVector(lua_State* state)
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
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformVector(param1);
                    
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

                    Matrix* instance = getInstance(state);
                    instance->transformVector(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transformVector - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformVector(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param2 = gameplay::ScriptUtil::getObjectPointer<Vector4>(3, "Vector4", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformVector(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transformVector - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL))
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
                    bool param5Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param5 = gameplay::ScriptUtil::getObjectPointer<Vector3>(6, "Vector3", false, &param5Valid);
                    if (!param5Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->transformVector(param1, param2, param3, param4, param5);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transformVector - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_Matrix_translate(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->translate(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_translate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param2 = gameplay::ScriptUtil::getObjectPointer<Matrix>(3, "Matrix", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->translate(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_translate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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

                    Matrix* instance = getInstance(state);
                    instance->translate(param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_translate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Matrix> param4 = gameplay::ScriptUtil::getObjectPointer<Matrix>(5, "Matrix", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Matrix* instance = getInstance(state);
                    instance->translate(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_translate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Matrix_transpose(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA))
                {
                    Matrix* instance = getInstance(state);
                    instance->transpose();
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transpose - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
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

                    Matrix* instance = getInstance(state);
                    instance->transpose(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Matrix_transpose - Failed to match the given parameters to a valid function signature.");
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

}
