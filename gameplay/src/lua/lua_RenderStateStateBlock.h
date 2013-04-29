#ifndef LUA_RENDERSTATESTATEBLOCK_H_
#define LUA_RENDERSTATESTATEBLOCK_H_

namespace gameplay
{

// Lua bindings for RenderState::StateBlock.
int lua_RenderStateStateBlock__gc(lua_State* state);
int lua_RenderStateStateBlock_addRef(lua_State* state);
int lua_RenderStateStateBlock_bind(lua_State* state);
int lua_RenderStateStateBlock_getRefCount(lua_State* state);
int lua_RenderStateStateBlock_release(lua_State* state);
int lua_RenderStateStateBlock_setBlend(lua_State* state);
int lua_RenderStateStateBlock_setBlendDst(lua_State* state);
int lua_RenderStateStateBlock_setBlendSrc(lua_State* state);
int lua_RenderStateStateBlock_setCullFace(lua_State* state);
int lua_RenderStateStateBlock_setCullFaceSide(lua_State* state);
int lua_RenderStateStateBlock_setDepthFunction(lua_State* state);
int lua_RenderStateStateBlock_setDepthTest(lua_State* state);
int lua_RenderStateStateBlock_setDepthWrite(lua_State* state);
int lua_RenderStateStateBlock_setState(lua_State* state);
int lua_RenderStateStateBlock_static_create(lua_State* state);

void luaRegister_RenderStateStateBlock();

}

#endif
