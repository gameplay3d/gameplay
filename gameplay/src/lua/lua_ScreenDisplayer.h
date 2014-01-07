#ifndef LUA_SCREENDISPLAYER_H_
#define LUA_SCREENDISPLAYER_H_

namespace gameplay
{

// Lua bindings for ScreenDisplayer.
int lua_ScreenDisplayer__gc(lua_State* state);
int lua_ScreenDisplayer__init(lua_State* state);
int lua_ScreenDisplayer_static_finish(lua_State* state);
int lua_ScreenDisplayer_static_start(lua_State* state);

void luaRegister_ScreenDisplayer();

}

#endif
