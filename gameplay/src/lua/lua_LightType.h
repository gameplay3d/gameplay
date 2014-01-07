#ifndef LUA_LIGHTTYPE_H_
#define LUA_LIGHTTYPE_H_

#include "Light.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Light::Type.
Light::Type lua_enumFromString_LightType(const char* s);
const char* lua_stringFromEnum_LightType(Light::Type e);

}

#endif
