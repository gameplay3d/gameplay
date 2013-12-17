#ifndef LUA_THEME_H_
#define LUA_THEME_H_

namespace gameplay
{

// Lua bindings for Theme.
int lua_Theme__gc(lua_State* state);
int lua_Theme_addRef(lua_State* state);
int lua_Theme_getEmptyStyle(lua_State* state);
int lua_Theme_getRefCount(lua_State* state);
int lua_Theme_getSpriteBatch(lua_State* state);
int lua_Theme_getStyle(lua_State* state);
int lua_Theme_release(lua_State* state);
int lua_Theme_static_create(lua_State* state);
int lua_Theme_static_getDefault(lua_State* state);

void luaRegister_Theme();

}

#endif
