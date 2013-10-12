#ifndef LUA_CONTROLAUTOSIZE_H_
#define LUA_CONTROLAUTOSIZE_H_

#include "Control.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Control::AutoSize.
Control::AutoSize lua_enumFromString_ControlAutoSize(const char* s);
const char* lua_stringFromEnum_ControlAutoSize(Control::AutoSize e);

}

#endif
