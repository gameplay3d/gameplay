#include "Base.h"
#include "ScriptController.h"
#include "lua_PhysicsVehicleWheel.h"
#include "Base.h"
#include "Game.h"
#include "Node.h"
#include "PhysicsCharacter.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "PhysicsGhostObject.h"
#include "PhysicsRigidBody.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"
#include "ScriptController.h"
#include "lua_PhysicsCollisionObjectCollisionListenerEventType.h"
#include "lua_PhysicsCollisionObjectType.h"
#include "lua_PhysicsCollisionShapeType.h"

namespace gameplay
{

void luaRegister_PhysicsVehicleWheel()
{
    const luaL_Reg lua_members[] = 
    {
        {"addCollisionListener", lua_PhysicsVehicleWheel_addCollisionListener},
        {"asCharacter", lua_PhysicsVehicleWheel_asCharacter},
        {"asGhostObject", lua_PhysicsVehicleWheel_asGhostObject},
        {"asRigidBody", lua_PhysicsVehicleWheel_asRigidBody},
        {"asVehicle", lua_PhysicsVehicleWheel_asVehicle},
        {"asVehicleWheel", lua_PhysicsVehicleWheel_asVehicleWheel},
        {"collidesWith", lua_PhysicsVehicleWheel_collidesWith},
        {"getCollisionShape", lua_PhysicsVehicleWheel_getCollisionShape},
        {"getFrictionBreakout", lua_PhysicsVehicleWheel_getFrictionBreakout},
        {"getNode", lua_PhysicsVehicleWheel_getNode},
        {"getRollInfluence", lua_PhysicsVehicleWheel_getRollInfluence},
        {"getShapeType", lua_PhysicsVehicleWheel_getShapeType},
        {"getStrutConnectionOffset", lua_PhysicsVehicleWheel_getStrutConnectionOffset},
        {"getStrutDampingCompression", lua_PhysicsVehicleWheel_getStrutDampingCompression},
        {"getStrutDampingRelaxation", lua_PhysicsVehicleWheel_getStrutDampingRelaxation},
        {"getStrutForceMax", lua_PhysicsVehicleWheel_getStrutForceMax},
        {"getStrutRestLength", lua_PhysicsVehicleWheel_getStrutRestLength},
        {"getStrutStiffness", lua_PhysicsVehicleWheel_getStrutStiffness},
        {"getStrutTravelMax", lua_PhysicsVehicleWheel_getStrutTravelMax},
        {"getType", lua_PhysicsVehicleWheel_getType},
        {"getWheelAxle", lua_PhysicsVehicleWheel_getWheelAxle},
        {"getWheelDirection", lua_PhysicsVehicleWheel_getWheelDirection},
        {"getWheelRadius", lua_PhysicsVehicleWheel_getWheelRadius},
        {"isDynamic", lua_PhysicsVehicleWheel_isDynamic},
        {"isEnabled", lua_PhysicsVehicleWheel_isEnabled},
        {"isKinematic", lua_PhysicsVehicleWheel_isKinematic},
        {"isStatic", lua_PhysicsVehicleWheel_isStatic},
        {"isSteerable", lua_PhysicsVehicleWheel_isSteerable},
        {"removeCollisionListener", lua_PhysicsVehicleWheel_removeCollisionListener},
        {"setEnabled", lua_PhysicsVehicleWheel_setEnabled},
        {"setFrictionBreakout", lua_PhysicsVehicleWheel_setFrictionBreakout},
        {"setRollInfluence", lua_PhysicsVehicleWheel_setRollInfluence},
        {"setSteerable", lua_PhysicsVehicleWheel_setSteerable},
        {"setStrutConnectionOffset", lua_PhysicsVehicleWheel_setStrutConnectionOffset},
        {"setStrutDampingCompression", lua_PhysicsVehicleWheel_setStrutDampingCompression},
        {"setStrutDampingRelaxation", lua_PhysicsVehicleWheel_setStrutDampingRelaxation},
        {"setStrutForceMax", lua_PhysicsVehicleWheel_setStrutForceMax},
        {"setStrutRestLength", lua_PhysicsVehicleWheel_setStrutRestLength},
        {"setStrutStiffness", lua_PhysicsVehicleWheel_setStrutStiffness},
        {"setStrutTravelMax", lua_PhysicsVehicleWheel_setStrutTravelMax},
        {"setWheelAxle", lua_PhysicsVehicleWheel_setWheelAxle},
        {"setWheelDirection", lua_PhysicsVehicleWheel_setWheelDirection},
        {"setWheelRadius", lua_PhysicsVehicleWheel_setWheelRadius},
        {"transform", lua_PhysicsVehicleWheel_transform},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("PhysicsVehicleWheel", lua_members, NULL, NULL, lua_statics, scopePath);
}

static PhysicsVehicleWheel* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsVehicleWheel");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsVehicleWheel' expected.");
    return (PhysicsVehicleWheel*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_PhysicsVehicleWheel_addCollisionListener(lua_State* state)
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
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionListener> param1 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionListener>(2, "PhysicsCollisionObjectCollisionListener", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->addCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->addCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsVehicleWheel_addCollisionListener - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionListener> param1 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionListener>(2, "PhysicsCollisionObjectCollisionListener", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(3, "PhysicsCollisionObject", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->addCollisionListener(param1, param2);
                    
                    return 0;
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
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(3, "PhysicsCollisionObject", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->addCollisionListener(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsVehicleWheel_addCollisionListener - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_asCharacter(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->asCharacter();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsCharacter");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_asCharacter - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_asGhostObject(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->asGhostObject();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGhostObject");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_asGhostObject - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_asRigidBody(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->asRigidBody();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsRigidBody");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_asRigidBody - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_asVehicle(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->asVehicle();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsVehicle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_asVehicle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_asVehicleWheel(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->asVehicleWheel();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsVehicleWheel");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_asVehicleWheel - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_collidesWith(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param1 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'PhysicsCollisionObject'.");
                    lua_error(state);
                }

                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->collidesWith(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_collidesWith - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getCollisionShape(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                void* returnPtr = (void*)instance->getCollisionShape();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsCollisionShape");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getCollisionShape - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getFrictionBreakout(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getFrictionBreakout();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getFrictionBreakout - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getNode(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getNode - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getRollInfluence(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getRollInfluence();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getRollInfluence - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getShapeType(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                PhysicsCollisionShape::Type result = instance->getShapeType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_PhysicsCollisionShapeType(result));

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getShapeType - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutConnectionOffset(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->getStrutConnectionOffset(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutConnectionOffset - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutDampingCompression(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutDampingCompression();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutDampingCompression - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutDampingRelaxation(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutDampingRelaxation();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutDampingRelaxation - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutForceMax(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutForceMax();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutForceMax - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutRestLength(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutRestLength();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutRestLength - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutStiffness(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutStiffness();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutStiffness - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getStrutTravelMax(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getStrutTravelMax();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getStrutTravelMax - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getType(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                PhysicsCollisionObject::Type result = instance->getType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_PhysicsCollisionObjectType(result));

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getType - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getWheelAxle(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->getWheelAxle(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getWheelAxle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getWheelDirection(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->getWheelDirection(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getWheelDirection - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_getWheelRadius(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                float result = instance->getWheelRadius();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_getWheelRadius - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_isDynamic(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->isDynamic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_isDynamic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_isEnabled(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->isEnabled();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_isEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_isKinematic(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->isKinematic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_isKinematic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_isStatic(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->isStatic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_isStatic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_isSteerable(lua_State* state)
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
                PhysicsVehicleWheel* instance = getInstance(state);
                bool result = instance->isSteerable();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_isSteerable - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_removeCollisionListener(lua_State* state)
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
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionListener> param1 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionListener>(2, "PhysicsCollisionObjectCollisionListener", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->removeCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->removeCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsVehicleWheel_removeCollisionListener - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionListener> param1 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionListener>(2, "PhysicsCollisionObjectCollisionListener", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(3, "PhysicsCollisionObject", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->removeCollisionListener(param1, param2);
                    
                    return 0;
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
                    gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(3, "PhysicsCollisionObject", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    PhysicsVehicleWheel* instance = getInstance(state);
                    instance->removeCollisionListener(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsVehicleWheel_removeCollisionListener - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setEnabled(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setEnabled(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setFrictionBreakout(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setFrictionBreakout(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setFrictionBreakout - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setRollInfluence(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setRollInfluence(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setRollInfluence - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setSteerable(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setSteerable(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setSteerable - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutConnectionOffset(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutConnectionOffset(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutConnectionOffset - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutDampingCompression(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutDampingCompression(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutDampingCompression - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutDampingRelaxation(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutDampingRelaxation(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutDampingRelaxation - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutForceMax(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutForceMax(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutForceMax - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutRestLength(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutRestLength(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutRestLength - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutStiffness(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutStiffness(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutStiffness - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setStrutTravelMax(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setStrutTravelMax(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setStrutTravelMax - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setWheelAxle(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setWheelAxle(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setWheelAxle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setWheelDirection(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setWheelDirection(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setWheelDirection - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_setWheelRadius(lua_State* state)
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

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->setWheelRadius(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_setWheelRadius - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsVehicleWheel_transform(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Node> param1 = gameplay::ScriptUtil::getObjectPointer<Node>(2, "Node", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Node'.");
                    lua_error(state);
                }

                PhysicsVehicleWheel* instance = getInstance(state);
                instance->transform(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsVehicleWheel_transform - Failed to match the given parameters to a valid function signature.");
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
