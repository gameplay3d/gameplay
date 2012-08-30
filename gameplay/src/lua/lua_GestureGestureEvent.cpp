#include "Base.h"
#include "lua_GestureGestureEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GestureGestureEvent_GESTURE_SWIPE = "GESTURE_SWIPE";
static const char* luaEnumString_GestureGestureEvent_GESTURE_PINCH = "GESTURE_PINCH";
static const char* luaEnumString_GestureGestureEvent_GESTURE_ROTATE = "GESTURE_ROTATE";
static const char* luaEnumString_GestureGestureEvent_GESTURE_TAP = "GESTURE_TAP";
static const char* luaEnumString_GestureGestureEvent_GESTURE_TAP_DOUBLE = "GESTURE_TAP_DOUBLE";
static const char* luaEnumString_GestureGestureEvent_GESTURE_NONE = "GESTURE_NONE";

Gesture::GestureEvent lua_enumFromString_GestureGestureEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_SWIPE) == 0)
        return Gesture::GESTURE_SWIPE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_PINCH) == 0)
        return Gesture::GESTURE_PINCH;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_ROTATE) == 0)
        return Gesture::GESTURE_ROTATE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_TAP) == 0)
        return Gesture::GESTURE_TAP;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_TAP_DOUBLE) == 0)
        return Gesture::GESTURE_TAP_DOUBLE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_GESTURE_NONE) == 0)
        return Gesture::GESTURE_NONE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gesture::GestureEvent.", s);
    return Gesture::GESTURE_SWIPE;
}

const char* lua_stringFromEnum_GestureGestureEvent(Gesture::GestureEvent e)
{
    if (e == Gesture::GESTURE_SWIPE)
        return luaEnumString_GestureGestureEvent_GESTURE_SWIPE;
    if (e == Gesture::GESTURE_PINCH)
        return luaEnumString_GestureGestureEvent_GESTURE_PINCH;
    if (e == Gesture::GESTURE_ROTATE)
        return luaEnumString_GestureGestureEvent_GESTURE_ROTATE;
    if (e == Gesture::GESTURE_TAP)
        return luaEnumString_GestureGestureEvent_GESTURE_TAP;
    if (e == Gesture::GESTURE_TAP_DOUBLE)
        return luaEnumString_GestureGestureEvent_GESTURE_TAP_DOUBLE;
    if (e == Gesture::GESTURE_NONE)
        return luaEnumString_GestureGestureEvent_GESTURE_NONE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gesture::GestureEvent.", e);
    return enumStringEmpty;
}

}

