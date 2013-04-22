#include "Base.h"
#include "ScriptController.h"
#include "lua_PhysicsCharacter.h"
#include "Animation.h"
#include "AnimationTarget.h"
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
#include "Scene.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Transform.h"
#include "lua_CurveInterpolationType.h"
#include "lua_PhysicsCollisionObjectCollisionListenerEventType.h"
#include "lua_PhysicsCollisionObjectType.h"
#include "lua_PhysicsCollisionShapeType.h"

namespace gameplay
{

void luaRegister_PhysicsCharacter()
{
    const luaL_Reg lua_members[] = 
    {
        {"addCollisionListener", lua_PhysicsCharacter_addCollisionListener},
        {"asCharacter", lua_PhysicsCharacter_asCharacter},
        {"asGhostObject", lua_PhysicsCharacter_asGhostObject},
        {"asRigidBody", lua_PhysicsCharacter_asRigidBody},
        {"asVehicle", lua_PhysicsCharacter_asVehicle},
        {"asVehicleWheel", lua_PhysicsCharacter_asVehicleWheel},
        {"collidesWith", lua_PhysicsCharacter_collidesWith},
        {"getCollisionShape", lua_PhysicsCharacter_getCollisionShape},
        {"getCurrentVelocity", lua_PhysicsCharacter_getCurrentVelocity},
        {"getMaxSlopeAngle", lua_PhysicsCharacter_getMaxSlopeAngle},
        {"getMaxStepHeight", lua_PhysicsCharacter_getMaxStepHeight},
        {"getNode", lua_PhysicsCharacter_getNode},
        {"getShapeType", lua_PhysicsCharacter_getShapeType},
        {"getType", lua_PhysicsCharacter_getType},
        {"isDynamic", lua_PhysicsCharacter_isDynamic},
        {"isEnabled", lua_PhysicsCharacter_isEnabled},
        {"isKinematic", lua_PhysicsCharacter_isKinematic},
        {"isPhysicsEnabled", lua_PhysicsCharacter_isPhysicsEnabled},
        {"isStatic", lua_PhysicsCharacter_isStatic},
        {"jump", lua_PhysicsCharacter_jump},
        {"removeCollisionListener", lua_PhysicsCharacter_removeCollisionListener},
        {"rotate", lua_PhysicsCharacter_rotate},
        {"setEnabled", lua_PhysicsCharacter_setEnabled},
        {"setForwardVelocity", lua_PhysicsCharacter_setForwardVelocity},
        {"setMaxSlopeAngle", lua_PhysicsCharacter_setMaxSlopeAngle},
        {"setMaxStepHeight", lua_PhysicsCharacter_setMaxStepHeight},
        {"setPhysicsEnabled", lua_PhysicsCharacter_setPhysicsEnabled},
        {"setRightVelocity", lua_PhysicsCharacter_setRightVelocity},
        {"setRotation", lua_PhysicsCharacter_setRotation},
        {"setVelocity", lua_PhysicsCharacter_setVelocity},
        {"transformChanged", lua_PhysicsCharacter_transformChanged},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("PhysicsCharacter", lua_members, NULL, NULL, lua_statics, scopePath);
}

static PhysicsCharacter* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsCharacter");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCharacter' expected.");
    return (PhysicsCharacter*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_PhysicsCharacter_addCollisionListener(lua_State* state)
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

                    PhysicsCharacter* instance = getInstance(state);
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->addCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_addCollisionListener - Failed to match the given parameters to a valid function signature.");
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

                    PhysicsCharacter* instance = getInstance(state);
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->addCollisionListener(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_addCollisionListener - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_asCharacter(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_asCharacter - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_asGhostObject(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_asGhostObject - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_asRigidBody(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_asRigidBody - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_asVehicle(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_asVehicle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_asVehicleWheel(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_asVehicleWheel - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_collidesWith(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->collidesWith(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_collidesWith - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getCollisionShape(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_getCollisionShape - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getCurrentVelocity(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                void* returnPtr = (void*)new Vector3(instance->getCurrentVelocity());
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

            lua_pushstring(state, "lua_PhysicsCharacter_getCurrentVelocity - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getMaxSlopeAngle(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                float result = instance->getMaxSlopeAngle();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_getMaxSlopeAngle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getMaxStepHeight(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                float result = instance->getMaxStepHeight();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_getMaxStepHeight - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getNode(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
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

            lua_pushstring(state, "lua_PhysicsCharacter_getNode - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getShapeType(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                PhysicsCollisionShape::Type result = instance->getShapeType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_PhysicsCollisionShapeType(result));

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_getShapeType - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_getType(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                PhysicsCollisionObject::Type result = instance->getType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_PhysicsCollisionObjectType(result));

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_getType - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_isDynamic(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->isDynamic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_isDynamic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_isEnabled(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->isEnabled();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_isEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_isKinematic(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->isKinematic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_isKinematic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_isPhysicsEnabled(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->isPhysicsEnabled();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_isPhysicsEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_isStatic(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                bool result = instance->isStatic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_isStatic - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_jump(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                instance->jump(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_jump - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_removeCollisionListener(lua_State* state)
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

                    PhysicsCharacter* instance = getInstance(state);
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->removeCollisionListener(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_removeCollisionListener - Failed to match the given parameters to a valid function signature.");
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

                    PhysicsCharacter* instance = getInstance(state);
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->removeCollisionListener(param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_removeCollisionListener - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_rotate(lua_State* state)
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->rotate(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_rotate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->rotate(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_rotate - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setEnabled(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                instance->setEnabled(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setForwardVelocity(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                instance->setForwardVelocity();
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setForwardVelocity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                PhysicsCharacter* instance = getInstance(state);
                instance->setForwardVelocity(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setForwardVelocity - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setMaxSlopeAngle(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                instance->setMaxSlopeAngle(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setMaxSlopeAngle - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setMaxStepHeight(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                instance->setMaxStepHeight(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setMaxStepHeight - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setPhysicsEnabled(lua_State* state)
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

                PhysicsCharacter* instance = getInstance(state);
                instance->setPhysicsEnabled(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setPhysicsEnabled - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setRightVelocity(lua_State* state)
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
                PhysicsCharacter* instance = getInstance(state);
                instance->setRightVelocity();
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setRightVelocity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                PhysicsCharacter* instance = getInstance(state);
                instance->setRightVelocity(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_setRightVelocity - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setRotation(lua_State* state)
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->setRotation(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_setRotation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->setRotation(*param1, param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_setRotation - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_setVelocity(lua_State* state)
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->setVelocity(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_setVelocity - Failed to match the given parameters to a valid function signature.");
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

                    PhysicsCharacter* instance = getInstance(state);
                    instance->setVelocity(param1, param2, param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_PhysicsCharacter_setVelocity - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCharacter_transformChanged(lua_State* state)
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
                gameplay::ScriptUtil::LuaArray<Transform> param1 = gameplay::ScriptUtil::getObjectPointer<Transform>(2, "Transform", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Transform'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                long param2 = (long)luaL_checklong(state, 3);

                PhysicsCharacter* instance = getInstance(state);
                instance->transformChanged(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCharacter_transformChanged - Failed to match the given parameters to a valid function signature.");
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

}
