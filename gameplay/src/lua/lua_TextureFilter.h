#ifndef LUA_TEXTUREFILTER_H_
#define LUA_TEXTUREFILTER_H_

#include "Texture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Texture::Filter.
Texture::Filter lua_enumFromString_TextureFilter(const char* s);
const char* lua_stringFromEnum_TextureFilter(Texture::Filter e);

}

#endif
