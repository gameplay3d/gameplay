#ifndef LUA_PHYSICSSOCKETCONSTRAINT_H_
#define LUA_PHYSICSSOCKETCONSTRAINT_H_

namespace gameplay
{

// Lua bindings for PhysicsSocketConstraint.
int lua_PhysicsSocketConstraint_getBreakingImpulse(lua_State* state);
int lua_PhysicsSocketConstraint_isEnabled(lua_State* state);
int lua_PhysicsSocketConstraint_setBreakingImpulse(lua_State* state);
int lua_PhysicsSocketConstraint_setEnabled(lua_State* state);
int lua_PhysicsSocketConstraint_static_centerOfMassMidpoint(lua_State* state);
int lua_PhysicsSocketConstraint_static_getRotationOffset(lua_State* state);
int lua_PhysicsSocketConstraint_static_getTranslationOffset(lua_State* state);

void luaRegister_PhysicsSocketConstraint();

}

#endif
