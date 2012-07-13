#ifndef LUA_GAMEPADBUTTONSTATE_H_
#define LUA_GAMEPADBUTTONSTATE_H_

#include "Gamepad.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Gamepad::ButtonState.
Gamepad::ButtonState lua_enumFromString_GamepadButtonState(const char* s);
const char* lua_stringFromEnum_GamepadButtonState(Gamepad::ButtonState e);

}

#endif
