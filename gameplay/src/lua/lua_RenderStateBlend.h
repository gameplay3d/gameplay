#ifndef LUA_RENDERSTATEBLEND_H_
#define LUA_RENDERSTATEBLEND_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::Blend.
RenderState::Blend lua_enumFromString_RenderStateBlend(const char* s);
const char* lua_stringFromEnum_RenderStateBlend(RenderState::Blend e);

}

#endif
