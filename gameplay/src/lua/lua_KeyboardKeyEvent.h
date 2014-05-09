#ifndef LUA_KEYBOARDKEYEVENT_H_
#define LUA_KEYBOARDKEYEVENT_H_

#include "Keyboard.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Keyboard::KeyEvent.
Keyboard::KeyEvent lua_enumFromString_KeyboardKeyEvent(const char* s);
const char* lua_stringFromEnum_KeyboardKeyEvent(Keyboard::KeyEvent e);

}

#endif
