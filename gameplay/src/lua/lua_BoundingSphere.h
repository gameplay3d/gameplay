#ifndef LUA_BOUNDINGSPHERE_H_
#define LUA_BOUNDINGSPHERE_H_

namespace gameplay
{

// Lua bindings for BoundingSphere.
int lua_BoundingSphere__gc(lua_State* state);
int lua_BoundingSphere__init(lua_State* state);
int lua_BoundingSphere_center(lua_State* state);
int lua_BoundingSphere_intersects(lua_State* state);
int lua_BoundingSphere_isEmpty(lua_State* state);
int lua_BoundingSphere_merge(lua_State* state);
int lua_BoundingSphere_radius(lua_State* state);
int lua_BoundingSphere_set(lua_State* state);
int lua_BoundingSphere_static_empty(lua_State* state);
int lua_BoundingSphere_transform(lua_State* state);

void luaRegister_BoundingSphere();

}

#endif
