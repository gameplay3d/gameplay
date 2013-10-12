#ifndef LUA_RENDERSTATEFRONTFACE_H_
#define LUA_RENDERSTATEFRONTFACE_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::FrontFace.
RenderState::FrontFace lua_enumFromString_RenderStateFrontFace(const char* s);
const char* lua_stringFromEnum_RenderStateFrontFace(RenderState::FrontFace e);

}

#endif
