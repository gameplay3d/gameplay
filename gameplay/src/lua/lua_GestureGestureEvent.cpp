#include "Base.h"
#include "lua_GestureGestureEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GestureGestureEvent_SWIPE = "SWIPE";
static const char* luaEnumString_GestureGestureEvent_PINCH = "PINCH";
static const char* luaEnumString_GestureGestureEvent_ROTATE = "ROTATE";
static const char* luaEnumString_GestureGestureEvent_TAP = "TAP";
static const char* luaEnumString_GestureGestureEvent_TAP_DOUBLE = "TAP_DOUBLE";
static const char* luaEnumString_GestureGestureEvent_NONE = "NONE";

Gesture::GestureEvent lua_enumFromString_GestureGestureEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GestureGestureEvent_SWIPE) == 0)
        return Gesture::SWIPE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_PINCH) == 0)
        return Gesture::PINCH;
    if (strcmp(s, luaEnumString_GestureGestureEvent_ROTATE) == 0)
        return Gesture::ROTATE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_TAP) == 0)
        return Gesture::TAP;
    if (strcmp(s, luaEnumString_GestureGestureEvent_TAP_DOUBLE) == 0)
        return Gesture::TAP_DOUBLE;
    if (strcmp(s, luaEnumString_GestureGestureEvent_NONE) == 0)
        return Gesture::NONE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gesture::GestureEvent.", s);
    return Gesture::SWIPE;
}

const char* lua_stringFromEnum_GestureGestureEvent(Gesture::GestureEvent e)
{
    if (e == Gesture::SWIPE)
        return luaEnumString_GestureGestureEvent_SWIPE;
    if (e == Gesture::PINCH)
        return luaEnumString_GestureGestureEvent_PINCH;
    if (e == Gesture::ROTATE)
        return luaEnumString_GestureGestureEvent_ROTATE;
    if (e == Gesture::TAP)
        return luaEnumString_GestureGestureEvent_TAP;
    if (e == Gesture::TAP_DOUBLE)
        return luaEnumString_GestureGestureEvent_TAP_DOUBLE;
    if (e == Gesture::NONE)
        return luaEnumString_GestureGestureEvent_NONE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gesture::GestureEvent.", e);
    return enumStringEmpty;
}

}

