#include "Base.h"
#include "ScriptController.h"
#include "PhysicsRigidBody.h"
#include "lua_PhysicsRigidBodyParameters.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsRigidBodyParameters()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"angularDamping", lua_PhysicsRigidBodyParameters_angularDamping},
        {"anisotropicFriction", lua_PhysicsRigidBodyParameters_anisotropicFriction},
        {"friction", lua_PhysicsRigidBodyParameters_friction},
        {"kinematic", lua_PhysicsRigidBodyParameters_kinematic},
        {"linearDamping", lua_PhysicsRigidBodyParameters_linearDamping},
        {"mass", lua_PhysicsRigidBodyParameters_mass},
        {"restitution", lua_PhysicsRigidBodyParameters_restitution},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsRigidBody");

    sc->registerClass("PhysicsRigidBodyParameters", lua_members, lua_PhysicsRigidBodyParameters__init, lua_PhysicsRigidBodyParameters__gc, lua_statics, scopePath);
}

static PhysicsRigidBody::Parameters* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsRigidBodyParameters");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsRigidBodyParameters' expected.");
    return (PhysicsRigidBody::Parameters*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsRigidBodyParameters__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "PhysicsRigidBodyParameters");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsRigidBodyParameters' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsRigidBody::Parameters* instance = (PhysicsRigidBody::Parameters*)object->instance;
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

int lua_PhysicsRigidBodyParameters__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new PhysicsRigidBody::Parameters();
            object->owns = true;
            luaL_getmetatable(state, "PhysicsRigidBodyParameters");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if (lua_type(state, 1) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 1);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 3);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
        case 6:
        {
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TBOOLEAN)
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
                bool param6 = (luaL_checkint(state, 6) != 0);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
            if (lua_type(state, 1) == LUA_TNUMBER &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TBOOLEAN &&
                lua_type(state, 7) == LUA_TUSERDATA)
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
                bool param6 = (luaL_checkint(state, 6) != 0);

                // Get parameter 7 off the stack.
                void* userdata7 = ScriptController::getInstance()->getObjectPointer(7, "Vector3");
                if (!userdata7)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 7.");
                    lua_error(state);
                }
                Vector3* param7 = (Vector3*)((ScriptController::LuaObject*)userdata7)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6, *param7);
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1, 2, 3, 4, 5, 6 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsRigidBodyParameters_angularDamping(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->angularDamping = param2;
        return 0;
    }
    else
    {
        float result = instance->angularDamping;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_anisotropicFriction(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
        if (!userdata2)
        {
            lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
            lua_error(state);
        }
        Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

        instance->anisotropicFriction = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->anisotropicFriction);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_friction(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->friction = param2;
        return 0;
    }
    else
    {
        float result = instance->friction;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_kinematic(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        bool param2 = (luaL_checkint(state, 2) != 0);

        instance->kinematic = param2;
        return 0;
    }
    else
    {
        bool result = instance->kinematic;

        // Push the return value onto the stack.
        lua_pushboolean(state, result);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_linearDamping(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->linearDamping = param2;
        return 0;
    }
    else
    {
        float result = instance->linearDamping;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_mass(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->mass = param2;
        return 0;
    }
    else
    {
        float result = instance->mass;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_PhysicsRigidBodyParameters_restitution(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsRigidBody::Parameters* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->restitution = param2;
        return 0;
    }
    else
    {
        float result = instance->restitution;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

}
