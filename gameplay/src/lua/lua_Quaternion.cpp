#include "Base.h"
#include "ScriptController.h"
#include "Quaternion.h"
#include "lua_Quaternion.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Quaternion()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"conjugate", lua_Quaternion_conjugate},
        {"inverse", lua_Quaternion_inverse},
        {"isIdentity", lua_Quaternion_isIdentity},
        {"isZero", lua_Quaternion_isZero},
        {"multiply", lua_Quaternion_multiply},
        {"normalize", lua_Quaternion_normalize},
        {"set", lua_Quaternion_set},
        {"setIdentity", lua_Quaternion_setIdentity},
        {"toAxisAngle", lua_Quaternion_toAxisAngle},
        {"w", lua_Quaternion_w},
        {"x", lua_Quaternion_x},
        {"y", lua_Quaternion_y},
        {"z", lua_Quaternion_z},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"createFromAxisAngle", lua_Quaternion_static_createFromAxisAngle},
        {"createFromRotationMatrix", lua_Quaternion_static_createFromRotationMatrix},
        {"identity", lua_Quaternion_static_identity},
        {"lerp", lua_Quaternion_static_lerp},
        {"multiply", lua_Quaternion_static_multiply},
        {"slerp", lua_Quaternion_static_slerp},
        {"squad", lua_Quaternion_static_squad},
        {"zero", lua_Quaternion_static_zero},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("Quaternion", lua_members, lua_Quaternion__init, lua_Quaternion__gc, lua_statics, scopePath);
}

static Quaternion* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Quaternion");
    luaL_argcheck(state, userdata != NULL, 1, "'Quaternion' expected.");
    return (Quaternion*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Quaternion__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Quaternion");
                luaL_argcheck(state, userdata != NULL, 1, "'Quaternion' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Quaternion* instance = (Quaternion*)object->instance;
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

int lua_Quaternion__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new Quaternion();
            object->owns = true;
            luaL_getmetatable(state, "Quaternion");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                float* param1 = ScriptController::getInstance()->getFloatPointer(1);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Quaternion(param1);
                object->owns = true;
                luaL_getmetatable(state, "Quaternion");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Matrix");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 1.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Quaternion(*param1);
                object->owns = true;
                luaL_getmetatable(state, "Quaternion");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Quaternion");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 1.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Quaternion(*param1);
                object->owns = true;
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
                lua_type(state, 2) == LUA_TNUMBER)
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
                float param2 = (float)luaL_checknumber(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Quaternion(*param1, param2);
                object->owns = true;
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

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Quaternion(param1, param2, param3, param4);
                object->owns = true;
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
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1, 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Quaternion_conjugate(lua_State* state)
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
                Quaternion* instance = getInstance(state);
                instance->conjugate();
                
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

                Quaternion* instance = getInstance(state);
                instance->conjugate(param1);
                
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

int lua_Quaternion_inverse(lua_State* state)
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
                Quaternion* instance = getInstance(state);
                bool result = instance->inverse();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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

                Quaternion* instance = getInstance(state);
                bool result = instance->inverse(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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

int lua_Quaternion_isIdentity(lua_State* state)
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
                Quaternion* instance = getInstance(state);
                bool result = instance->isIdentity();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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

int lua_Quaternion_isZero(lua_State* state)
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
                Quaternion* instance = getInstance(state);
                bool result = instance->isZero();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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

int lua_Quaternion_multiply(lua_State* state)
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

                Quaternion* instance = getInstance(state);
                instance->multiply(*param1);
                
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

int lua_Quaternion_normalize(lua_State* state)
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
                Quaternion* instance = getInstance(state);
                instance->normalize();
                
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

                Quaternion* instance = getInstance(state);
                instance->normalize(param1);
                
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

int lua_Quaternion_set(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                float* param1 = ScriptController::getInstance()->getFloatPointer(2);

                Quaternion* instance = getInstance(state);
                instance->set(param1);
                
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

                Quaternion* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
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

                Quaternion* instance = getInstance(state);
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

                Quaternion* instance = getInstance(state);
                instance->set(*param1, param2);
                
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

                Quaternion* instance = getInstance(state);
                instance->set(param1, param2, param3, param4);
                
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

int lua_Quaternion_setIdentity(lua_State* state)
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
                Quaternion* instance = getInstance(state);
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

int lua_Quaternion_static_createFromAxisAngle(lua_State* state)
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
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Vector3");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 1.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                // Get parameter 3 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param3 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                Quaternion::createFromAxisAngle(*param1, param2, param3);
                
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

int lua_Quaternion_static_createFromRotationMatrix(lua_State* state)
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
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Matrix");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 1.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                Quaternion::createFromRotationMatrix(*param1, param2);
                
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

int lua_Quaternion_static_identity(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)&(Quaternion::identity());
            object->owns = false;
            luaL_getmetatable(state, "Quaternion");
            lua_setmetatable(state, -2);

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

int lua_Quaternion_static_lerp(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Quaternion");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 1.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 3);

                // Get parameter 4 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Quaternion");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 4.");
                    lua_error(state);
                }
                Quaternion* param4 = (Quaternion*)((ScriptController::LuaObject*)userdata4)->instance;

                Quaternion::lerp(*param1, *param2, param3, param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Quaternion_static_multiply(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Quaternion");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 1.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param3 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                Quaternion::multiply(*param1, *param2, param3);
                
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

int lua_Quaternion_static_slerp(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Quaternion");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 1.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 3);

                // Get parameter 4 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Quaternion");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 4.");
                    lua_error(state);
                }
                Quaternion* param4 = (Quaternion*)((ScriptController::LuaObject*)userdata4)->instance;

                Quaternion::slerp(*param1, *param2, param3, param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Quaternion_static_squad(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Quaternion");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 1.");
                    lua_error(state);
                }
                Quaternion* param1 = (Quaternion*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Quaternion");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 2.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param3 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 4 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Quaternion");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 4.");
                    lua_error(state);
                }
                Quaternion* param4 = (Quaternion*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 5 off the stack.
                float param5 = (float)luaL_checknumber(state, 5);

                // Get parameter 6 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param6 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                Quaternion::squad(*param1, *param2, *param3, *param4, param5, param6);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Quaternion_static_zero(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)&(Quaternion::zero());
            object->owns = false;
            luaL_getmetatable(state, "Quaternion");
            lua_setmetatable(state, -2);

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

int lua_Quaternion_toAxisAngle(lua_State* state)
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

                Quaternion* instance = getInstance(state);
                float result = instance->toAxisAngle(param1);

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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Quaternion_w(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Quaternion* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->w = param2;
        return 0;
    }
    else
    {
        float result = instance->w;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Quaternion_x(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Quaternion* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->x = param2;
        return 0;
    }
    else
    {
        float result = instance->x;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Quaternion_y(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Quaternion* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->y = param2;
        return 0;
    }
    else
    {
        float result = instance->y;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Quaternion_z(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Quaternion* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->z = param2;
        return 0;
    }
    else
    {
        float result = instance->z;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

}
