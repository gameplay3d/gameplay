#ifndef LUA_VECTOR3_H_
#define LUA_VECTOR3_H_

namespace gameplay
{

// Lua bindings for Vector3.
int lua_Vector3__gc(lua_State* state);
int lua_Vector3__init(lua_State* state);
int lua_Vector3_add(lua_State* state);
int lua_Vector3_clamp(lua_State* state);
int lua_Vector3_cross(lua_State* state);
int lua_Vector3_distance(lua_State* state);
int lua_Vector3_distanceSquared(lua_State* state);
int lua_Vector3_dot(lua_State* state);
int lua_Vector3_isOne(lua_State* state);
int lua_Vector3_isZero(lua_State* state);
int lua_Vector3_length(lua_State* state);
int lua_Vector3_lengthSquared(lua_State* state);
int lua_Vector3_negate(lua_State* state);
int lua_Vector3_normalize(lua_State* state);
int lua_Vector3_scale(lua_State* state);
int lua_Vector3_set(lua_State* state);
int lua_Vector3_smooth(lua_State* state);
int lua_Vector3_static_add(lua_State* state);
int lua_Vector3_static_angle(lua_State* state);
int lua_Vector3_static_clamp(lua_State* state);
int lua_Vector3_static_cross(lua_State* state);
int lua_Vector3_static_dot(lua_State* state);
int lua_Vector3_static_fromColor(lua_State* state);
int lua_Vector3_static_one(lua_State* state);
int lua_Vector3_static_subtract(lua_State* state);
int lua_Vector3_static_unitX(lua_State* state);
int lua_Vector3_static_unitY(lua_State* state);
int lua_Vector3_static_unitZ(lua_State* state);
int lua_Vector3_static_zero(lua_State* state);
int lua_Vector3_subtract(lua_State* state);
int lua_Vector3_x(lua_State* state);
int lua_Vector3_y(lua_State* state);
int lua_Vector3_z(lua_State* state);

void luaRegister_Vector3();

}

#endif
