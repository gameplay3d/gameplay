#ifndef LUA_TILESHEET_H_
#define LUA_TILESHEET_H_

namespace gameplay
{

// Lua bindings for TileSheet.
int lua_TileSheet__gc(lua_State* state);
int lua_TileSheet_addRef(lua_State* state);
int lua_TileSheet_addStrip(lua_State* state);
int lua_TileSheet_getRefCount(lua_State* state);
int lua_TileSheet_getId(lua_State* state);
int lua_TileSheet_getSpriteBatch(lua_State* state);
//getStripCount
//getStripFrame
//getStripFrameCount
//getStripId
int lua_TileSheet_release(lua_State* state);
//removeStrip
//setStripFrame
int lua_TileSheet_static_create(lua_State* state);

void luaRegister_TileSheet();

}

#endif
