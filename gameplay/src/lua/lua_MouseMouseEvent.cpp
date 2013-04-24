#include "Base.h"
#include "lua_MouseMouseEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON = "MOUSE_PRESS_LEFT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON = "MOUSE_RELEASE_LEFT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON = "MOUSE_PRESS_MIDDLE_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON = "MOUSE_RELEASE_MIDDLE_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON = "MOUSE_PRESS_RIGHT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON = "MOUSE_RELEASE_RIGHT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_MOVE = "MOUSE_MOVE";
static const char* luaEnumString_MouseMouseEvent_MOUSE_WHEEL = "MOUSE_WHEEL";

Mouse::MouseEvent lua_enumFromString_MouseMouseEvent(const char* s)
{
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_LEFT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_LEFT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_MIDDLE_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_MIDDLE_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_RIGHT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_RIGHT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_MOVE) == 0)
        return Mouse::MOUSE_MOVE;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_WHEEL) == 0)
        return Mouse::MOUSE_WHEEL;
    return Mouse::MOUSE_PRESS_LEFT_BUTTON;
}

const char* lua_stringFromEnum_MouseMouseEvent(Mouse::MouseEvent e)
{
    if (e == Mouse::MOUSE_PRESS_LEFT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_LEFT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON;
    if (e == Mouse::MOUSE_PRESS_MIDDLE_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_MIDDLE_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON;
    if (e == Mouse::MOUSE_PRESS_RIGHT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_RIGHT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON;
    if (e == Mouse::MOUSE_MOVE)
        return luaEnumString_MouseMouseEvent_MOUSE_MOVE;
    if (e == Mouse::MOUSE_WHEEL)
        return luaEnumString_MouseMouseEvent_MOUSE_WHEEL;
    return enumStringEmpty;
}

}

