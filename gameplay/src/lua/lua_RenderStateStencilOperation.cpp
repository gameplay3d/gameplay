#include "Base.h"
#include "lua_RenderStateStencilOperation.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_KEEP = "STENCIL_OP_KEEP";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_ZERO = "STENCIL_OP_ZERO";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_REPLACE = "STENCIL_OP_REPLACE";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR = "STENCIL_OP_INCR";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR = "STENCIL_OP_DECR";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_INVERT = "STENCIL_OP_INVERT";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR_WRAP = "STENCIL_OP_INCR_WRAP";
static const char* luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR_WRAP = "STENCIL_OP_DECR_WRAP";

RenderState::StencilOperation lua_enumFromString_RenderStateStencilOperation(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_KEEP) == 0)
        return RenderState::STENCIL_OP_KEEP;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_ZERO) == 0)
        return RenderState::STENCIL_OP_ZERO;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_REPLACE) == 0)
        return RenderState::STENCIL_OP_REPLACE;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR) == 0)
        return RenderState::STENCIL_OP_INCR;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR) == 0)
        return RenderState::STENCIL_OP_DECR;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_INVERT) == 0)
        return RenderState::STENCIL_OP_INVERT;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR_WRAP) == 0)
        return RenderState::STENCIL_OP_INCR_WRAP;
    if (strcmp(s, luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR_WRAP) == 0)
        return RenderState::STENCIL_OP_DECR_WRAP;
    return RenderState::STENCIL_OP_KEEP;
}

const char* lua_stringFromEnum_RenderStateStencilOperation(RenderState::StencilOperation e)
{
    if (e == RenderState::STENCIL_OP_KEEP)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_KEEP;
    if (e == RenderState::STENCIL_OP_ZERO)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_ZERO;
    if (e == RenderState::STENCIL_OP_REPLACE)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_REPLACE;
    if (e == RenderState::STENCIL_OP_INCR)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR;
    if (e == RenderState::STENCIL_OP_DECR)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR;
    if (e == RenderState::STENCIL_OP_INVERT)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_INVERT;
    if (e == RenderState::STENCIL_OP_INCR_WRAP)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_INCR_WRAP;
    if (e == RenderState::STENCIL_OP_DECR_WRAP)
        return luaEnumString_RenderStateStencilOperation_STENCIL_OP_DECR_WRAP;
    return enumStringEmpty;
}

}

