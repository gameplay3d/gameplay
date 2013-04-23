#include "Base.h"
#include "lua_TextureFilter.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureFilter_NEAREST = "NEAREST";
static const char* luaEnumString_TextureFilter_LINEAR = "LINEAR";
static const char* luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST = "NEAREST_MIPMAP_NEAREST";
static const char* luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST = "LINEAR_MIPMAP_NEAREST";
static const char* luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR = "NEAREST_MIPMAP_LINEAR";
static const char* luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR = "LINEAR_MIPMAP_LINEAR";

Texture::Filter lua_enumFromString_TextureFilter(const char* s)
{
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST) == 0)
        return Texture::NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR) == 0)
        return Texture::LINEAR;
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST) == 0)
        return Texture::NEAREST_MIPMAP_NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST) == 0)
        return Texture::LINEAR_MIPMAP_NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR) == 0)
        return Texture::NEAREST_MIPMAP_LINEAR;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR) == 0)
        return Texture::LINEAR_MIPMAP_LINEAR;
    return Texture::NEAREST;
}

const char* lua_stringFromEnum_TextureFilter(Texture::Filter e)
{
    if (e == Texture::NEAREST)
        return luaEnumString_TextureFilter_NEAREST;
    if (e == Texture::LINEAR)
        return luaEnumString_TextureFilter_LINEAR;
    if (e == Texture::NEAREST_MIPMAP_NEAREST)
        return luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST;
    if (e == Texture::LINEAR_MIPMAP_NEAREST)
        return luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST;
    if (e == Texture::NEAREST_MIPMAP_LINEAR)
        return luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR;
    if (e == Texture::LINEAR_MIPMAP_LINEAR)
        return luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR;
    return enumStringEmpty;
}

}

