#ifndef LUA_TERRAINFLAGS_H_
#define LUA_TERRAINFLAGS_H_

#include "Terrain.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Terrain::Flags.
Terrain::Flags lua_enumFromString_TerrainFlags(const char* s);
const char* lua_stringFromEnum_TerrainFlags(Terrain::Flags e);

}

#endif
