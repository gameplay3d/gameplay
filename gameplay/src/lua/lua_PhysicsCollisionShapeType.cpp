#include "Base.h"
#include "lua_PhysicsCollisionShapeType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_NONE = "SHAPE_NONE";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX = "SHAPE_BOX";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE = "SHAPE_SPHERE";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE = "SHAPE_CAPSULE";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH = "SHAPE_MESH";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD = "SHAPE_HEIGHTFIELD";

PhysicsCollisionShape::Type lua_enumFromString_PhysicsCollisionShapeType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_NONE) == 0)
        return PhysicsCollisionShape::SHAPE_NONE;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX) == 0)
        return PhysicsCollisionShape::SHAPE_BOX;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE) == 0)
        return PhysicsCollisionShape::SHAPE_SPHERE;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE) == 0)
        return PhysicsCollisionShape::SHAPE_CAPSULE;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH) == 0)
        return PhysicsCollisionShape::SHAPE_MESH;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD) == 0)
        return PhysicsCollisionShape::SHAPE_HEIGHTFIELD;
    return PhysicsCollisionShape::SHAPE_NONE;
}

const char* lua_stringFromEnum_PhysicsCollisionShapeType(PhysicsCollisionShape::Type e)
{
    if (e == PhysicsCollisionShape::SHAPE_NONE)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_NONE;
    if (e == PhysicsCollisionShape::SHAPE_BOX)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX;
    if (e == PhysicsCollisionShape::SHAPE_SPHERE)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE;
    if (e == PhysicsCollisionShape::SHAPE_CAPSULE)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE;
    if (e == PhysicsCollisionShape::SHAPE_MESH)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH;
    if (e == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD;
    return enumStringEmpty;
}

}

