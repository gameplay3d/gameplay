#include "Base.h"
#include "lua_PhysicsCollisionObjectCollisionListenerEventType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING = "COLLIDING";
static const char* luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING = "NOT_COLLIDING";

PhysicsCollisionObject::CollisionListener::EventType lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING) == 0)
        return PhysicsCollisionObject::CollisionListener::COLLIDING;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING) == 0)
        return PhysicsCollisionObject::CollisionListener::NOT_COLLIDING;
    return PhysicsCollisionObject::CollisionListener::COLLIDING;
}

const char* lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType(PhysicsCollisionObject::CollisionListener::EventType e)
{
    if (e == PhysicsCollisionObject::CollisionListener::COLLIDING)
        return luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING;
    if (e == PhysicsCollisionObject::CollisionListener::NOT_COLLIDING)
        return luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING;
    return enumStringEmpty;
}

}

