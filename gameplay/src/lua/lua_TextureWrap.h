#ifndef LUA_TEXTUREWRAP_H_
#define LUA_TEXTUREWRAP_H_

#include "Texture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Texture::Wrap.
Texture::Wrap lua_enumFromString_TextureWrap(const char* s);
const char* lua_stringFromEnum_TextureWrap(Texture::Wrap e);

}

#endif
