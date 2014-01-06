#ifndef LUA_FONTTEXT_H_
#define LUA_FONTTEXT_H_

namespace gameplay
{

// Lua bindings for Font::Text.
int lua_FontText__gc(lua_State* state);
int lua_FontText__init(lua_State* state);
int lua_FontText_getText(lua_State* state);

void luaRegister_FontText();

}

#endif
