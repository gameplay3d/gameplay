#include "Base.h"
#include "lua_ControlListenerEventType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ControlListenerEventType_PRESS = "PRESS";
static const char* luaEnumString_ControlListenerEventType_RELEASE = "RELEASE";
static const char* luaEnumString_ControlListenerEventType_CLICK = "CLICK";
static const char* luaEnumString_ControlListenerEventType_VALUE_CHANGED = "VALUE_CHANGED";
static const char* luaEnumString_ControlListenerEventType_TEXT_CHANGED = "TEXT_CHANGED";
static const char* luaEnumString_ControlListenerEventType_MIDDLE_CLICK = "MIDDLE_CLICK";
static const char* luaEnumString_ControlListenerEventType_RIGHT_CLICK = "RIGHT_CLICK";
static const char* luaEnumString_ControlListenerEventType_ENTER = "ENTER";
static const char* luaEnumString_ControlListenerEventType_LEAVE = "LEAVE";

Control::Listener::EventType lua_enumFromString_ControlListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_ControlListenerEventType_PRESS) == 0)
        return Control::Listener::PRESS;
    if (strcmp(s, luaEnumString_ControlListenerEventType_RELEASE) == 0)
        return Control::Listener::RELEASE;
    if (strcmp(s, luaEnumString_ControlListenerEventType_CLICK) == 0)
        return Control::Listener::CLICK;
    if (strcmp(s, luaEnumString_ControlListenerEventType_VALUE_CHANGED) == 0)
        return Control::Listener::VALUE_CHANGED;
    if (strcmp(s, luaEnumString_ControlListenerEventType_TEXT_CHANGED) == 0)
        return Control::Listener::TEXT_CHANGED;
    if (strcmp(s, luaEnumString_ControlListenerEventType_MIDDLE_CLICK) == 0)
        return Control::Listener::MIDDLE_CLICK;
    if (strcmp(s, luaEnumString_ControlListenerEventType_RIGHT_CLICK) == 0)
        return Control::Listener::RIGHT_CLICK;
    if (strcmp(s, luaEnumString_ControlListenerEventType_ENTER) == 0)
        return Control::Listener::ENTER;
    if (strcmp(s, luaEnumString_ControlListenerEventType_LEAVE) == 0)
        return Control::Listener::LEAVE;
    return Control::Listener::PRESS;
}

const char* lua_stringFromEnum_ControlListenerEventType(Control::Listener::EventType e)
{
    if (e == Control::Listener::PRESS)
        return luaEnumString_ControlListenerEventType_PRESS;
    if (e == Control::Listener::RELEASE)
        return luaEnumString_ControlListenerEventType_RELEASE;
    if (e == Control::Listener::CLICK)
        return luaEnumString_ControlListenerEventType_CLICK;
    if (e == Control::Listener::VALUE_CHANGED)
        return luaEnumString_ControlListenerEventType_VALUE_CHANGED;
    if (e == Control::Listener::TEXT_CHANGED)
        return luaEnumString_ControlListenerEventType_TEXT_CHANGED;
    if (e == Control::Listener::MIDDLE_CLICK)
        return luaEnumString_ControlListenerEventType_MIDDLE_CLICK;
    if (e == Control::Listener::RIGHT_CLICK)
        return luaEnumString_ControlListenerEventType_RIGHT_CLICK;
    if (e == Control::Listener::ENTER)
        return luaEnumString_ControlListenerEventType_ENTER;
    if (e == Control::Listener::LEAVE)
        return luaEnumString_ControlListenerEventType_LEAVE;
    return enumStringEmpty;
}

}

