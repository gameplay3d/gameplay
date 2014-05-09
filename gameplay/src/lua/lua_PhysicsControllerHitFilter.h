#ifndef LUA_PHYSICSCONTROLLERHITFILTER_H_
#define LUA_PHYSICSCONTROLLERHITFILTER_H_

namespace gameplay
{

// Lua bindings for PhysicsController::HitFilter.
int lua_PhysicsControllerHitFilter__gc(lua_State* state);
int lua_PhysicsControllerHitFilter__init(lua_State* state);
int lua_PhysicsControllerHitFilter_filter(lua_State* state);
int lua_PhysicsControllerHitFilter_hit(lua_State* state);

void luaRegister_PhysicsControllerHitFilter();

}

#endif
