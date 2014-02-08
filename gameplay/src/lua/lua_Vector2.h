#ifndef LUA_VECTOR2_H_
#define LUA_VECTOR2_H_

namespace gameplay
{

// Lua bindings for Vector2.
int lua_Vector2__gc(lua_State* state);
int lua_Vector2__init(lua_State* state);
int lua_Vector2_add(lua_State* state);
int lua_Vector2_clamp(lua_State* state);
int lua_Vector2_distance(lua_State* state);
int lua_Vector2_distanceSquared(lua_State* state);
int lua_Vector2_dot(lua_State* state);
int lua_Vector2_isOne(lua_State* state);
int lua_Vector2_isZero(lua_State* state);
int lua_Vector2_length(lua_State* state);
int lua_Vector2_lengthSquared(lua_State* state);
int lua_Vector2_negate(lua_State* state);
int lua_Vector2_normalize(lua_State* state);
int lua_Vector2_rotate(lua_State* state);
int lua_Vector2_scale(lua_State* state);
int lua_Vector2_set(lua_State* state);
int lua_Vector2_smooth(lua_State* state);
int lua_Vector2_static_add(lua_State* state);
int lua_Vector2_static_angle(lua_State* state);
int lua_Vector2_static_clamp(lua_State* state);
int lua_Vector2_static_dot(lua_State* state);
int lua_Vector2_static_one(lua_State* state);
int lua_Vector2_static_subtract(lua_State* state);
int lua_Vector2_static_unitX(lua_State* state);
int lua_Vector2_static_unitY(lua_State* state);
int lua_Vector2_static_zero(lua_State* state);
int lua_Vector2_subtract(lua_State* state);
int lua_Vector2_x(lua_State* state);
int lua_Vector2_y(lua_State* state);

void luaRegister_Vector2();

}

#endif
