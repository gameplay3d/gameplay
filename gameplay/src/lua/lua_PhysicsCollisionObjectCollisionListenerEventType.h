#ifndef LUA_PHYSICSCOLLISIONOBJECTCOLLISIONLISTENEREVENTTYPE_H_
#define LUA_PHYSICSCOLLISIONOBJECTCOLLISIONLISTENEREVENTTYPE_H_

#include "PhysicsCollisionObject.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for PhysicsCollisionObject::CollisionListener::EventType.
PhysicsCollisionObject::CollisionListener::EventType lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(const char* s);
const char* lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType(PhysicsCollisionObject::CollisionListener::EventType e);

}

#endif
