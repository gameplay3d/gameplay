#ifndef LUA_REF_H_
#define LUA_REF_H_

namespace gameplay
{

// Lua bindings for Ref.
int lua_Ref_addRef(lua_State* state);
int lua_Ref_getRefCount(lua_State* state);
int lua_Ref_release(lua_State* state);

void luaRegister_Ref();

}

#endif
