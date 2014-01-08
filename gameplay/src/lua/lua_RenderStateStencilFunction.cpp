#include "Base.h"
#include "lua_RenderStateStencilFunction.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateStencilFunction_STENCIL_NEVER = "STENCIL_NEVER";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_ALWAYS = "STENCIL_ALWAYS";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_LESS = "STENCIL_LESS";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_LEQUAL = "STENCIL_LEQUAL";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_EQUAL = "STENCIL_EQUAL";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_GREATER = "STENCIL_GREATER";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_GEQUAL = "STENCIL_GEQUAL";
static const char* luaEnumString_RenderStateStencilFunction_STENCIL_NOTEQUAL = "STENCIL_NOTEQUAL";

RenderState::StencilFunction lua_enumFromString_RenderStateStencilFunction(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_NEVER) == 0)
        return RenderState::STENCIL_NEVER;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_ALWAYS) == 0)
        return RenderState::STENCIL_ALWAYS;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_LESS) == 0)
        return RenderState::STENCIL_LESS;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_LEQUAL) == 0)
        return RenderState::STENCIL_LEQUAL;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_EQUAL) == 0)
        return RenderState::STENCIL_EQUAL;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_GREATER) == 0)
        return RenderState::STENCIL_GREATER;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_GEQUAL) == 0)
        return RenderState::STENCIL_GEQUAL;
    if (strcmp(s, luaEnumString_RenderStateStencilFunction_STENCIL_NOTEQUAL) == 0)
        return RenderState::STENCIL_NOTEQUAL;
    return RenderState::STENCIL_NEVER;
}

const char* lua_stringFromEnum_RenderStateStencilFunction(RenderState::StencilFunction e)
{
    if (e == RenderState::STENCIL_NEVER)
        return luaEnumString_RenderStateStencilFunction_STENCIL_NEVER;
    if (e == RenderState::STENCIL_ALWAYS)
        return luaEnumString_RenderStateStencilFunction_STENCIL_ALWAYS;
    if (e == RenderState::STENCIL_LESS)
        return luaEnumString_RenderStateStencilFunction_STENCIL_LESS;
    if (e == RenderState::STENCIL_LEQUAL)
        return luaEnumString_RenderStateStencilFunction_STENCIL_LEQUAL;
    if (e == RenderState::STENCIL_EQUAL)
        return luaEnumString_RenderStateStencilFunction_STENCIL_EQUAL;
    if (e == RenderState::STENCIL_GREATER)
        return luaEnumString_RenderStateStencilFunction_STENCIL_GREATER;
    if (e == RenderState::STENCIL_GEQUAL)
        return luaEnumString_RenderStateStencilFunction_STENCIL_GEQUAL;
    if (e == RenderState::STENCIL_NOTEQUAL)
        return luaEnumString_RenderStateStencilFunction_STENCIL_NOTEQUAL;
    return enumStringEmpty;
}

}

