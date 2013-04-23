#include "Base.h"
#include "lua_TouchTouchEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TouchTouchEvent_TOUCH_PRESS = "TOUCH_PRESS";
static const char* luaEnumString_TouchTouchEvent_TOUCH_RELEASE = "TOUCH_RELEASE";
static const char* luaEnumString_TouchTouchEvent_TOUCH_MOVE = "TOUCH_MOVE";

Touch::TouchEvent lua_enumFromString_TouchTouchEvent(const char* s)
{
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_PRESS) == 0)
        return Touch::TOUCH_PRESS;
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_RELEASE) == 0)
        return Touch::TOUCH_RELEASE;
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_MOVE) == 0)
        return Touch::TOUCH_MOVE;
    return Touch::TOUCH_PRESS;
}

const char* lua_stringFromEnum_TouchTouchEvent(Touch::TouchEvent e)
{
    if (e == Touch::TOUCH_PRESS)
        return luaEnumString_TouchTouchEvent_TOUCH_PRESS;
    if (e == Touch::TOUCH_RELEASE)
        return luaEnumString_TouchTouchEvent_TOUCH_RELEASE;
    if (e == Touch::TOUCH_MOVE)
        return luaEnumString_TouchTouchEvent_TOUCH_MOVE;
    return enumStringEmpty;
}

}

