#ifndef LUA_CONTROLLISTENEREVENTTYPE_H_
#define LUA_CONTROLLISTENEREVENTTYPE_H_

#include "Control.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Control::Listener::EventType.
Control::Listener::EventType lua_enumFromString_ControlListenerEventType(const char* s);
const char* lua_stringFromEnum_ControlListenerEventType(Control::Listener::EventType e);

}

#endif
