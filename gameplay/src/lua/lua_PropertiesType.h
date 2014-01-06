#ifndef LUA_PROPERTIESTYPE_H_
#define LUA_PROPERTIESTYPE_H_

#include "Properties.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Properties::Type.
Properties::Type lua_enumFromString_PropertiesType(const char* s);
const char* lua_stringFromEnum_PropertiesType(Properties::Type e);

}

#endif
