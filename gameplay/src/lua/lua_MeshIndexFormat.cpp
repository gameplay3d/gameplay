#include "Base.h"
#include "lua_MeshIndexFormat.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_MeshIndexFormat_INDEX8 = "INDEX8";
static const char* luaEnumString_MeshIndexFormat_INDEX16 = "INDEX16";
static const char* luaEnumString_MeshIndexFormat_INDEX32 = "INDEX32";

Mesh::IndexFormat lua_enumFromString_MeshIndexFormat(const char* s)
{
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX8) == 0)
        return Mesh::INDEX8;
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX16) == 0)
        return Mesh::INDEX16;
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX32) == 0)
        return Mesh::INDEX32;
    return Mesh::INDEX8;
}

const char* lua_stringFromEnum_MeshIndexFormat(Mesh::IndexFormat e)
{
    if (e == Mesh::INDEX8)
        return luaEnumString_MeshIndexFormat_INDEX8;
    if (e == Mesh::INDEX16)
        return luaEnumString_MeshIndexFormat_INDEX16;
    if (e == Mesh::INDEX32)
        return luaEnumString_MeshIndexFormat_INDEX32;
    return enumStringEmpty;
}

}

