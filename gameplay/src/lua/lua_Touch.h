#ifndef LUA_TOUCH_H_
#define LUA_TOUCH_H_

namespace gameplay
{

// Lua bindings for Touch.
int lua_Touch__gc(lua_State* state);
int lua_Touch_static_MAX_TOUCH_POINTS(lua_State* state);

void luaRegister_Touch();

}

#endif
