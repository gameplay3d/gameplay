#ifndef LUA_KEYBOARD_H_
#define LUA_KEYBOARD_H_

namespace gameplay
{

// Lua bindings for Keyboard.
int lua_Keyboard__gc(lua_State* state);

void luaRegister_Keyboard();

}

#endif
