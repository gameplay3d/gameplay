#ifndef LUA_VERTEXFORMATUSAGE_H_
#define LUA_VERTEXFORMATUSAGE_H_

#include "VertexFormat.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for VertexFormat::Usage.
VertexFormat::Usage lua_enumFromString_VertexFormatUsage(const char* s);
const char* lua_stringFromEnum_VertexFormatUsage(VertexFormat::Usage e);

}

#endif
