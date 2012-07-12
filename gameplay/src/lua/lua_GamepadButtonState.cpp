#include "Base.h"
#include "lua_GamepadButtonState.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GamepadButtonState_BUTTON_PRESSED = "BUTTON_PRESSED";
static const char* luaEnumString_GamepadButtonState_BUTTON_RELEASED = "BUTTON_RELEASED";

Gamepad::ButtonState lua_enumFromString_GamepadButtonState(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadButtonState_BUTTON_PRESSED) == 0)
        return Gamepad::BUTTON_PRESSED;
    if (strcmp(s, luaEnumString_GamepadButtonState_BUTTON_RELEASED) == 0)
        return Gamepad::BUTTON_RELEASED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gamepad::ButtonState.", s);
    return Gamepad::BUTTON_PRESSED;
}

const char* lua_stringFromEnum_GamepadButtonState(Gamepad::ButtonState e)
{
    if (e == Gamepad::BUTTON_PRESSED)
        return luaEnumString_GamepadButtonState_BUTTON_PRESSED;
    if (e == Gamepad::BUTTON_RELEASED)
        return luaEnumString_GamepadButtonState_BUTTON_RELEASED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gamepad::ButtonState.", e);
    return enumStringEmpty;
}

}

