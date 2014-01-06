#ifndef LUA_TEXTUREFORMAT_H_
#define LUA_TEXTUREFORMAT_H_

#include "Texture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Texture::Format.
Texture::Format lua_enumFromString_TextureFormat(const char* s);
const char* lua_stringFromEnum_TextureFormat(Texture::Format e);

}

#endif
