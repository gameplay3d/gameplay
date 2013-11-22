#ifndef LUA_FONT_H_
#define LUA_FONT_H_

namespace gameplay
{

// Lua bindings for Font.
int lua_Font__gc(lua_State* state);
int lua_Font_addRef(lua_State* state);
int lua_Font_createText(lua_State* state);
int lua_Font_drawText(lua_State* state);
int lua_Font_finish(lua_State* state);
int lua_Font_getCharacterSpacing(lua_State* state);
int lua_Font_getFormat(lua_State* state);
int lua_Font_getIndexAtLocation(lua_State* state);
int lua_Font_getLocationAtIndex(lua_State* state);
int lua_Font_getRefCount(lua_State* state);
int lua_Font_getSize(lua_State* state);
int lua_Font_getSpriteBatch(lua_State* state);
int lua_Font_isCharacterSupported(lua_State* state);
int lua_Font_measureText(lua_State* state);
int lua_Font_release(lua_State* state);
int lua_Font_setCharacterSpacing(lua_State* state);
int lua_Font_start(lua_State* state);
int lua_Font_static_create(lua_State* state);
int lua_Font_static_getJustify(lua_State* state);

void luaRegister_Font();

}

#endif
