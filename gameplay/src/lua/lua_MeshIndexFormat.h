#ifndef LUA_MESHINDEXFORMAT_H_
#define LUA_MESHINDEXFORMAT_H_

#include "Mesh.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Mesh::IndexFormat.
Mesh::IndexFormat lua_enumFromString_MeshIndexFormat(const char* s);
const char* lua_stringFromEnum_MeshIndexFormat(Mesh::IndexFormat e);

}

#endif
