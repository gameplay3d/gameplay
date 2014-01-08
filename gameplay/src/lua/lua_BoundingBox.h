#ifndef LUA_BOUNDINGBOX_H_
#define LUA_BOUNDINGBOX_H_

namespace gameplay
{

// Lua bindings for BoundingBox.
int lua_BoundingBox__gc(lua_State* state);
int lua_BoundingBox__init(lua_State* state);
int lua_BoundingBox_getCenter(lua_State* state);
int lua_BoundingBox_getCorners(lua_State* state);
int lua_BoundingBox_intersects(lua_State* state);
int lua_BoundingBox_isEmpty(lua_State* state);
int lua_BoundingBox_max(lua_State* state);
int lua_BoundingBox_merge(lua_State* state);
int lua_BoundingBox_min(lua_State* state);
int lua_BoundingBox_set(lua_State* state);
int lua_BoundingBox_static_empty(lua_State* state);
int lua_BoundingBox_transform(lua_State* state);

void luaRegister_BoundingBox();

}

#endif
