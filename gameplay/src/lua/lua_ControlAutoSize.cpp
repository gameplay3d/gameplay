#include "Base.h"
#include "lua_ControlAutoSize.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_NONE = "AUTO_SIZE_NONE";
static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_WIDTH = "AUTO_SIZE_WIDTH";
static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_HEIGHT = "AUTO_SIZE_HEIGHT";
static const char* luaEnumString_ControlAutoSize_AUTO_SIZE_BOTH = "AUTO_SIZE_BOTH";

Control::AutoSize lua_enumFromString_ControlAutoSize(const char* s)
{
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_NONE) == 0)
        return Control::AUTO_SIZE_NONE;
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_WIDTH) == 0)
        return Control::AUTO_SIZE_WIDTH;
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_HEIGHT) == 0)
        return Control::AUTO_SIZE_HEIGHT;
    if (strcmp(s, luaEnumString_ControlAutoSize_AUTO_SIZE_BOTH) == 0)
        return Control::AUTO_SIZE_BOTH;
    return Control::AUTO_SIZE_NONE;
}

const char* lua_stringFromEnum_ControlAutoSize(Control::AutoSize e)
{
    if (e == Control::AUTO_SIZE_NONE)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_NONE;
    if (e == Control::AUTO_SIZE_WIDTH)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_WIDTH;
    if (e == Control::AUTO_SIZE_HEIGHT)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_HEIGHT;
    if (e == Control::AUTO_SIZE_BOTH)
        return luaEnumString_ControlAutoSize_AUTO_SIZE_BOTH;
    return enumStringEmpty;
}

}

