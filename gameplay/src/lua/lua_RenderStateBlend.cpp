#include "Base.h"
#include "lua_RenderStateBlend.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateBlend_BLEND_ZERO = "BLEND_ZERO";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE = "BLEND_ONE";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_COLOR = "BLEND_SRC_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_COLOR = "BLEND_ONE_MINUS_SRC_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_DST_COLOR = "BLEND_DST_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR = "BLEND_ONE_MINUS_DST_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA = "BLEND_SRC_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA = "BLEND_ONE_MINUS_SRC_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_DST_ALPHA = "BLEND_DST_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA = "BLEND_ONE_MINUS_DST_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA = "BLEND_CONSTANT_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA = "BLEND_ONE_MINUS_CONSTANT_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE = "BLEND_SRC_ALPHA_SATURATE";

RenderState::Blend lua_enumFromString_RenderStateBlend(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ZERO) == 0)
        return RenderState::BLEND_ZERO;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE) == 0)
        return RenderState::BLEND_ONE;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_COLOR) == 0)
        return RenderState::BLEND_SRC_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_COLOR) == 0)
        return RenderState::BLEND_ONE_MINUS_SRC_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_DST_COLOR) == 0)
        return RenderState::BLEND_DST_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR) == 0)
        return RenderState::BLEND_ONE_MINUS_DST_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA) == 0)
        return RenderState::BLEND_SRC_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_SRC_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_DST_ALPHA) == 0)
        return RenderState::BLEND_DST_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_DST_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA) == 0)
        return RenderState::BLEND_CONSTANT_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE) == 0)
        return RenderState::BLEND_SRC_ALPHA_SATURATE;
    return RenderState::BLEND_ZERO;
}

const char* lua_stringFromEnum_RenderStateBlend(RenderState::Blend e)
{
    if (e == RenderState::BLEND_ZERO)
        return luaEnumString_RenderStateBlend_BLEND_ZERO;
    if (e == RenderState::BLEND_ONE)
        return luaEnumString_RenderStateBlend_BLEND_ONE;
    if (e == RenderState::BLEND_SRC_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_SRC_COLOR;
    if (e == RenderState::BLEND_ONE_MINUS_SRC_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_COLOR;
    if (e == RenderState::BLEND_DST_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_DST_COLOR;
    if (e == RenderState::BLEND_ONE_MINUS_DST_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR;
    if (e == RenderState::BLEND_SRC_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_SRC_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA;
    if (e == RenderState::BLEND_DST_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_DST_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_DST_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA;
    if (e == RenderState::BLEND_CONSTANT_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (e == RenderState::BLEND_SRC_ALPHA_SATURATE)
        return luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE;
    return enumStringEmpty;
}

}

