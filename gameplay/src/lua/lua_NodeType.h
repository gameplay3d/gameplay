#ifndef LUA_NODETYPE_H_
#define LUA_NODETYPE_H_

#include "Node.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Node::Type.
Node::Type lua_enumFromString_NodeType(const char* s);
const char* lua_stringFromEnum_NodeType(Node::Type e);

}

#endif
