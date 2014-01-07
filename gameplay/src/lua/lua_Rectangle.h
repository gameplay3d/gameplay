#ifndef LUA_RECTANGLE_H_
#define LUA_RECTANGLE_H_

namespace gameplay
{

// Lua bindings for Rectangle.
int lua_Rectangle__gc(lua_State* state);
int lua_Rectangle__init(lua_State* state);
int lua_Rectangle_bottom(lua_State* state);
int lua_Rectangle_contains(lua_State* state);
int lua_Rectangle_height(lua_State* state);
int lua_Rectangle_inflate(lua_State* state);
int lua_Rectangle_intersects(lua_State* state);
int lua_Rectangle_isEmpty(lua_State* state);
int lua_Rectangle_left(lua_State* state);
int lua_Rectangle_right(lua_State* state);
int lua_Rectangle_set(lua_State* state);
int lua_Rectangle_setPosition(lua_State* state);
int lua_Rectangle_static_combine(lua_State* state);
int lua_Rectangle_static_empty(lua_State* state);
int lua_Rectangle_static_intersect(lua_State* state);
int lua_Rectangle_top(lua_State* state);
int lua_Rectangle_width(lua_State* state);
int lua_Rectangle_x(lua_State* state);
int lua_Rectangle_y(lua_State* state);

void luaRegister_Rectangle();

}

#endif
