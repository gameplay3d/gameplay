#ifndef LUA_THEMETHEMEIMAGE_H_
#define LUA_THEMETHEMEIMAGE_H_

namespace gameplay
{

// Lua bindings for Theme::ThemeImage.
int lua_ThemeThemeImage__gc(lua_State* state);
int lua_ThemeThemeImage_addRef(lua_State* state);
int lua_ThemeThemeImage_getColor(lua_State* state);
int lua_ThemeThemeImage_getId(lua_State* state);
int lua_ThemeThemeImage_getRefCount(lua_State* state);
int lua_ThemeThemeImage_getRegion(lua_State* state);
int lua_ThemeThemeImage_getUVs(lua_State* state);
int lua_ThemeThemeImage_release(lua_State* state);

void luaRegister_ThemeThemeImage();

}

#endif
