#ifndef LUA_PHYSICSVEHICLEWHEEL_H_
#define LUA_PHYSICSVEHICLEWHEEL_H_

namespace gameplay
{

// Lua bindings for PhysicsVehicleWheel.
int lua_PhysicsVehicleWheel_addCollisionListener(lua_State* state);
int lua_PhysicsVehicleWheel_asCharacter(lua_State* state);
int lua_PhysicsVehicleWheel_asGhostObject(lua_State* state);
int lua_PhysicsVehicleWheel_asRigidBody(lua_State* state);
int lua_PhysicsVehicleWheel_asVehicle(lua_State* state);
int lua_PhysicsVehicleWheel_asVehicleWheel(lua_State* state);
int lua_PhysicsVehicleWheel_collidesWith(lua_State* state);
int lua_PhysicsVehicleWheel_getCollisionShape(lua_State* state);
int lua_PhysicsVehicleWheel_getFrictionBreakout(lua_State* state);
int lua_PhysicsVehicleWheel_getNode(lua_State* state);
int lua_PhysicsVehicleWheel_getRollInfluence(lua_State* state);
int lua_PhysicsVehicleWheel_getShapeType(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutConnectionOffset(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutDampingCompression(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutDampingRelaxation(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutForceMax(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutRestLength(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutStiffness(lua_State* state);
int lua_PhysicsVehicleWheel_getStrutTravelMax(lua_State* state);
int lua_PhysicsVehicleWheel_getType(lua_State* state);
int lua_PhysicsVehicleWheel_getWheelAxle(lua_State* state);
int lua_PhysicsVehicleWheel_getWheelDirection(lua_State* state);
int lua_PhysicsVehicleWheel_getWheelRadius(lua_State* state);
int lua_PhysicsVehicleWheel_isDynamic(lua_State* state);
int lua_PhysicsVehicleWheel_isEnabled(lua_State* state);
int lua_PhysicsVehicleWheel_isKinematic(lua_State* state);
int lua_PhysicsVehicleWheel_isStatic(lua_State* state);
int lua_PhysicsVehicleWheel_isSteerable(lua_State* state);
int lua_PhysicsVehicleWheel_removeCollisionListener(lua_State* state);
int lua_PhysicsVehicleWheel_setEnabled(lua_State* state);
int lua_PhysicsVehicleWheel_setFrictionBreakout(lua_State* state);
int lua_PhysicsVehicleWheel_setRollInfluence(lua_State* state);
int lua_PhysicsVehicleWheel_setSteerable(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutConnectionOffset(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutDampingCompression(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutDampingRelaxation(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutForceMax(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutRestLength(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutStiffness(lua_State* state);
int lua_PhysicsVehicleWheel_setStrutTravelMax(lua_State* state);
int lua_PhysicsVehicleWheel_setWheelAxle(lua_State* state);
int lua_PhysicsVehicleWheel_setWheelDirection(lua_State* state);
int lua_PhysicsVehicleWheel_setWheelRadius(lua_State* state);
int lua_PhysicsVehicleWheel_transform(lua_State* state);

void luaRegister_PhysicsVehicleWheel();

}

#endif
