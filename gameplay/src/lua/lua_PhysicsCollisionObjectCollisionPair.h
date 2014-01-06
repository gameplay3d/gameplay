#ifndef LUA_PHYSICSCOLLISIONOBJECTCOLLISIONPAIR_H_
#define LUA_PHYSICSCOLLISIONOBJECTCOLLISIONPAIR_H_

namespace gameplay
{

// Lua bindings for PhysicsCollisionObject::CollisionPair.
int lua_PhysicsCollisionObjectCollisionPair__gc(lua_State* state);
int lua_PhysicsCollisionObjectCollisionPair__init(lua_State* state);
int lua_PhysicsCollisionObjectCollisionPair_objectA(lua_State* state);
int lua_PhysicsCollisionObjectCollisionPair_objectB(lua_State* state);

void luaRegister_PhysicsCollisionObjectCollisionPair();

}

#endif
