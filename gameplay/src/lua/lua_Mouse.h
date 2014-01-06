#ifndef LUA_MOUSE_H_
#define LUA_MOUSE_H_

namespace gameplay
{

// Lua bindings for Mouse.
int lua_Mouse__gc(lua_State* state);

void luaRegister_Mouse();

}

#endif
