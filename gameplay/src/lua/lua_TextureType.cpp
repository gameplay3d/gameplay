#include "Base.h"
#include "lua_TextureType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureType_TEX_UNKNOWN = "TEX_UNKNOWN";
static const char* luaEnumString_TextureType_TEX_2D = "TEX_2D";
static const char* luaEnumString_TextureType_TEX_CUBE = "TEX_CUBE";

Texture::Type lua_enumFromString_TextureType(const char* s)
{
    if (strcmp(s, luaEnumString_TextureType_TEX_UNKNOWN) == 0)
        return Texture::TEX_UNKNOWN;
    if (strcmp(s, luaEnumString_TextureType_TEX_2D) == 0)
        return Texture::TEX_2D;
    if (strcmp(s, luaEnumString_TextureType_TEX_CUBE) == 0)
        return Texture::TEX_CUBE;
    return Texture::TEX_UNKNOWN;
}

const char* lua_stringFromEnum_TextureType(Texture::Type e)
{
    if (e == Texture::TEX_UNKNOWN)
        return luaEnumString_TextureType_TEX_UNKNOWN;
    if (e == Texture::TEX_2D)
        return luaEnumString_TextureType_TEX_2D;
    if (e == Texture::TEX_CUBE)
        return luaEnumString_TextureType_TEX_CUBE;
    return enumStringEmpty;
}

}

