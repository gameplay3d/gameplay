#ifndef LUA_DEPTHSTENCILTARGETFORMAT_H_
#define LUA_DEPTHSTENCILTARGETFORMAT_H_

#include "DepthStencilTarget.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for DepthStencilTarget::Format.
DepthStencilTarget::Format lua_enumFromString_DepthStencilTargetFormat(const char* s);
const char* lua_stringFromEnum_DepthStencilTargetFormat(DepthStencilTarget::Format e);

}

#endif
