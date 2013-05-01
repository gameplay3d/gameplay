#ifndef LUA_FRAMEBUFFER_H_
#define LUA_FRAMEBUFFER_H_

namespace gameplay
{

// Lua bindings for FrameBuffer.
int lua_FrameBuffer__gc(lua_State* state);
int lua_FrameBuffer_addRef(lua_State* state);
int lua_FrameBuffer_bind(lua_State* state);
int lua_FrameBuffer_getDepthStencilTarget(lua_State* state);
int lua_FrameBuffer_getHeight(lua_State* state);
int lua_FrameBuffer_getId(lua_State* state);
int lua_FrameBuffer_getRefCount(lua_State* state);
int lua_FrameBuffer_getRenderTarget(lua_State* state);
int lua_FrameBuffer_getRenderTargetCount(lua_State* state);
int lua_FrameBuffer_getWidth(lua_State* state);
int lua_FrameBuffer_isDefault(lua_State* state);
int lua_FrameBuffer_release(lua_State* state);
int lua_FrameBuffer_setDepthStencilTarget(lua_State* state);
int lua_FrameBuffer_setRenderTarget(lua_State* state);
int lua_FrameBuffer_static_bindDefault(lua_State* state);
int lua_FrameBuffer_static_create(lua_State* state);
int lua_FrameBuffer_static_getCurrent(lua_State* state);
int lua_FrameBuffer_static_getFrameBuffer(lua_State* state);
int lua_FrameBuffer_static_getMaxRenderTargets(lua_State* state);

void luaRegister_FrameBuffer();

}

#endif
