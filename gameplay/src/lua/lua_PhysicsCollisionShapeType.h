#ifndef LUA_PHYSICSCOLLISIONSHAPETYPE_H_
#define LUA_PHYSICSCOLLISIONSHAPETYPE_H_

#include "PhysicsCollisionShape.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for PhysicsCollisionShape::Type.
PhysicsCollisionShape::Type lua_enumFromString_PhysicsCollisionShapeType(const char* s);
const char* lua_stringFromEnum_PhysicsCollisionShapeType(PhysicsCollisionShape::Type e);

}

#endif
