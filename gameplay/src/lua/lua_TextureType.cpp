#include "Base.h"
#include "lua_TextureType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureType_TEXTURE_2D = "TEXTURE_2D";
static const char* luaEnumString_TextureType_TEXTURE_CUBE = "TEXTURE_CUBE";

Texture::Type lua_enumFromString_TextureType(const char* s)
{
    if (strcmp(s, luaEnumString_TextureType_TEXTURE_2D) == 0)
        return Texture::TEXTURE_2D;
    if (strcmp(s, luaEnumString_TextureType_TEXTURE_CUBE) == 0)
        return Texture::TEXTURE_CUBE;
    return Texture::TEXTURE_2D;
}

const char* lua_stringFromEnum_TextureType(Texture::Type e)
{
    if (e == Texture::TEXTURE_2D)
        return luaEnumString_TextureType_TEXTURE_2D;
    if (e == Texture::TEXTURE_CUBE)
        return luaEnumString_TextureType_TEXTURE_CUBE;
    return enumStringEmpty;
}

}

