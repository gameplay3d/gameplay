#ifndef LUA_VISIBLESETDEFAULT_H_
#define LUA_VISIBLESETDEFAULT_H_

namespace gameplay
{

// Lua bindings for VisibleSetDefault.
int lua_VisibleSetDefault_getNext(lua_State* state);
int lua_VisibleSetDefault_getScene(lua_State* state);
int lua_VisibleSetDefault_reset(lua_State* state);
int lua_VisibleSetDefault_setScene(lua_State* state);
int lua_VisibleSetDefault_static_create(lua_State* state);

void luaRegister_VisibleSetDefault();

}

#endif
