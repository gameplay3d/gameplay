#include "Base.h"
#include "lua_MeshPrimitiveType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_MeshPrimitiveType_TRIANGLES = "TRIANGLES";
static const char* luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP = "TRIANGLE_STRIP";
static const char* luaEnumString_MeshPrimitiveType_LINES = "LINES";
static const char* luaEnumString_MeshPrimitiveType_LINE_STRIP = "LINE_STRIP";
static const char* luaEnumString_MeshPrimitiveType_POINTS = "POINTS";

Mesh::PrimitiveType lua_enumFromString_MeshPrimitiveType(const char* s)
{
    if (strcmp(s, luaEnumString_MeshPrimitiveType_TRIANGLES) == 0)
        return Mesh::TRIANGLES;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP) == 0)
        return Mesh::TRIANGLE_STRIP;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_LINES) == 0)
        return Mesh::LINES;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_LINE_STRIP) == 0)
        return Mesh::LINE_STRIP;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_POINTS) == 0)
        return Mesh::POINTS;
    return Mesh::TRIANGLES;
}

const char* lua_stringFromEnum_MeshPrimitiveType(Mesh::PrimitiveType e)
{
    if (e == Mesh::TRIANGLES)
        return luaEnumString_MeshPrimitiveType_TRIANGLES;
    if (e == Mesh::TRIANGLE_STRIP)
        return luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP;
    if (e == Mesh::LINES)
        return luaEnumString_MeshPrimitiveType_LINES;
    if (e == Mesh::LINE_STRIP)
        return luaEnumString_MeshPrimitiveType_LINE_STRIP;
    if (e == Mesh::POINTS)
        return luaEnumString_MeshPrimitiveType_POINTS;
    return enumStringEmpty;
}

}

