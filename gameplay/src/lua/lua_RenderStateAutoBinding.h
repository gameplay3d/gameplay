#ifndef LUA_RENDERSTATEAUTOBINDING_H_
#define LUA_RENDERSTATEAUTOBINDING_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::AutoBinding.
RenderState::AutoBinding lua_enumFromString_RenderStateAutoBinding(const char* s);
const char* lua_stringFromEnum_RenderStateAutoBinding(RenderState::AutoBinding e);

}

#endif
