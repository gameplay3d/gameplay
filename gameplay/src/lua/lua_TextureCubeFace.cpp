#include "Base.h"
#include "lua_TextureCubeFace.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureCubeFace_POS_X = "POS_X";
static const char* luaEnumString_TextureCubeFace_NEG_X = "NEG_X";
static const char* luaEnumString_TextureCubeFace_POS_Y = "POS_Y";
static const char* luaEnumString_TextureCubeFace_NEG_Y = "NEG_Y";
static const char* luaEnumString_TextureCubeFace_POS_Z = "POS_Z";
static const char* luaEnumString_TextureCubeFace_NEG_Z = "NEG_Z";

Texture::CubeFace lua_enumFromString_TextureCubeFace(const char* s)
{
    if (strcmp(s, luaEnumString_TextureCubeFace_POS_X) == 0)
        return Texture::POS_X;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEG_X) == 0)
        return Texture::NEG_X;
    if (strcmp(s, luaEnumString_TextureCubeFace_POS_Y) == 0)
        return Texture::POS_Y;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEG_Y) == 0)
        return Texture::NEG_Y;
    if (strcmp(s, luaEnumString_TextureCubeFace_POS_Z) == 0)
        return Texture::POS_Z;
    if (strcmp(s, luaEnumString_TextureCubeFace_NEG_Z) == 0)
        return Texture::NEG_Z;
    return Texture::POS_X;
}

const char* lua_stringFromEnum_TextureCubeFace(Texture::CubeFace e)
{
    if (e == Texture::POS_X)
        return luaEnumString_TextureCubeFace_POS_X;
    if (e == Texture::NEG_X)
        return luaEnumString_TextureCubeFace_NEG_X;
    if (e == Texture::POS_Y)
        return luaEnumString_TextureCubeFace_POS_Y;
    if (e == Texture::NEG_Y)
        return luaEnumString_TextureCubeFace_NEG_Y;
    if (e == Texture::POS_Z)
        return luaEnumString_TextureCubeFace_POS_Z;
    if (e == Texture::NEG_Z)
        return luaEnumString_TextureCubeFace_NEG_Z;
    return enumStringEmpty;
}

}

