#include "Base.h"
#include "lua_ControlState.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ControlState_NORMAL = "NORMAL";
static const char* luaEnumString_ControlState_FOCUS = "FOCUS";
static const char* luaEnumString_ControlState_ACTIVE = "ACTIVE";
static const char* luaEnumString_ControlState_DISABLED = "DISABLED";
static const char* luaEnumString_ControlState_HOVER = "HOVER";

Control::State lua_enumFromString_ControlState(const char* s)
{
    if (strcmp(s, luaEnumString_ControlState_NORMAL) == 0)
        return Control::NORMAL;
    if (strcmp(s, luaEnumString_ControlState_FOCUS) == 0)
        return Control::FOCUS;
    if (strcmp(s, luaEnumString_ControlState_ACTIVE) == 0)
        return Control::ACTIVE;
    if (strcmp(s, luaEnumString_ControlState_DISABLED) == 0)
        return Control::DISABLED;
    if (strcmp(s, luaEnumString_ControlState_HOVER) == 0)
        return Control::HOVER;
    return Control::NORMAL;
}

const char* lua_stringFromEnum_ControlState(Control::State e)
{
    if (e == Control::NORMAL)
        return luaEnumString_ControlState_NORMAL;
    if (e == Control::FOCUS)
        return luaEnumString_ControlState_FOCUS;
    if (e == Control::ACTIVE)
        return luaEnumString_ControlState_ACTIVE;
    if (e == Control::DISABLED)
        return luaEnumString_ControlState_DISABLED;
    if (e == Control::HOVER)
        return luaEnumString_ControlState_HOVER;
    return enumStringEmpty;
}

}

