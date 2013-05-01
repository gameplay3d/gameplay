#include "Base.h"
#include "lua_GestureGestureEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GestureGestureEvent_GESTURE_TAP = "GESTURE_TAP";
static const char* luaEnumString_GestureGestureEvent_GESTURE_SWIPE = "GESTURE_SWIPE";
static const char* luaEnumString_GestureGestureEvent_GESTURE_PINCH = "GESTURE_PINCH";
static const char* luaEnumString_GestureGestureEvent_GESTURE_ANY_SUPPORTED = "GESTURE_ANY_SUPPORTED";

Gesture::GestureEvent lua_enumFromString_GestureGestureEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_TAP) == 0)
        return Gesture::GESTURE_TAP;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_SWIPE) == 0)
        return Gesture::GESTURE_SWIPE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_PINCH) == 0)
        return Gesture::GESTURE_PINCH;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_ANY_SUPPORTED) == 0)
        return Gesture::GESTURE_ANY_SUPPORTED;
    return Gesture::GESTURE_TAP;
}

const char* lua_stringFromEnum_GestureGestureEvent(Gesture::GestureEvent e)
{
    if (e == Gesture::GESTURE_TAP)
        return luaEnumString_GestureGestureEvent_GESTURE_TAP;
    if (e == Gesture::GESTURE_SWIPE)
        return luaEnumString_GestureGestureEvent_GESTURE_SWIPE;
    if (e == Gesture::GESTURE_PINCH)
        return luaEnumString_GestureGestureEvent_GESTURE_PINCH;
    if (e == Gesture::GESTURE_ANY_SUPPORTED)
        return luaEnumString_GestureGestureEvent_GESTURE_ANY_SUPPORTED;
    return enumStringEmpty;
}

}

