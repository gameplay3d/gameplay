#ifndef LUA_NODECLONECONTEXT_H_
#define LUA_NODECLONECONTEXT_H_

namespace gameplay
{

// Lua bindings for NodeCloneContext.
int lua_NodeCloneContext__gc(lua_State* state);
int lua_NodeCloneContext__init(lua_State* state);
int lua_NodeCloneContext_findClonedAnimation(lua_State* state);
int lua_NodeCloneContext_findClonedNode(lua_State* state);
int lua_NodeCloneContext_registerClonedAnimation(lua_State* state);
int lua_NodeCloneContext_registerClonedNode(lua_State* state);

void luaRegister_NodeCloneContext();

}

#endif
