#ifndef LUA_TEXTURECUBEFACE_H_
#define LUA_TEXTURECUBEFACE_H_

#include "Texture.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Texture::CubeFace.
Texture::CubeFace lua_enumFromString_TextureCubeFace(const char* s);
const char* lua_stringFromEnum_TextureCubeFace(Texture::CubeFace e);

}

#endif
