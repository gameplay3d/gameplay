#include "Base.h"
#include "lua_LayoutType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_LayoutType_LAYOUT_FLOW = "LAYOUT_FLOW";
static const char* luaEnumString_LayoutType_LAYOUT_VERTICAL = "LAYOUT_VERTICAL";
static const char* luaEnumString_LayoutType_LAYOUT_ABSOLUTE = "LAYOUT_ABSOLUTE";
static const char* luaEnumString_LayoutType_LAYOUT_SCROLL = "LAYOUT_SCROLL";

Layout::Type lua_enumFromString_LayoutType(const char* s)
{
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_FLOW) == 0)
        return Layout::LAYOUT_FLOW;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_VERTICAL) == 0)
        return Layout::LAYOUT_VERTICAL;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_ABSOLUTE) == 0)
        return Layout::LAYOUT_ABSOLUTE;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_SCROLL) == 0)
        return Layout::LAYOUT_SCROLL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Layout::Type.", s);
    return Layout::LAYOUT_FLOW;
}

const char* lua_stringFromEnum_LayoutType(Layout::Type e)
{
    if (e == Layout::LAYOUT_FLOW)
        return luaEnumString_LayoutType_LAYOUT_FLOW;
    if (e == Layout::LAYOUT_VERTICAL)
        return luaEnumString_LayoutType_LAYOUT_VERTICAL;
    if (e == Layout::LAYOUT_ABSOLUTE)
        return luaEnumString_LayoutType_LAYOUT_ABSOLUTE;
    if (e == Layout::LAYOUT_SCROLL)
        return luaEnumString_LayoutType_LAYOUT_SCROLL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Layout::Type.", e);
    return enumStringEmpty;
}

}

