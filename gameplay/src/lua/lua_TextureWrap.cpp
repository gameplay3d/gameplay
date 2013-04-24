#include "Base.h"
#include "lua_TextureWrap.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextureWrap_REPEAT = "REPEAT";
static const char* luaEnumString_TextureWrap_CLAMP = "CLAMP";

Texture::Wrap lua_enumFromString_TextureWrap(const char* s)
{
    if (strcmp(s, luaEnumString_TextureWrap_REPEAT) == 0)
        return Texture::REPEAT;
    if (strcmp(s, luaEnumString_TextureWrap_CLAMP) == 0)
        return Texture::CLAMP;
    return Texture::REPEAT;
}

const char* lua_stringFromEnum_TextureWrap(Texture::Wrap e)
{
    if (e == Texture::REPEAT)
        return luaEnumString_TextureWrap_REPEAT;
    if (e == Texture::CLAMP)
        return luaEnumString_TextureWrap_CLAMP;
    return enumStringEmpty;
}

}

