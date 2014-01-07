#ifndef LUA_CONTROLALIGNMENT_H_
#define LUA_CONTROLALIGNMENT_H_

#include "Control.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Control::Alignment.
Control::Alignment lua_enumFromString_ControlAlignment(const char* s);
const char* lua_stringFromEnum_ControlAlignment(Control::Alignment e);

}

#endif
