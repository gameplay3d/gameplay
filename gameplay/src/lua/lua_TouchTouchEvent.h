#ifndef LUA_TOUCHTOUCHEVENT_H_
#define LUA_TOUCHTOUCHEVENT_H_

#include "Touch.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Touch::TouchEvent.
Touch::TouchEvent lua_enumFromString_TouchTouchEvent(const char* s);
const char* lua_stringFromEnum_TouchTouchEvent(Touch::TouchEvent e);

}

#endif
