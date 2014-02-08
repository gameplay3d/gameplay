#ifndef LUA_CONTROLSTATE_H_
#define LUA_CONTROLSTATE_H_

#include "Control.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Control::State.
Control::State lua_enumFromString_ControlState(const char* s);
const char* lua_stringFromEnum_ControlState(Control::State e);

}

#endif
