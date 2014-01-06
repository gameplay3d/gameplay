#ifndef LUA_LAYOUTTYPE_H_
#define LUA_LAYOUTTYPE_H_

#include "Layout.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Layout::Type.
Layout::Type lua_enumFromString_LayoutType(const char* s);
const char* lua_stringFromEnum_LayoutType(Layout::Type e);

}

#endif
