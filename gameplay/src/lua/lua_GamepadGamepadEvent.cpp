#include "Base.h"
#include "lua_GamepadGamepadEvent.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT = "ATTACHED_EVENT";
static const char* luaEnumString_GamepadGamepadEvent_DETACHED_EVENT = "DETACHED_EVENT";

Gamepad::GamepadEvent lua_enumFromString_GamepadGamepadEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT) == 0)
        return Gamepad::ATTACHED_EVENT;
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_DETACHED_EVENT) == 0)
        return Gamepad::DETACHED_EVENT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gamepad::GamepadEvent.", s);
    return Gamepad::ATTACHED_EVENT;
}

const char* lua_stringFromEnum_GamepadGamepadEvent(Gamepad::GamepadEvent e)
{
    if (e == Gamepad::ATTACHED_EVENT)
        return luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT;
    if (e == Gamepad::DETACHED_EVENT)
        return luaEnumString_GamepadGamepadEvent_DETACHED_EVENT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gamepad::GamepadEvent.", e);
    return enumStringEmpty;
}

}

