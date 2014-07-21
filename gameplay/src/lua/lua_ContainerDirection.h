#ifndef LUA_CONTAINERDIRECTION_H_
#define LUA_CONTAINERDIRECTION_H_

#include "Container.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Container::Direction.
Container::Direction lua_enumFromString_ContainerDirection(const char* s);
const char* lua_stringFromEnum_ContainerDirection(Container::Direction e);

}

#endif
