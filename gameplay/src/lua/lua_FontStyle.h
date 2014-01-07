#ifndef LUA_FONTSTYLE_H_
#define LUA_FONTSTYLE_H_

#include "Font.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Font::Style.
Font::Style lua_enumFromString_FontStyle(const char* s);
const char* lua_stringFromEnum_FontStyle(Font::Style e);

}

#endif
