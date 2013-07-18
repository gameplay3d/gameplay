#ifndef LUA_RENDERSTATESTENCILFUNCTION_H_
#define LUA_RENDERSTATESTENCILFUNCTION_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::StencilFunction.
RenderState::StencilFunction lua_enumFromString_RenderStateStencilFunction(const char* s);
const char* lua_stringFromEnum_RenderStateStencilFunction(RenderState::StencilFunction e);

}

#endif
