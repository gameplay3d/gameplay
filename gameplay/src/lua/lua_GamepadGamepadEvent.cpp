#include "Base.h"
#include "lua_GamepadGamepadEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GamepadGamepadEvent_CONNECTED_EVENT = "CONNECTED_EVENT";
static const char* luaEnumString_GamepadGamepadEvent_DISCONNECTED_EVENT = "DISCONNECTED_EVENT";

Gamepad::GamepadEvent lua_enumFromString_GamepadGamepadEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_CONNECTED_EVENT) == 0)
        return Gamepad::CONNECTED_EVENT;
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_DISCONNECTED_EVENT) == 0)
        return Gamepad::DISCONNECTED_EVENT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gamepad::GamepadEvent.", s);
    return Gamepad::CONNECTED_EVENT;
}

const char* lua_stringFromEnum_GamepadGamepadEvent(Gamepad::GamepadEvent e)
{
    if (e == Gamepad::CONNECTED_EVENT)
        return luaEnumString_GamepadGamepadEvent_CONNECTED_EVENT;
    if (e == Gamepad::DISCONNECTED_EVENT)
        return luaEnumString_GamepadGamepadEvent_DISCONNECTED_EVENT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gamepad::GamepadEvent.", e);
    return enumStringEmpty;
}

}

