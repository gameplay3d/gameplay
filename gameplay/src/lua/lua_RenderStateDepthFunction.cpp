#include "Base.h"
#include "lua_RenderStateDepthFunction.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateDepthFunction_DEPTH_NEVER = "DEPTH_NEVER";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_LESS = "DEPTH_LESS";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_EQUAL = "DEPTH_EQUAL";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_LEQUAL = "DEPTH_LEQUAL";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_GREATER = "DEPTH_GREATER";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_NOTEQUAL = "DEPTH_NOTEQUAL";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_GEQUAL = "DEPTH_GEQUAL";
static const char* luaEnumString_RenderStateDepthFunction_DEPTH_ALWAYS = "DEPTH_ALWAYS";

RenderState::DepthFunction lua_enumFromString_RenderStateDepthFunction(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_NEVER) == 0)
        return RenderState::DEPTH_NEVER;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_LESS) == 0)
        return RenderState::DEPTH_LESS;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_EQUAL) == 0)
        return RenderState::DEPTH_EQUAL;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_LEQUAL) == 0)
        return RenderState::DEPTH_LEQUAL;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_GREATER) == 0)
        return RenderState::DEPTH_GREATER;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_NOTEQUAL) == 0)
        return RenderState::DEPTH_NOTEQUAL;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_GEQUAL) == 0)
        return RenderState::DEPTH_GEQUAL;
    if (strcmp(s, luaEnumString_RenderStateDepthFunction_DEPTH_ALWAYS) == 0)
        return RenderState::DEPTH_ALWAYS;
    return RenderState::DEPTH_NEVER;
}

const char* lua_stringFromEnum_RenderStateDepthFunction(RenderState::DepthFunction e)
{
    if (e == RenderState::DEPTH_NEVER)
        return luaEnumString_RenderStateDepthFunction_DEPTH_NEVER;
    if (e == RenderState::DEPTH_LESS)
        return luaEnumString_RenderStateDepthFunction_DEPTH_LESS;
    if (e == RenderState::DEPTH_EQUAL)
        return luaEnumString_RenderStateDepthFunction_DEPTH_EQUAL;
    if (e == RenderState::DEPTH_LEQUAL)
        return luaEnumString_RenderStateDepthFunction_DEPTH_LEQUAL;
    if (e == RenderState::DEPTH_GREATER)
        return luaEnumString_RenderStateDepthFunction_DEPTH_GREATER;
    if (e == RenderState::DEPTH_NOTEQUAL)
        return luaEnumString_RenderStateDepthFunction_DEPTH_NOTEQUAL;
    if (e == RenderState::DEPTH_GEQUAL)
        return luaEnumString_RenderStateDepthFunction_DEPTH_GEQUAL;
    if (e == RenderState::DEPTH_ALWAYS)
        return luaEnumString_RenderStateDepthFunction_DEPTH_ALWAYS;
    return enumStringEmpty;
}

}

