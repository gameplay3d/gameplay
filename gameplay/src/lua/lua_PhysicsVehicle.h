#ifndef LUA_PHYSICSVEHICLE_H_
#define LUA_PHYSICSVEHICLE_H_

namespace gameplay
{

// Lua bindings for PhysicsVehicle.
int lua_PhysicsVehicle_addCollisionListener(lua_State* state);
int lua_PhysicsVehicle_addWheel(lua_State* state);
int lua_PhysicsVehicle_asCharacter(lua_State* state);
int lua_PhysicsVehicle_asGhostObject(lua_State* state);
int lua_PhysicsVehicle_asRigidBody(lua_State* state);
int lua_PhysicsVehicle_asVehicle(lua_State* state);
int lua_PhysicsVehicle_asVehicleWheel(lua_State* state);
int lua_PhysicsVehicle_collidesWith(lua_State* state);
int lua_PhysicsVehicle_getBoostGain(lua_State* state);
int lua_PhysicsVehicle_getBoostSpeed(lua_State* state);
int lua_PhysicsVehicle_getBrakedownFull(lua_State* state);
int lua_PhysicsVehicle_getBrakedownStart(lua_State* state);
int lua_PhysicsVehicle_getBrakingForce(lua_State* state);
int lua_PhysicsVehicle_getCollisionShape(lua_State* state);
int lua_PhysicsVehicle_getDownforce(lua_State* state);
int lua_PhysicsVehicle_getDrivedownFull(lua_State* state);
int lua_PhysicsVehicle_getDrivedownStart(lua_State* state);
int lua_PhysicsVehicle_getDrivingForce(lua_State* state);
int lua_PhysicsVehicle_getNode(lua_State* state);
int lua_PhysicsVehicle_getNumWheels(lua_State* state);
int lua_PhysicsVehicle_getRigidBody(lua_State* state);
int lua_PhysicsVehicle_getShapeType(lua_State* state);
int lua_PhysicsVehicle_getSpeedKph(lua_State* state);
int lua_PhysicsVehicle_getSpeedSmoothKph(lua_State* state);
int lua_PhysicsVehicle_getSteerdownGain(lua_State* state);
int lua_PhysicsVehicle_getSteerdownSpeed(lua_State* state);
int lua_PhysicsVehicle_getSteeringGain(lua_State* state);
int lua_PhysicsVehicle_getType(lua_State* state);
int lua_PhysicsVehicle_getWheel(lua_State* state);
int lua_PhysicsVehicle_isDynamic(lua_State* state);
int lua_PhysicsVehicle_isEnabled(lua_State* state);
int lua_PhysicsVehicle_isKinematic(lua_State* state);
int lua_PhysicsVehicle_isStatic(lua_State* state);
int lua_PhysicsVehicle_removeCollisionListener(lua_State* state);
int lua_PhysicsVehicle_reset(lua_State* state);
int lua_PhysicsVehicle_setBoost(lua_State* state);
int lua_PhysicsVehicle_setBrakedown(lua_State* state);
int lua_PhysicsVehicle_setBrakingForce(lua_State* state);
int lua_PhysicsVehicle_setDownforce(lua_State* state);
int lua_PhysicsVehicle_setDrivedown(lua_State* state);
int lua_PhysicsVehicle_setDrivingForce(lua_State* state);
int lua_PhysicsVehicle_setEnabled(lua_State* state);
int lua_PhysicsVehicle_setSteerdown(lua_State* state);
int lua_PhysicsVehicle_setSteeringGain(lua_State* state);
int lua_PhysicsVehicle_update(lua_State* state);

void luaRegister_PhysicsVehicle();

}

#endif
