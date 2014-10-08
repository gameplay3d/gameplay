#ifndef LUA_TEXTURETYPE_H_
#define LUA_TEXTURETYPE_H_

#include "Texture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Texture::Type.
Texture::Type lua_enumFromString_TextureType(const char* s);
const char* lua_stringFromEnum_TextureType(Texture::Type e);

}

#endif
