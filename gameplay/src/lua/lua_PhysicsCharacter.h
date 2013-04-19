#ifndef LUA_PHYSICSCHARACTER_H_
#define LUA_PHYSICSCHARACTER_H_

namespace gameplay
{

// Lua bindings for PhysicsCharacter.
int lua_PhysicsCharacter_addCollisionListener(lua_State* state);
int lua_PhysicsCharacter_asCharacter(lua_State* state);
int lua_PhysicsCharacter_asGhostObject(lua_State* state);
int lua_PhysicsCharacter_asRigidBody(lua_State* state);
int lua_PhysicsCharacter_asVehicle(lua_State* state);
int lua_PhysicsCharacter_asVehicleWheel(lua_State* state);
int lua_PhysicsCharacter_collidesWith(lua_State* state);
int lua_PhysicsCharacter_getCollisionShape(lua_State* state);
int lua_PhysicsCharacter_getCurrentVelocity(lua_State* state);
int lua_PhysicsCharacter_getMaxSlopeAngle(lua_State* state);
int lua_PhysicsCharacter_getMaxStepHeight(lua_State* state);
int lua_PhysicsCharacter_getNode(lua_State* state);
int lua_PhysicsCharacter_getShapeType(lua_State* state);
int lua_PhysicsCharacter_getType(lua_State* state);
int lua_PhysicsCharacter_isDynamic(lua_State* state);
int lua_PhysicsCharacter_isEnabled(lua_State* state);
int lua_PhysicsCharacter_isKinematic(lua_State* state);
int lua_PhysicsCharacter_isPhysicsEnabled(lua_State* state);
int lua_PhysicsCharacter_isStatic(lua_State* state);
int lua_PhysicsCharacter_jump(lua_State* state);
int lua_PhysicsCharacter_removeCollisionListener(lua_State* state);
int lua_PhysicsCharacter_rotate(lua_State* state);
int lua_PhysicsCharacter_setEnabled(lua_State* state);
int lua_PhysicsCharacter_setForwardVelocity(lua_State* state);
int lua_PhysicsCharacter_setMaxSlopeAngle(lua_State* state);
int lua_PhysicsCharacter_setMaxStepHeight(lua_State* state);
int lua_PhysicsCharacter_setPhysicsEnabled(lua_State* state);
int lua_PhysicsCharacter_setRightVelocity(lua_State* state);
int lua_PhysicsCharacter_setRotation(lua_State* state);
int lua_PhysicsCharacter_setVelocity(lua_State* state);
int lua_PhysicsCharacter_transformChanged(lua_State* state);

void luaRegister_PhysicsCharacter();

}

#endif
