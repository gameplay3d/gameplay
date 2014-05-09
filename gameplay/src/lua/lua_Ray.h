#ifndef LUA_RAY_H_
#define LUA_RAY_H_

namespace gameplay
{

// Lua bindings for Ray.
int lua_Ray__gc(lua_State* state);
int lua_Ray__init(lua_State* state);
int lua_Ray_getDirection(lua_State* state);
int lua_Ray_getOrigin(lua_State* state);
int lua_Ray_intersects(lua_State* state);
int lua_Ray_set(lua_State* state);
int lua_Ray_setDirection(lua_State* state);
int lua_Ray_setOrigin(lua_State* state);
int lua_Ray_static_INTERSECTS_NONE(lua_State* state);
int lua_Ray_transform(lua_State* state);

void luaRegister_Ray();

}

#endif
