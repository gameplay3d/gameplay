#include "Base.h"
#include "ScriptController.h"
#include "PhysicsCollisionObject.h"
#include "lua_PhysicsCollisionObjectCollisionListener.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsCollisionObjectCollisionListener()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"collisionEvent", lua_PhysicsCollisionObjectCollisionListener_collisionEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsCollisionObject");

    sc->registerClass("PhysicsCollisionObjectCollisionListener", lua_members, NULL, lua_PhysicsCollisionObjectCollisionListener__gc, lua_statics, scopePath);
}

static PhysicsCollisionObject::CollisionListener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsCollisionObjectCollisionListener");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCollisionObjectCollisionListener' expected.");
    return (PhysicsCollisionObject::CollisionListener*)((ScriptController::LuaObject*)userdata)->instance;
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
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsCollisionObject::CollisionListener* instance = (PhysicsCollisionObject::CollisionListener*)object->instance;
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
                PhysicsCollisionObject::CollisionPair* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true);

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2);
                
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
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject::CollisionListener::EventType param1 = (PhysicsCollisionObject::CollisionListener::EventType)lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                PhysicsCollisionObject::CollisionPair* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2, *param3);
                
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
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject::CollisionListener::EventType param1 = (PhysicsCollisionObject::CollisionListener::EventType)lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                PhysicsCollisionObject::CollisionPair* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject::CollisionPair>(3, "PhysicsCollisionObjectCollisionPair", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                Vector3* param4 = ScriptController::getInstance()->getObjectPointer<Vector3>(5, "Vector3", true);

                PhysicsCollisionObject::CollisionListener* instance = getInstance(state);
                instance->collisionEvent(param1, *param2, *param3, *param4);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
