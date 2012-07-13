#ifndef LUA_FONTGLYPH_H_
#define LUA_FONTGLYPH_H_

namespace gameplay
{

// Lua bindings for Font::Glyph.
int lua_FontGlyph__gc(lua_State* state);
int lua_FontGlyph__init(lua_State* state);
int lua_FontGlyph_code(lua_State* state);
int lua_FontGlyph_uvs(lua_State* state);
int lua_FontGlyph_width(lua_State* state);

void luaRegister_FontGlyph();

}

#endif
