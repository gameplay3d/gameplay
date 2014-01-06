#ifndef LUA_PHYSICSHINGECONSTRAINT_H_
#define LUA_PHYSICSHINGECONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsHingeConstraint.
int lua_PhysicsHingeConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsHingeConstraint_isEnabled(lua_State* state);
int lua_PhysicsHingeConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsHingeConstraint_setEnabled(lua_State* state);
int lua_PhysicsHingeConstraint_setLimits(lua_State* state);
int lua_PhysicsHingeConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsHingeConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsHingeConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsHingeConstraint();

}

#endif
