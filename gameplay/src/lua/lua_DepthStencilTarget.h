#ifndef LUA_DEPTHSTENCILTARGET_H_
#define LUA_DEPTHSTENCILTARGET_H_

namespace gameplay
{

// Lua bindings for DepthStencilTarget.
int lua_DepthStencilTarget__gc(lua_State* state);
int lua_DepthStencilTarget_addRef(lua_State* state);
int lua_DepthStencilTarget_getFormat(lua_State* state);
int lua_DepthStencilTarget_getHeight(lua_State* state);
int lua_DepthStencilTarget_getId(lua_State* state);
int lua_DepthStencilTarget_getRefCount(lua_State* state);
int lua_DepthStencilTarget_getWidth(lua_State* state);
int lua_DepthStencilTarget_isPacked(lua_State* state);
int lua_DepthStencilTarget_release(lua_State* state);
int lua_DepthStencilTarget_static_create(lua_State* state);
int lua_DepthStencilTarget_static_getDepthStencilTarget(lua_State* state);

void luaRegister_DepthStencilTarget();

}

#endif
