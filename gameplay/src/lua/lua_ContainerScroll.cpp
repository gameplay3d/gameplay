#include "Base.h"
#include "lua_ContainerScroll.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ContainerScroll_SCROLL_NONE = "SCROLL_NONE";
static const char* luaEnumString_ContainerScroll_SCROLL_HORIZONTAL = "SCROLL_HORIZONTAL";
static const char* luaEnumString_ContainerScroll_SCROLL_VERTICAL = "SCROLL_VERTICAL";
static const char* luaEnumString_ContainerScroll_SCROLL_BOTH = "SCROLL_BOTH";

Container::Scroll lua_enumFromString_ContainerScroll(const char* s)
{
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_NONE) == 0)
        return Container::SCROLL_NONE;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_HORIZONTAL) == 0)
        return Container::SCROLL_HORIZONTAL;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_VERTICAL) == 0)
        return Container::SCROLL_VERTICAL;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_BOTH) == 0)
        return Container::SCROLL_BOTH;
    return Container::SCROLL_NONE;
}

const char* lua_stringFromEnum_ContainerScroll(Container::Scroll e)
{
    if (e == Container::SCROLL_NONE)
        return luaEnumString_ContainerScroll_SCROLL_NONE;
    if (e == Container::SCROLL_HORIZONTAL)
        return luaEnumString_ContainerScroll_SCROLL_HORIZONTAL;
    if (e == Container::SCROLL_VERTICAL)
        return luaEnumString_ContainerScroll_SCROLL_VERTICAL;
    if (e == Container::SCROLL_BOTH)
        return luaEnumString_ContainerScroll_SCROLL_BOTH;
    return enumStringEmpty;
}

}

