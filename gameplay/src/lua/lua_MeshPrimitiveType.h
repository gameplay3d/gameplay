#ifndef LUA_MESHPRIMITIVETYPE_H_
#define LUA_MESHPRIMITIVETYPE_H_

#include "Mesh.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Mesh::PrimitiveType.
Mesh::PrimitiveType lua_enumFromString_MeshPrimitiveType(const char* s);
const char* lua_stringFromEnum_MeshPrimitiveType(Mesh::PrimitiveType e);

}

#endif
