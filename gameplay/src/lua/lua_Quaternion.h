#ifndef LUA_QUATERNION_H_
#define LUA_QUATERNION_H_

namespace gameplay
{

// Lua bindings for Quaternion.
int lua_Quaternion__gc(lua_State* state);
int lua_Quaternion__init(lua_State* state);
int lua_Quaternion_conjugate(lua_State* state);
int lua_Quaternion_inverse(lua_State* state);
int lua_Quaternion_isIdentity(lua_State* state);
int lua_Quaternion_isZero(lua_State* state);
int lua_Quaternion_multiply(lua_State* state);
int lua_Quaternion_normalize(lua_State* state);
int lua_Quaternion_set(lua_State* state);
int lua_Quaternion_setIdentity(lua_State* state);
int lua_Quaternion_static_createFromAxisAngle(lua_State* state);
int lua_Quaternion_static_createFromRotationMatrix(lua_State* state);
int lua_Quaternion_static_identity(lua_State* state);
int lua_Quaternion_static_lerp(lua_State* state);
int lua_Quaternion_static_multiply(lua_State* state);
int lua_Quaternion_static_slerp(lua_State* state);
int lua_Quaternion_static_squad(lua_State* state);
int lua_Quaternion_static_zero(lua_State* state);
int lua_Quaternion_toAxisAngle(lua_State* state);
int lua_Quaternion_w(lua_State* state);
int lua_Quaternion_x(lua_State* state);
int lua_Quaternion_y(lua_State* state);
int lua_Quaternion_z(lua_State* state);

void luaRegister_Quaternion();

}

#endif
