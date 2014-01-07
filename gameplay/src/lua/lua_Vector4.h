#ifndef LUA_VECTOR4_H_
#define LUA_VECTOR4_H_

namespace gameplay
{

// Lua bindings for Vector4.
int lua_Vector4__gc(lua_State* state);
int lua_Vector4__init(lua_State* state);
int lua_Vector4_add(lua_State* state);
int lua_Vector4_clamp(lua_State* state);
int lua_Vector4_distance(lua_State* state);
int lua_Vector4_distanceSquared(lua_State* state);
int lua_Vector4_dot(lua_State* state);
int lua_Vector4_isOne(lua_State* state);
int lua_Vector4_isZero(lua_State* state);
int lua_Vector4_length(lua_State* state);
int lua_Vector4_lengthSquared(lua_State* state);
int lua_Vector4_negate(lua_State* state);
int lua_Vector4_normalize(lua_State* state);
int lua_Vector4_scale(lua_State* state);
int lua_Vector4_set(lua_State* state);
int lua_Vector4_static_add(lua_State* state);
int lua_Vector4_static_angle(lua_State* state);
int lua_Vector4_static_clamp(lua_State* state);
int lua_Vector4_static_dot(lua_State* state);
int lua_Vector4_static_fromColor(lua_State* state);
int lua_Vector4_static_one(lua_State* state);
int lua_Vector4_static_subtract(lua_State* state);
int lua_Vector4_static_unitW(lua_State* state);
int lua_Vector4_static_unitX(lua_State* state);
int lua_Vector4_static_unitY(lua_State* state);
int lua_Vector4_static_unitZ(lua_State* state);
int lua_Vector4_static_zero(lua_State* state);
int lua_Vector4_subtract(lua_State* state);
int lua_Vector4_w(lua_State* state);
int lua_Vector4_x(lua_State* state);
int lua_Vector4_y(lua_State* state);
int lua_Vector4_z(lua_State* state);

void luaRegister_Vector4();

}

#endif
