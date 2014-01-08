#ifndef LUA_TERRAIN_H_
#define LUA_TERRAIN_H_

namespace gameplay
{

// Lua bindings for Terrain.
int lua_Terrain__gc(lua_State* state);
int lua_Terrain_addRef(lua_State* state);
int lua_Terrain_draw(lua_State* state);
int lua_Terrain_getBoundingBox(lua_State* state);
int lua_Terrain_getHeight(lua_State* state);
int lua_Terrain_getNode(lua_State* state);
int lua_Terrain_getPatch(lua_State* state);
int lua_Terrain_getPatchCount(lua_State* state);
int lua_Terrain_getRefCount(lua_State* state);
int lua_Terrain_isFlagSet(lua_State* state);
int lua_Terrain_release(lua_State* state);
int lua_Terrain_setFlag(lua_State* state);
int lua_Terrain_static_create(lua_State* state);

void luaRegister_Terrain();

}

#endif
