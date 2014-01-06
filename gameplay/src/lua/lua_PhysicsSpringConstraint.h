#ifndef LUA_PHYSICSSPRINGCONSTRAINT_H_
#define LUA_PHYSICSSPRINGCONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsSpringConstraint.
int lua_PhysicsSpringConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsSpringConstraint_getRotationOffsetA(lua_State* state);
int lua_PhysicsSpringConstraint_getRotationOffsetB(lua_State* state);
int lua_PhysicsSpringConstraint_getTranslationOffsetA(lua_State* state);
int lua_PhysicsSpringConstraint_getTranslationOffsetB(lua_State* state);
int lua_PhysicsSpringConstraint_isEnabled(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularDampingX(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularDampingY(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularDampingZ(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularLowerLimit(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularStrengthX(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularStrengthY(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularStrengthZ(lua_State* state);
int lua_PhysicsSpringConstraint_setAngularUpperLimit(lua_State* state);
int lua_PhysicsSpringConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsSpringConstraint_setEnabled(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearDampingX(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearDampingY(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearDampingZ(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearLowerLimit(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearStrengthX(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearStrengthY(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearStrengthZ(lua_State* state);
int lua_PhysicsSpringConstraint_setLinearUpperLimit(lua_State* state);
int lua_PhysicsSpringConstraint_setRotationOffsetA(lua_State* state);
int lua_PhysicsSpringConstraint_setRotationOffsetB(lua_State* state);
int lua_PhysicsSpringConstraint_setTranslationOffsetA(lua_State* state);
int lua_PhysicsSpringConstraint_setTranslationOffsetB(lua_State* state);
int lua_PhysicsSpringConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsSpringConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsSpringConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsSpringConstraint();

}

#endif
