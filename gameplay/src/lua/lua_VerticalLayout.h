#ifndef LUA_VERTICALLAYOUT_H_
#define LUA_VERTICALLAYOUT_H_

namespace gameplay
{

// Lua bindings for VerticalLayout.
int lua_VerticalLayout__gc(lua_State* state);
int lua_VerticalLayout_addRef(lua_State* state);
int lua_VerticalLayout_getBottomToTop(lua_State* state);
int lua_VerticalLayout_getRefCount(lua_State* state);
int lua_VerticalLayout_getSpacing(lua_State* state);
int lua_VerticalLayout_getType(lua_State* state);
int lua_VerticalLayout_release(lua_State* state);
int lua_VerticalLayout_setBottomToTop(lua_State* state);
int lua_VerticalLayout_setSpacing(lua_State* state);

void luaRegister_VerticalLayout();

}

#endif
