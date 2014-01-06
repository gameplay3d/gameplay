#ifndef LUA_RENDERTARGET_H_
#define LUA_RENDERTARGET_H_

namespace gameplay
{

// Lua bindings for RenderTarget.
int lua_RenderTarget__gc(lua_State* state);
int lua_RenderTarget_addRef(lua_State* state);
int lua_RenderTarget_getHeight(lua_State* state);
int lua_RenderTarget_getId(lua_State* state);
int lua_RenderTarget_getRefCount(lua_State* state);
int lua_RenderTarget_getTexture(lua_State* state);
int lua_RenderTarget_getWidth(lua_State* state);
int lua_RenderTarget_release(lua_State* state);
int lua_RenderTarget_static_create(lua_State* state);
int lua_RenderTarget_static_getRenderTarget(lua_State* state);

void luaRegister_RenderTarget();

}

#endif
