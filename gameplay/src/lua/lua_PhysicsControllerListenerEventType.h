#ifndef LUA_PHYSICSCONTROLLERLISTENEREVENTTYPE_H_
#define LUA_PHYSICSCONTROLLERLISTENEREVENTTYPE_H_

#include "PhysicsController.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for PhysicsController::Listener::EventType.
PhysicsController::Listener::EventType lua_enumFromString_PhysicsControllerListenerEventType(const char* s);
const char* lua_stringFromEnum_PhysicsControllerListenerEventType(PhysicsController::Listener::EventType e);

}

#endif
