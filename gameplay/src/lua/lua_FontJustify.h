#ifndef LUA_FONTJUSTIFY_H_
#define LUA_FONTJUSTIFY_H_

#include "Font.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Font::Justify.
Font::Justify lua_enumFromString_FontJustify(const char* s);
const char* lua_stringFromEnum_FontJustify(Font::Justify e);

}

#endif
