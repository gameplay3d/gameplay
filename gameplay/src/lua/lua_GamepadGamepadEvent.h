#ifndef LUA_GAMEPADGAMEPADEVENT_H_
#define LUA_GAMEPADGAMEPADEVENT_H_

#include "Gamepad.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Gamepad::GamepadEvent.
Gamepad::GamepadEvent lua_enumFromString_GamepadGamepadEvent(const char* s);
const char* lua_stringFromEnum_GamepadGamepadEvent(Gamepad::GamepadEvent e);

}

#endif
