#ifndef LUA_FRUSTUM_H_
#define LUA_FRUSTUM_H_

namespace gameplay
{

// Lua bindings for Frustum.
int lua_Frustum__gc(lua_State* state);
int lua_Frustum__init(lua_State* state);
int lua_Frustum_getBottom(lua_State* state);
int lua_Frustum_getCorners(lua_State* state);
int lua_Frustum_getFar(lua_State* state);
int lua_Frustum_getFarCorners(lua_State* state);
int lua_Frustum_getLeft(lua_State* state);
int lua_Frustum_getMatrix(lua_State* state);
int lua_Frustum_getNear(lua_State* state);
int lua_Frustum_getNearCorners(lua_State* state);
int lua_Frustum_getRight(lua_State* state);
int lua_Frustum_getTop(lua_State* state);
int lua_Frustum_intersects(lua_State* state);
int lua_Frustum_set(lua_State* state);

void luaRegister_Frustum();

}

#endif
