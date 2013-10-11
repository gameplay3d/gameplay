#ifndef LUA_FONTFORMAT_H_
#define LUA_FONTFORMAT_H_

#include "Font.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Font::Format.
Font::Format lua_enumFromString_FontFormat(const char* s);
const char* lua_stringFromEnum_FontFormat(Font::Format e);

}

#endif
