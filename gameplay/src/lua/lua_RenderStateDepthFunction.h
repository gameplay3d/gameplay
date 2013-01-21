#ifndef LUA_RENDERSTATEDEPTHFUNCTION_H_
#define LUA_RENDERSTATEDEPTHFUNCTION_H_

#include "RenderState.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for RenderState::DepthFunction.
RenderState::DepthFunction lua_enumFromString_RenderStateDepthFunction(const char* s);
const char* lua_stringFromEnum_RenderStateDepthFunction(RenderState::DepthFunction e);

}

#endif
