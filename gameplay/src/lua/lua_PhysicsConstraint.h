#ifndef LUA_PHYSICSCONSTRAINT_H_
#define LUA_PHYSICSCONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsConstraint.
int lua_PhysicsConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsConstraint_isEnabled(lua_State* state);
int lua_PhysicsConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsConstraint_setEnabled(lua_State* state);
int lua_PhysicsConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsConstraint();

}

#endif
