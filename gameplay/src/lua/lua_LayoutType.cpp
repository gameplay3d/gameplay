#include "Base.h"
#include "lua_LayoutType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_LayoutType_LAYOUT_FLOW = "LAYOUT_FLOW";
static const char* luaEnumString_LayoutType_LAYOUT_VERTICAL = "LAYOUT_VERTICAL";
static const char* luaEnumString_LayoutType_LAYOUT_ABSOLUTE = "LAYOUT_ABSOLUTE";

Layout::Type lua_enumFromString_LayoutType(const char* s)
{
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_FLOW) == 0)
        return Layout::LAYOUT_FLOW;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_VERTICAL) == 0)
        return Layout::LAYOUT_VERTICAL;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_ABSOLUTE) == 0)
        return Layout::LAYOUT_ABSOLUTE;
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
    return enumStringEmpty;
}

}

