#ifndef LUA_CURVE_H_
#define LUA_CURVE_H_

namespace gameplay
{

// Lua bindings for Curve.
int lua_Curve__gc(lua_State* state);
int lua_Curve_addRef(lua_State* state);
int lua_Curve_evaluate(lua_State* state);
int lua_Curve_getComponentCount(lua_State* state);
int lua_Curve_getEndTime(lua_State* state);
int lua_Curve_getPointCount(lua_State* state);
int lua_Curve_getRefCount(lua_State* state);
int lua_Curve_getStartTime(lua_State* state);
int lua_Curve_release(lua_State* state);
int lua_Curve_setPoint(lua_State* state);
int lua_Curve_setTangent(lua_State* state);
int lua_Curve_static_create(lua_State* state);
int lua_Curve_static_lerp(lua_State* state);

void luaRegister_Curve();

}

#endif
