#include "Base.h"
#include "lua_KeyboardKeyEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_KeyboardKeyEvent_KEY_PRESS = "KEY_PRESS";
static const char* luaEnumString_KeyboardKeyEvent_KEY_RELEASE = "KEY_RELEASE";
static const char* luaEnumString_KeyboardKeyEvent_KEY_CHAR = "KEY_CHAR";

Keyboard::KeyEvent lua_enumFromString_KeyboardKeyEvent(const char* s)
{
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_PRESS) == 0)
        return Keyboard::KEY_PRESS;
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_RELEASE) == 0)
        return Keyboard::KEY_RELEASE;
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_CHAR) == 0)
        return Keyboard::KEY_CHAR;
    return Keyboard::KEY_PRESS;
}

const char* lua_stringFromEnum_KeyboardKeyEvent(Keyboard::KeyEvent e)
{
    if (e == Keyboard::KEY_PRESS)
        return luaEnumString_KeyboardKeyEvent_KEY_PRESS;
    if (e == Keyboard::KEY_RELEASE)
        return luaEnumString_KeyboardKeyEvent_KEY_RELEASE;
    if (e == Keyboard::KEY_CHAR)
        return luaEnumString_KeyboardKeyEvent_KEY_CHAR;
    return enumStringEmpty;
}

}

