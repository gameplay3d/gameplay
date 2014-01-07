#ifndef LUA_THEMESTYLE_H_
#define LUA_THEMESTYLE_H_

namespace gameplay
{

// Lua bindings for Theme::Style.
int lua_ThemeStyle_getTheme(lua_State* state);

void luaRegister_ThemeStyle();

}

#endif
