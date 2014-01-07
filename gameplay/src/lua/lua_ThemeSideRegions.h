#ifndef LUA_THEMESIDEREGIONS_H_
#define LUA_THEMESIDEREGIONS_H_

namespace gameplay
{

// Lua bindings for Theme::SideRegions.
int lua_ThemeSideRegions__gc(lua_State* state);
int lua_ThemeSideRegions__init(lua_State* state);
int lua_ThemeSideRegions_bottom(lua_State* state);
int lua_ThemeSideRegions_left(lua_State* state);
int lua_ThemeSideRegions_right(lua_State* state);
int lua_ThemeSideRegions_static_empty(lua_State* state);
int lua_ThemeSideRegions_top(lua_State* state);

void luaRegister_ThemeSideRegions();

}

#endif
