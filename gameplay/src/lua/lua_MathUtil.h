#ifndef LUA_MATHUTIL_H_
#define LUA_MATHUTIL_H_

namespace gameplay
{

// Lua bindings for MathUtil.
int lua_MathUtil__gc(lua_State* state);
int lua_MathUtil_static_smooth(lua_State* state);

void luaRegister_MathUtil();

}

#endif
