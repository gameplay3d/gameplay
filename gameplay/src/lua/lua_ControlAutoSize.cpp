#include "Base.h"
#include "lua_ControlAutoSize.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_NONE = "AUTO_SIZE_NONE";
static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_STRETCH = "AUTO_SIZE_STRETCH";
static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_FIT = "AUTO_SIZE_FIT";

Control::AutoSize lua_enumFromString_ControlAutoSize(const char* s)
{
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_NONE) == 0)
        return Control::AUTO_SIZE_NONE;
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_STRETCH) == 0)
        return Control::AUTO_SIZE_STRETCH;
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_FIT) == 0)
        return Control::AUTO_SIZE_FIT;
    return Control::AUTO_SIZE_NONE;
}

const char* lua_stringFromEnum_ControlAutoSize(Control::AutoSize e)
{
    if (e == Control::AUTO_SIZE_NONE)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_NONE;
    if (e == Control::AUTO_SIZE_STRETCH)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_STRETCH;
    if (e == Control::AUTO_SIZE_FIT)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_FIT;
    return enumStringEmpty;
}

}

