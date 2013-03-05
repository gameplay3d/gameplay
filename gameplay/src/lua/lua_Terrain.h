#ifndef LUA_TERRAIN_H_
#define LUA_TERRAIN_H_

namespace gameplay
{

// Lua bindings for Terrain.
int lua_Terrain__gc(lua_State* state);
int lua_Terrain_addListener(lua_State* state);
int lua_Terrain_addRef(lua_State* state);
int lua_Terrain_draw(lua_State* state);
int lua_Terrain_getBoundingBox(lua_State* state);
int lua_Terrain_getHeight(lua_State* state);
int lua_Terrain_getInverseWorldMatrix(lua_State* state);
int lua_Terrain_getNode(lua_State* state);
int lua_Terrain_getNormalMatrix(lua_State* state);
int lua_Terrain_getPatchCount(lua_State* state);
int lua_Terrain_getRefCount(lua_State* state);
int lua_Terrain_getTriangleCount(lua_State* state);
int lua_Terrain_getVisiblePatchCount(lua_State* state);
int lua_Terrain_getVisibleTriangleCount(lua_State* state);
int lua_Terrain_getWorldMatrix(lua_State* state);
int lua_Terrain_getWorldViewMatrix(lua_State* state);
int lua_Terrain_getWorldViewProjectionMatrix(lua_State* state);
int lua_Terrain_isFlagSet(lua_State* state);
int lua_Terrain_release(lua_State* state);
int lua_Terrain_removeListener(lua_State* state);
int lua_Terrain_setFlag(lua_State* state);
int lua_Terrain_static_create(lua_State* state);
int lua_Terrain_transformChanged(lua_State* state);

void luaRegister_Terrain();

}

#endif
