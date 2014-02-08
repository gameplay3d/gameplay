#ifndef LUA_MOUSEMOUSEEVENT_H_
#define LUA_MOUSEMOUSEEVENT_H_

#include "Mouse.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Mouse::MouseEvent.
Mouse::MouseEvent lua_enumFromString_MouseMouseEvent(const char* s);
const char* lua_stringFromEnum_MouseMouseEvent(Mouse::MouseEvent e);

}

#endif
