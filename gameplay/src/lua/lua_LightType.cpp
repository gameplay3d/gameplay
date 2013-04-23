#include "Base.h"
#include "lua_LightType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_LightType_DIRECTIONAL = "DIRECTIONAL";
static const char* luaEnumString_LightType_POINT = "POINT";
static const char* luaEnumString_LightType_SPOT = "SPOT";

Light::Type lua_enumFromString_LightType(const char* s)
{
    if (strcmp(s, luaEnumString_LightType_DIRECTIONAL) == 0)
        return Light::DIRECTIONAL;
    if (strcmp(s, luaEnumString_LightType_POINT) == 0)
        return Light::POINT;
    if (strcmp(s, luaEnumString_LightType_SPOT) == 0)
        return Light::SPOT;
    return Light::DIRECTIONAL;
}

const char* lua_stringFromEnum_LightType(Light::Type e)
{
    if (e == Light::DIRECTIONAL)
        return luaEnumString_LightType_DIRECTIONAL;
    if (e == Light::POINT)
        return luaEnumString_LightType_POINT;
    if (e == Light::SPOT)
        return luaEnumString_LightType_SPOT;
    return enumStringEmpty;
}

}

