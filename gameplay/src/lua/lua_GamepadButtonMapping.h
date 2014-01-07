#ifndef LUA_GAMEPADBUTTONMAPPING_H_
#define LUA_GAMEPADBUTTONMAPPING_H_

#include "Gamepad.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Gamepad::ButtonMapping.
Gamepad::ButtonMapping lua_enumFromString_GamepadButtonMapping(const char* s);
const char* lua_stringFromEnum_GamepadButtonMapping(Gamepad::ButtonMapping e);

}

#endif
