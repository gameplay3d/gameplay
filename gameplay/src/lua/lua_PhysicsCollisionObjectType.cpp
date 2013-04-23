#include "Base.h"
#include "lua_PhysicsCollisionObjectType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_PhysicsCollisionObjectType_RIGID_BODY = "RIGID_BODY";
static const char* luaEnumString_PhysicsCollisionObjectType_CHARACTER = "CHARACTER";
static const char* luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT = "GHOST_OBJECT";
static const char* luaEnumString_PhysicsCollisionObjectType_VEHICLE = "VEHICLE";
static const char* luaEnumString_PhysicsCollisionObjectType_VEHICLE_WHEEL = "VEHICLE_WHEEL";
static const char* luaEnumString_PhysicsCollisionObjectType_NONE = "NONE";

PhysicsCollisionObject::Type lua_enumFromString_PhysicsCollisionObjectType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_RIGID_BODY) == 0)
        return PhysicsCollisionObject::RIGID_BODY;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_CHARACTER) == 0)
        return PhysicsCollisionObject::CHARACTER;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT) == 0)
        return PhysicsCollisionObject::GHOST_OBJECT;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_VEHICLE) == 0)
        return PhysicsCollisionObject::VEHICLE;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_VEHICLE_WHEEL) == 0)
        return PhysicsCollisionObject::VEHICLE_WHEEL;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_NONE) == 0)
        return PhysicsCollisionObject::NONE;
    return PhysicsCollisionObject::RIGID_BODY;
}

const char* lua_stringFromEnum_PhysicsCollisionObjectType(PhysicsCollisionObject::Type e)
{
    if (e == PhysicsCollisionObject::RIGID_BODY)
        return luaEnumString_PhysicsCollisionObjectType_RIGID_BODY;
    if (e == PhysicsCollisionObject::CHARACTER)
        return luaEnumString_PhysicsCollisionObjectType_CHARACTER;
    if (e == PhysicsCollisionObject::GHOST_OBJECT)
        return luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT;
    if (e == PhysicsCollisionObject::VEHICLE)
        return luaEnumString_PhysicsCollisionObjectType_VEHICLE;
    if (e == PhysicsCollisionObject::VEHICLE_WHEEL)
        return luaEnumString_PhysicsCollisionObjectType_VEHICLE_WHEEL;
    if (e == PhysicsCollisionObject::NONE)
        return luaEnumString_PhysicsCollisionObjectType_NONE;
    return enumStringEmpty;
}

}

