#ifndef LUA_LAYOUT_H_
#define LUA_LAYOUT_H_

namespace gameplay
{

// Lua bindings for Layout.
int lua_Layout__gc(lua_State* state);
int lua_Layout_addRef(lua_State* state);
int lua_Layout_getRefCount(lua_State* state);
int lua_Layout_getType(lua_State* state);
int lua_Layout_release(lua_State* state);

void luaRegister_Layout();

}

#endif
