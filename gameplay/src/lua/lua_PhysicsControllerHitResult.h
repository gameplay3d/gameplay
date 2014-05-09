#ifndef LUA_PHYSICSCONTROLLERHITRESULT_H_
#define LUA_PHYSICSCONTROLLERHITRESULT_H_

namespace gameplay
{

// Lua bindings for PhysicsController::HitResult.
int lua_PhysicsControllerHitResult__gc(lua_State* state);
int lua_PhysicsControllerHitResult__init(lua_State* state);
int lua_PhysicsControllerHitResult_fraction(lua_State* state);
int lua_PhysicsControllerHitResult_normal(lua_State* state);
int lua_PhysicsControllerHitResult_object(lua_State* state);
int lua_PhysicsControllerHitResult_point(lua_State* state);

void luaRegister_PhysicsControllerHitResult();

}

#endif
