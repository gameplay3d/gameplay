#ifndef LUA_TERRAINHEIGHTFIELD_H_
#define LUA_TERRAINHEIGHTFIELD_H_

namespace gameplay
{

// Lua bindings for Terrain::HeightField.
int lua_TerrainHeightField__gc(lua_State* state);
int lua_TerrainHeightField_addRef(lua_State* state);
int lua_TerrainHeightField_getArray(lua_State* state);
int lua_TerrainHeightField_getColumnCount(lua_State* state);
int lua_TerrainHeightField_getRefCount(lua_State* state);
int lua_TerrainHeightField_getRowCount(lua_State* state);
int lua_TerrainHeightField_release(lua_State* state);
int lua_TerrainHeightField_static_create(lua_State* state);

void luaRegister_TerrainHeightField();

}

#endif
