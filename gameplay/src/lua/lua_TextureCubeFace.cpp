#include "Base.h"
#include "lua_TextureCubeFace.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureCubeFace_POSITIVE_X = "POSITIVE_X";
static const char* luaEnumString_TextureCubeFace_NEGATIVE_X = "NEGATIVE_X";
static const char* luaEnumString_TextureCubeFace_POSITIVE_Y = "POSITIVE_Y";
static const char* luaEnumString_TextureCubeFace_NEGATIVE_Y = "NEGATIVE_Y";
static const char* luaEnumString_TextureCubeFace_POSITIVE_Z = "POSITIVE_Z";
static const char* luaEnumString_TextureCubeFace_NEGATIVE_Z = "NEGATIVE_Z";

Texture::CubeFace lua_enumFromString_TextureCubeFace(const char* s)
{
    if (strcmp(s, luaEnumString_TextureCubeFace_POSITIVE_X) == 0)
        return Texture::POSITIVE_X;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEGATIVE_X) == 0)
        return Texture::NEGATIVE_X;
    if (strcmp(s, luaEnumString_TextureCubeFace_POSITIVE_Y) == 0)
        return Texture::POSITIVE_Y;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEGATIVE_Y) == 0)
        return Texture::NEGATIVE_Y;
    if (strcmp(s, luaEnumString_TextureCubeFace_POSITIVE_Z) == 0)
        return Texture::POSITIVE_Z;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEGATIVE_Z) == 0)
        return Texture::NEGATIVE_Z;
    return Texture::POSITIVE_X;
}

const char* lua_stringFromEnum_TextureCubeFace(Texture::CubeFace e)
{
    if (e == Texture::POSITIVE_X)
        return luaEnumString_TextureCubeFace_POSITIVE_X;
    if (e == Texture::NEGATIVE_X)
        return luaEnumString_TextureCubeFace_NEGATIVE_X;
    if (e == Texture::POSITIVE_Y)
        return luaEnumString_TextureCubeFace_POSITIVE_Y;
    if (e == Texture::NEGATIVE_Y)
        return luaEnumString_TextureCubeFace_NEGATIVE_Y;
    if (e == Texture::POSITIVE_Z)
        return luaEnumString_TextureCubeFace_POSITIVE_Z;
    if (e == Texture::NEGATIVE_Z)
        return luaEnumString_TextureCubeFace_NEGATIVE_Z;
    return enumStringEmpty;
}

}

