#include "Base.h"
#include "lua_VertexFormatUsage.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_VertexFormatUsage_POSITION = "POSITION";
static const char* luaEnumString_VertexFormatUsage_NORMAL = "NORMAL";
static const char* luaEnumString_VertexFormatUsage_COLOR = "COLOR";
static const char* luaEnumString_VertexFormatUsage_TANGENT = "TANGENT";
static const char* luaEnumString_VertexFormatUsage_BINORMAL = "BINORMAL";
static const char* luaEnumString_VertexFormatUsage_BLENDWEIGHTS = "BLENDWEIGHTS";
static const char* luaEnumString_VertexFormatUsage_BLENDINDICES = "BLENDINDICES";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD0 = "TEXCOORD0";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD1 = "TEXCOORD1";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD2 = "TEXCOORD2";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD3 = "TEXCOORD3";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD4 = "TEXCOORD4";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD5 = "TEXCOORD5";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD6 = "TEXCOORD6";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD7 = "TEXCOORD7";

VertexFormat::Usage lua_enumFromString_VertexFormatUsage(const char* s)
{
    if (strcmp(s, luaEnumString_VertexFormatUsage_POSITION) == 0)
        return VertexFormat::POSITION;
    if (strcmp(s, luaEnumString_VertexFormatUsage_NORMAL) == 0)
        return VertexFormat::NORMAL;
    if (strcmp(s, luaEnumString_VertexFormatUsage_COLOR) == 0)
        return VertexFormat::COLOR;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TANGENT) == 0)
        return VertexFormat::TANGENT;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BINORMAL) == 0)
        return VertexFormat::BINORMAL;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BLENDWEIGHTS) == 0)
        return VertexFormat::BLENDWEIGHTS;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BLENDINDICES) == 0)
        return VertexFormat::BLENDINDICES;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD0) == 0)
        return VertexFormat::TEXCOORD0;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD1) == 0)
        return VertexFormat::TEXCOORD1;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD2) == 0)
        return VertexFormat::TEXCOORD2;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD3) == 0)
        return VertexFormat::TEXCOORD3;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD4) == 0)
        return VertexFormat::TEXCOORD4;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD5) == 0)
        return VertexFormat::TEXCOORD5;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD6) == 0)
        return VertexFormat::TEXCOORD6;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD7) == 0)
        return VertexFormat::TEXCOORD7;
    return VertexFormat::POSITION;
}

const char* lua_stringFromEnum_VertexFormatUsage(VertexFormat::Usage e)
{
    if (e == VertexFormat::POSITION)
        return luaEnumString_VertexFormatUsage_POSITION;
    if (e == VertexFormat::NORMAL)
        return luaEnumString_VertexFormatUsage_NORMAL;
    if (e == VertexFormat::COLOR)
        return luaEnumString_VertexFormatUsage_COLOR;
    if (e == VertexFormat::TANGENT)
        return luaEnumString_VertexFormatUsage_TANGENT;
    if (e == VertexFormat::BINORMAL)
        return luaEnumString_VertexFormatUsage_BINORMAL;
    if (e == VertexFormat::BLENDWEIGHTS)
        return luaEnumString_VertexFormatUsage_BLENDWEIGHTS;
    if (e == VertexFormat::BLENDINDICES)
        return luaEnumString_VertexFormatUsage_BLENDINDICES;
    if (e == VertexFormat::TEXCOORD0)
        return luaEnumString_VertexFormatUsage_TEXCOORD0;
    if (e == VertexFormat::TEXCOORD1)
        return luaEnumString_VertexFormatUsage_TEXCOORD1;
    if (e == VertexFormat::TEXCOORD2)
        return luaEnumString_VertexFormatUsage_TEXCOORD2;
    if (e == VertexFormat::TEXCOORD3)
        return luaEnumString_VertexFormatUsage_TEXCOORD3;
    if (e == VertexFormat::TEXCOORD4)
        return luaEnumString_VertexFormatUsage_TEXCOORD4;
    if (e == VertexFormat::TEXCOORD5)
        return luaEnumString_VertexFormatUsage_TEXCOORD5;
    if (e == VertexFormat::TEXCOORD6)
        return luaEnumString_VertexFormatUsage_TEXCOORD6;
    if (e == VertexFormat::TEXCOORD7)
        return luaEnumString_VertexFormatUsage_TEXCOORD7;
    return enumStringEmpty;
}

}

