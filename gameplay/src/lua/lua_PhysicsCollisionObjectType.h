#ifndef LUA_PHYSICSCOLLISIONOBJECTTYPE_H_
#define LUA_PHYSICSCOLLISIONOBJECTTYPE_H_

#include "PhysicsCollisionObject.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for PhysicsCollisionObject::Type.
PhysicsCollisionObject::Type lua_enumFromString_PhysicsCollisionObjectType(const char* s);
const char* lua_stringFromEnum_PhysicsCollisionObjectType(PhysicsCollisionObject::Type e);

}

#endif
