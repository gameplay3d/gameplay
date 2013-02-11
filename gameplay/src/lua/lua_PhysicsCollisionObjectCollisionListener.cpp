#include "Base.h"
#include "ScriptController.h"
#include "lua_PhysicsCollisionObjectCollisionListener.h"
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

void luaRegister_PhysicsCollisionObjectCollisionListener()
{
    const luaL_Reg lua_members[] = 
    {
        {"collisionEvent", lua_PhysicsCollisionObjectCollisionListener_collisionEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsCollisionObject");

    gameplay::ScriptUtil::registerClass("PhysicsCollisionObjectCollisionListener", lua_members, NULL, lua_PhysicsCollisionObjectCollisionListener__gc, lua_statics, scopePath);
}

static PhysicsCollisionObject::CollisionListener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsCollisionObjectCollisionListener");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCollisionObjectCollisionListener' expected.");
    return (PhysicsCollisionObject::CollisionListener*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_PhysicsCollisionObjectCollisionListener__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "PhysicsCollisionObjectCollisionListener");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCollisionObjectCollisionListener' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsCollisionObject::CollisionListener* instance = (PhysicsCollisionObject::CollisionListener*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCollisionObjectCollisionListener__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsCollisionObjectCollisionListener_collisionEvent(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject::CollisionListener::EventType param1 = (PhysicsCollisionObject::CollisionListener::EventType)lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionPair> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'PhysicsCollisionObject::CollisionPair'.");
                    lua_error(state);
                }

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCollisionObjectCollisionListener_collisionEvent - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject::CollisionListener::EventType param1 = (PhysicsCollisionObject::CollisionListener::EventType)lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionPair> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'PhysicsCollisionObject::CollisionPair'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector3'.");
                    lua_error(state);
                }

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2, *param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCollisionObjectCollisionListener_collisionEvent - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject::CollisionListener::EventType param1 = (PhysicsCollisionObject::CollisionListener::EventType)lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject::CollisionPair> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'PhysicsCollisionObject::CollisionPair'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector3'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param4 = gameplay::ScriptUtil::getObjectPointer<Vector3>(5, "Vector3", true, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector3'.");
                    lua_error(state);
                }

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2, *param3, *param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsCollisionObjectCollisionListener_collisionEvent - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
