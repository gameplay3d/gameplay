#ifndef LUA_THEMEUVS_H_
#define LUA_THEMEUVS_H_

namespace gameplay
{

// Lua bindings for Theme::UVs.
int lua_ThemeUVs__gc(lua_State* state);
int lua_ThemeUVs__init(lua_State* state);
int lua_ThemeUVs_static_empty(lua_State* state);
int lua_ThemeUVs_static_full(lua_State* state);
int lua_ThemeUVs_u1(lua_State* state);
int lua_ThemeUVs_u2(lua_State* state);
int lua_ThemeUVs_v1(lua_State* state);
int lua_ThemeUVs_v2(lua_State* state);

void luaRegister_ThemeUVs();

}

#endif
