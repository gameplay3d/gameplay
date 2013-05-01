#include "Base.h"
#include "lua_DepthStencilTargetFormat.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_DepthStencilTargetFormat_DEPTH = "DEPTH";
static const char* luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL = "DEPTH_STENCIL";

DepthStencilTarget::Format lua_enumFromString_DepthStencilTargetFormat(const char* s)
{
    if (strcmp(s, luaEnumString_DepthStencilTargetFormat_DEPTH) == 0)
        return DepthStencilTarget::DEPTH;
    if (strcmp(s, luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL) == 0)
        return DepthStencilTarget::DEPTH_STENCIL;
    return DepthStencilTarget::DEPTH;
}

const char* lua_stringFromEnum_DepthStencilTargetFormat(DepthStencilTarget::Format e)
{
    if (e == DepthStencilTarget::DEPTH)
        return luaEnumString_DepthStencilTargetFormat_DEPTH;
    if (e == DepthStencilTarget::DEPTH_STENCIL)
        return luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL;
    return enumStringEmpty;
}

}

