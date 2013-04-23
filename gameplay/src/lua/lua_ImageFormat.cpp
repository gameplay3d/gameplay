#include "Base.h"
#include "lua_ImageFormat.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ImageFormat_RGB = "RGB";
static const char* luaEnumString_ImageFormat_RGBA = "RGBA";

Image::Format lua_enumFromString_ImageFormat(const char* s)
{
    if (strcmp(s, luaEnumString_ImageFormat_RGB) == 0)
        return Image::RGB;
    if (strcmp(s, luaEnumString_ImageFormat_RGBA) == 0)
        return Image::RGBA;
    return Image::RGB;
}

const char* lua_stringFromEnum_ImageFormat(Image::Format e)
{
    if (e == Image::RGB)
        return luaEnumString_ImageFormat_RGB;
    if (e == Image::RGBA)
        return luaEnumString_ImageFormat_RGBA;
    return enumStringEmpty;
}

}

