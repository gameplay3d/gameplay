#ifndef LUA_CONTAINERSCROLL_H_
#define LUA_CONTAINERSCROLL_H_

#include "Container.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Container::Scroll.
Container::Scroll lua_enumFromString_ContainerScroll(const char* s);
const char* lua_stringFromEnum_ContainerScroll(Container::Scroll e);

}

#endif
