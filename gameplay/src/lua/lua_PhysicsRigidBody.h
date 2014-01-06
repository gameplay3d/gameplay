#ifndef LUA_PHYSICSRIGIDBODY_H_
#define LUA_PHYSICSRIGIDBODY_H_

namespace gameplay
{

// Lua bindings for PhysicsRigidBody.
int lua_PhysicsRigidBody_addCollisionListener(lua_State* state);
int lua_PhysicsRigidBody_applyForce(lua_State* state);
int lua_PhysicsRigidBody_applyImpulse(lua_State* state);
int lua_PhysicsRigidBody_applyTorque(lua_State* state);
int lua_PhysicsRigidBody_applyTorqueImpulse(lua_State* state);
int lua_PhysicsRigidBody_asCharacter(lua_State* state);
int lua_PhysicsRigidBody_asGhostObject(lua_State* state);
int lua_PhysicsRigidBody_asRigidBody(lua_State* state);
int lua_PhysicsRigidBody_asVehicle(lua_State* state);
int lua_PhysicsRigidBody_asVehicleWheel(lua_State* state);
int lua_PhysicsRigidBody_collidesWith(lua_State* state);
int lua_PhysicsRigidBody_getAngularDamping(lua_State* state);
int lua_PhysicsRigidBody_getAngularFactor(lua_State* state);
int lua_PhysicsRigidBody_getAngularVelocity(lua_State* state);
int lua_PhysicsRigidBody_getAnisotropicFriction(lua_State* state);
int lua_PhysicsRigidBody_getCollisionShape(lua_State* state);
int lua_PhysicsRigidBody_getFriction(lua_State* state);
int lua_PhysicsRigidBody_getGravity(lua_State* state);
int lua_PhysicsRigidBody_getHeight(lua_State* state);
int lua_PhysicsRigidBody_getLinearDamping(lua_State* state);
int lua_PhysicsRigidBody_getLinearFactor(lua_State* state);
int lua_PhysicsRigidBody_getLinearVelocity(lua_State* state);
int lua_PhysicsRigidBody_getMass(lua_State* state);
int lua_PhysicsRigidBody_getNode(lua_State* state);
int lua_PhysicsRigidBody_getRestitution(lua_State* state);
int lua_PhysicsRigidBody_getShapeType(lua_State* state);
int lua_PhysicsRigidBody_getType(lua_State* state);
int lua_PhysicsRigidBody_isDynamic(lua_State* state);
int lua_PhysicsRigidBody_isEnabled(lua_State* state);
int lua_PhysicsRigidBody_isKinematic(lua_State* state);
int lua_PhysicsRigidBody_isStatic(lua_State* state);
int lua_PhysicsRigidBody_removeCollisionListener(lua_State* state);
int lua_PhysicsRigidBody_setAngularFactor(lua_State* state);
int lua_PhysicsRigidBody_setAngularVelocity(lua_State* state);
int lua_PhysicsRigidBody_setAnisotropicFriction(lua_State* state);
int lua_PhysicsRigidBody_setDamping(lua_State* state);
int lua_PhysicsRigidBody_setEnabled(lua_State* state);
int lua_PhysicsRigidBody_setFriction(lua_State* state);
int lua_PhysicsRigidBody_setGravity(lua_State* state);
int lua_PhysicsRigidBody_setKinematic(lua_State* state);
int lua_PhysicsRigidBody_setLinearFactor(lua_State* state);
int lua_PhysicsRigidBody_setLinearVelocity(lua_State* state);
int lua_PhysicsRigidBody_setRestitution(lua_State* state);

void luaRegister_PhysicsRigidBody();

}

#endif
