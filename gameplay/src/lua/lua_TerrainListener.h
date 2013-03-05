#ifndef LUA_TERRAINLISTENER_H_
#define LUA_TERRAINLISTENER_H_

namespace gameplay
{

// Lua bindings for Terrain::Listener.
int lua_TerrainListener__gc(lua_State* state);
int lua_TerrainListener_materialUpdated(lua_State* state);

void luaRegister_TerrainListener();

}

#endif
