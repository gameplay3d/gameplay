#ifndef LUA_KEYBOARDKEY_H_
#define LUA_KEYBOARDKEY_H_

#include "Keyboard.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Keyboard::Key.
Keyboard::Key lua_enumFromString_KeyboardKey(const char* s);
const char* lua_stringFromEnum_KeyboardKey(Keyboard::Key e);

}

#endif
