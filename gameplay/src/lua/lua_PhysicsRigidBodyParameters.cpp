#include "Base.h"
#include "ScriptController.h"
#include "lua_PhysicsRigidBodyParameters.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "Game.h"
#include "Image.h"
#include "MeshPart.h"
#include "Node.h"
#include "PhysicsCharacter.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "PhysicsGhostObject.h"
#include "PhysicsRigidBody.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Terrain.h"
#include "Transform.h"
#include "lua_CurveInterpolationType.h"
#include "lua_PhysicsCollisionObjectCollisionListenerEventType.h"
#include "lua_PhysicsCollisionObjectType.h"
#include "lua_PhysicsCollisionShapeType.h"

namespace gameplay
{

void luaRegister_PhysicsRigidBodyParameters()
{
    const luaL_Reg lua_members[] = 
    {
        {"angularDamping", lua_PhysicsRigidBodyParameters_angularDamping},
        {"angularFactor", lua_PhysicsRigidBodyParameters_angularFactor},
        {"anisotropicFriction", lua_PhysicsRigidBodyParameters_anisotropicFriction},
        {"friction", lua_PhysicsRigidBodyParameters_friction},
        {"kinematic", lua_PhysicsRigidBodyParameters_kinematic},
        {"linearDamping", lua_PhysicsRigidBodyParameters_linearDamping},
        {"linearFactor", lua_PhysicsRigidBodyParameters_linearFactor},
        {"mass", lua_PhysicsRigidBodyParameters_mass},
        {"restitution", lua_PhysicsRigidBodyParameters_restitution},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsRigidBody");

    gameplay::ScriptUtil::registerClass("PhysicsRigidBodyParameters", lua_members, lua_PhysicsRigidBodyParameters__init, lua_PhysicsRigidBodyParameters__gc, lua_statics, scopePath);
}

static PhysicsRigidBody::Parameters* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsRigidBodyParameters");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsRigidBodyParameters' expected.");
    return (PhysicsRigidBody::Parameters*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "PhysicsRigidBodyParameters");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsRigidBodyParameters' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsRigidBody::Parameters* instance = (PhysicsRigidBody::Parameters*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsRigidBodyParameters__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new PhysicsRigidBody::Parameters();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "PhysicsRigidBodyParameters");
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
                if (lua_type(state, 1) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
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

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
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

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
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

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
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
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 6);

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TBOOLEAN &&
                    (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL))
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
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 6);

                    // Get parameter 7 off the stack.
                    bool param7Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param7 = gameplay::ScriptUtil::getObjectPointer<Vector3>(7, "Vector3", true, &param7Valid);
                    if (!param7Valid)
                        break;

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6, *param7);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TBOOLEAN &&
                    (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL) &&
                    (lua_type(state, 8) == LUA_TUSERDATA || lua_type(state, 8) == LUA_TNIL))
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
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 6);

                    // Get parameter 7 off the stack.
                    bool param7Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param7 = gameplay::ScriptUtil::getObjectPointer<Vector3>(7, "Vector3", true, &param7Valid);
                    if (!param7Valid)
                        break;

                    // Get parameter 8 off the stack.
                    bool param8Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param8 = gameplay::ScriptUtil::getObjectPointer<Vector3>(8, "Vector3", true, &param8Valid);
                    if (!param8Valid)
                        break;

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6, *param7, *param8);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TBOOLEAN &&
                    (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL) &&
                    (lua_type(state, 8) == LUA_TUSERDATA || lua_type(state, 8) == LUA_TNIL) &&
                    (lua_type(state, 9) == LUA_TUSERDATA || lua_type(state, 9) == LUA_TNIL))
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
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 6);

                    // Get parameter 7 off the stack.
                    bool param7Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param7 = gameplay::ScriptUtil::getObjectPointer<Vector3>(7, "Vector3", true, &param7Valid);
                    if (!param7Valid)
                        break;

                    // Get parameter 8 off the stack.
                    bool param8Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param8 = gameplay::ScriptUtil::getObjectPointer<Vector3>(8, "Vector3", true, &param8Valid);
                    if (!param8Valid)
                        break;

                    // Get parameter 9 off the stack.
                    bool param9Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param9 = gameplay::ScriptUtil::getObjectPointer<Vector3>(9, "Vector3", true, &param9Valid);
                    if (!param9Valid)
                        break;

                    void* returnPtr = (void*)new PhysicsRigidBody::Parameters(param1, param2, param3, param4, param5, param6, *param7, *param8, *param9);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "PhysicsRigidBodyParameters");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsRigidBodyParameters__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1, 2, 3, 4, 5, 6, 7, 8 or 9).");
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

int lua_PhysicsRigidBodyParameters_angularFactor(lua_State* state)
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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->angularFactor = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->angularFactor);
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = true;
            luaL_getmetatable(state, "Vector3");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->anisotropicFriction = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->anisotropicFriction);
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = true;
            luaL_getmetatable(state, "Vector3");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

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
        bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 2);

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

int lua_PhysicsRigidBodyParameters_linearFactor(lua_State* state)
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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->linearFactor = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->linearFactor);
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = true;
            luaL_getmetatable(state, "Vector3");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

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
