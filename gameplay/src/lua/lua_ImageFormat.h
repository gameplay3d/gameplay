#ifndef LUA_IMAGEFORMAT_H_
#define LUA_IMAGEFORMAT_H_

#include "Image.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Image::Format.
Image::Format lua_enumFromString_ImageFormat(const char* s);
const char* lua_stringFromEnum_ImageFormat(Image::Format e);

}

#endif
