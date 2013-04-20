#ifndef LUA_PHYSICSCOLLISIONOBJECT_H_
#define LUA_PHYSICSCOLLISIONOBJECT_H_

namespace gameplay
{

// Lua bindings for PhysicsCollisionObject.
int lua_PhysicsCollisionObject__gc(lua_State* state);
int lua_PhysicsCollisionObject_addCollisionListener(lua_State* state);
int lua_PhysicsCollisionObject_asCharacter(lua_State* state);
int lua_PhysicsCollisionObject_asGhostObject(lua_State* state);
int lua_PhysicsCollisionObject_asRigidBody(lua_State* state);
int lua_PhysicsCollisionObject_asVehicle(lua_State* state);
int lua_PhysicsCollisionObject_asVehicleWheel(lua_State* state);
int lua_PhysicsCollisionObject_collidesWith(lua_State* state);
int lua_PhysicsCollisionObject_getCollisionShape(lua_State* state);
int lua_PhysicsCollisionObject_getNode(lua_State* state);
int lua_PhysicsCollisionObject_getShapeType(lua_State* state);
int lua_PhysicsCollisionObject_getType(lua_State* state);
int lua_PhysicsCollisionObject_isDynamic(lua_State* state);
int lua_PhysicsCollisionObject_isEnabled(lua_State* state);
int lua_PhysicsCollisionObject_isKinematic(lua_State* state);
int lua_PhysicsCollisionObject_isStatic(lua_State* state);
int lua_PhysicsCollisionObject_removeCollisionListener(lua_State* state);
int lua_PhysicsCollisionObject_setEnabled(lua_State* state);

void luaRegister_PhysicsCollisionObject();

}

#endif
