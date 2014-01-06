#ifndef LUA_GESTUREGESTUREEVENT_H_
#define LUA_GESTUREGESTUREEVENT_H_

#include "Gesture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Gesture::GestureEvent.
Gesture::GestureEvent lua_enumFromString_GestureGestureEvent(const char* s);
const char* lua_stringFromEnum_GestureGestureEvent(Gesture::GestureEvent e);

}

#endif
