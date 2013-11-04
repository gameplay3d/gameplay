#ifndef LUA_TERRAINPATCH_H_
#define LUA_TERRAINPATCH_H_

namespace gameplay
{

// Lua bindings for TerrainPatch.
int lua_TerrainPatch_cameraChanged(lua_State* state);
int lua_TerrainPatch_getBoundingBox(lua_State* state);
int lua_TerrainPatch_getMaterial(lua_State* state);
int lua_TerrainPatch_getMaterialCount(lua_State* state);

void luaRegister_TerrainPatch();

}

#endif
