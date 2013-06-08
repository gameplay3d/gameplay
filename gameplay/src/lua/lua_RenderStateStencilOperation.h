#ifndef LUA_RENDERSTATESTENCILOPERATION_H_
#define LUA_RENDERSTATESTENCILOPERATION_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::StencilOperation.
RenderState::StencilOperation lua_enumFromString_RenderStateStencilOperation(const char* s);
const char* lua_stringFromEnum_RenderStateStencilOperation(RenderState::StencilOperation e);

}

#endif
