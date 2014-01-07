#ifndef LUA_PHYSICSCOLLISIONOBJECTCOLLISIONLISTENER_H_
#define LUA_PHYSICSCOLLISIONOBJECTCOLLISIONLISTENER_H_

namespace gameplay
{

// Lua bindings for PhysicsCollisionObject::CollisionListener.
int lua_PhysicsCollisionObjectCollisionListener__gc(lua_State* state);
int lua_PhysicsCollisionObjectCollisionListener_collisionEvent(lua_State* state);

void luaRegister_PhysicsCollisionObjectCollisionListener();

}

#endif
