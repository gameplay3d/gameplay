#ifndef LUA_ABSOLUTELAYOUT_H_
#define LUA_ABSOLUTELAYOUT_H_

namespace gameplay
{

// Lua bindings for AbsoluteLayout.
int lua_AbsoluteLayout__gc(lua_State* state);
int lua_AbsoluteLayout_addRef(lua_State* state);
int lua_AbsoluteLayout_getRefCount(lua_State* state);
int lua_AbsoluteLayout_getType(lua_State* state);
int lua_AbsoluteLayout_release(lua_State* state);

void luaRegister_AbsoluteLayout();

}

#endif
