#include "Base.h"
#include "lua_TerrainFlags.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TerrainFlags_DEBUG_PATCHES = "DEBUG_PATCHES";
static const char* luaEnumString_TerrainFlags_ENABLE_FRUSTUM_CULLING = "ENABLE_FRUSTUM_CULLING";
static const char* luaEnumString_TerrainFlags_ENABLE_LEVEL_OF_DETAIL = "ENABLE_LEVEL_OF_DETAIL";

Terrain::Flags lua_enumFromString_TerrainFlags(const char* s)
{
    if (strcmp(s, luaEnumString_TerrainFlags_DEBUG_PATCHES) == 0)
        return Terrain::DEBUG_PATCHES;
    if (strcmp(s, luaEnumString_TerrainFlags_ENABLE_FRUSTUM_CULLING) == 0)
        return Terrain::ENABLE_FRUSTUM_CULLING;
    if (strcmp(s, luaEnumString_TerrainFlags_ENABLE_LEVEL_OF_DETAIL) == 0)
        return Terrain::ENABLE_LEVEL_OF_DETAIL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Terrain::Flags.", s);
    return Terrain::DEBUG_PATCHES;
}

const char* lua_stringFromEnum_TerrainFlags(Terrain::Flags e)
{
    if (e == Terrain::DEBUG_PATCHES)
        return luaEnumString_TerrainFlags_DEBUG_PATCHES;
    if (e == Terrain::ENABLE_FRUSTUM_CULLING)
        return luaEnumString_TerrainFlags_ENABLE_FRUSTUM_CULLING;
    if (e == Terrain::ENABLE_LEVEL_OF_DETAIL)
        return luaEnumString_TerrainFlags_ENABLE_LEVEL_OF_DETAIL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Terrain::Flags.", e);
    return enumStringEmpty;
}

}

