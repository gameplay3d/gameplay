#ifndef LUA_FLOWLAYOUT_H_
#define LUA_FLOWLAYOUT_H_

namespace gameplay
{

// Lua bindings for FlowLayout.
int lua_FlowLayout__gc(lua_State* state);
int lua_FlowLayout_addRef(lua_State* state);
int lua_FlowLayout_getHorizontalSpacing(lua_State* state);
int lua_FlowLayout_getRefCount(lua_State* state);
int lua_FlowLayout_getType(lua_State* state);
int lua_FlowLayout_getVerticalSpacing(lua_State* state);
int lua_FlowLayout_release(lua_State* state);
int lua_FlowLayout_setSpacing(lua_State* state);

void luaRegister_FlowLayout();

}

#endif
