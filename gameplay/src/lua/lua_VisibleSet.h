#ifndef LUA_VISIBLESET_H_
#define LUA_VISIBLESET_H_

namespace gameplay
{

// Lua bindings for VisibleSet.
int lua_VisibleSet__gc(lua_State* state);
int lua_VisibleSet_getActiveCamera(lua_State* state);
int lua_VisibleSet_getNext(lua_State* state);
int lua_VisibleSet_reset(lua_State* state);

void luaRegister_VisibleSet();

}

#endif
