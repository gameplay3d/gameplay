#ifndef LUA_PHYSICSGENERICCONSTRAINT_H_
#define LUA_PHYSICSGENERICCONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsGenericConstraint.
int lua_PhysicsGenericConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsGenericConstraint_getRotationOffsetA(lua_State* state);
int lua_PhysicsGenericConstraint_getRotationOffsetB(lua_State* state);
int lua_PhysicsGenericConstraint_getTranslationOffsetA(lua_State* state);
int lua_PhysicsGenericConstraint_getTranslationOffsetB(lua_State* state);
int lua_PhysicsGenericConstraint_isEnabled(lua_State* state);
int lua_PhysicsGenericConstraint_setAngularLowerLimit(lua_State* state);
int lua_PhysicsGenericConstraint_setAngularUpperLimit(lua_State* state);
int lua_PhysicsGenericConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsGenericConstraint_setEnabled(lua_State* state);
int lua_PhysicsGenericConstraint_setLinearLowerLimit(lua_State* state);
int lua_PhysicsGenericConstraint_setLinearUpperLimit(lua_State* state);
int lua_PhysicsGenericConstraint_setRotationOffsetA(lua_State* state);
int lua_PhysicsGenericConstraint_setRotationOffsetB(lua_State* state);
int lua_PhysicsGenericConstraint_setTranslationOffsetA(lua_State* state);
int lua_PhysicsGenericConstraint_setTranslationOffsetB(lua_State* state);
int lua_PhysicsGenericConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsGenericConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsGenericConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsGenericConstraint();

}

#endif
