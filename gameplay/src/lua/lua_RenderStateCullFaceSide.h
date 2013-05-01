#ifndef LUA_RENDERSTATECULLFACESIDE_H_
#define LUA_RENDERSTATECULLFACESIDE_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::CullFaceSide.
RenderState::CullFaceSide lua_enumFromString_RenderStateCullFaceSide(const char* s);
const char* lua_stringFromEnum_RenderStateCullFaceSide(RenderState::CullFaceSide e);

}

#endif
