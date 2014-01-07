#ifndef LUA_PLANE_H_
#define LUA_PLANE_H_

namespace gameplay
{

// Lua bindings for Plane.
int lua_Plane__gc(lua_State* state);
int lua_Plane__init(lua_State* state);
int lua_Plane_distance(lua_State* state);
int lua_Plane_getDistance(lua_State* state);
int lua_Plane_getNormal(lua_State* state);
int lua_Plane_intersects(lua_State* state);
int lua_Plane_isParallel(lua_State* state);
int lua_Plane_set(lua_State* state);
int lua_Plane_setDistance(lua_State* state);
int lua_Plane_setNormal(lua_State* state);
int lua_Plane_static_INTERSECTS_BACK(lua_State* state);
int lua_Plane_static_INTERSECTS_FRONT(lua_State* state);
int lua_Plane_static_INTERSECTS_INTERSECTING(lua_State* state);
int lua_Plane_static_intersection(lua_State* state);
int lua_Plane_transform(lua_State* state);

void luaRegister_Plane();

}

#endif
