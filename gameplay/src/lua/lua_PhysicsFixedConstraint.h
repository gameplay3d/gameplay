#ifndef LUA_PHYSICSFIXEDCONSTRAINT_H_
#define LUA_PHYSICSFIXEDCONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsFixedConstraint.
int lua_PhysicsFixedConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsFixedConstraint_getRotationOffsetA(lua_State* state);
int lua_PhysicsFixedConstraint_getRotationOffsetB(lua_State* state);
int lua_PhysicsFixedConstraint_getTranslationOffsetA(lua_State* state);
int lua_PhysicsFixedConstraint_getTranslationOffsetB(lua_State* state);
int lua_PhysicsFixedConstraint_isEnabled(lua_State* state);
int lua_PhysicsFixedConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsFixedConstraint_setEnabled(lua_State* state);
int lua_PhysicsFixedConstraint_setRotationOffsetA(lua_State* state);
int lua_PhysicsFixedConstraint_setRotationOffsetB(lua_State* state);
int lua_PhysicsFixedConstraint_setTranslationOffsetA(lua_State* state);
int lua_PhysicsFixedConstraint_setTranslationOffsetB(lua_State* state);
int lua_PhysicsFixedConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsFixedConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsFixedConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsFixedConstraint();

}

#endif
