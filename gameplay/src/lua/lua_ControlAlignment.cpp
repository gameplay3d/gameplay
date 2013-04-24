#include "Base.h"
#include "lua_ControlAlignment.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ControlAlignment_ALIGN_LEFT = "ALIGN_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_HCENTER = "ALIGN_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_RIGHT = "ALIGN_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP = "ALIGN_TOP";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER = "ALIGN_VCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM = "ALIGN_BOTTOM";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_LEFT = "ALIGN_TOP_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT = "ALIGN_VCENTER_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT = "ALIGN_BOTTOM_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER = "ALIGN_TOP_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER = "ALIGN_VCENTER_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER = "ALIGN_BOTTOM_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT = "ALIGN_TOP_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT = "ALIGN_VCENTER_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT = "ALIGN_BOTTOM_RIGHT";

Control::Alignment lua_enumFromString_ControlAlignment(const char* s)
{
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_LEFT) == 0)
        return Control::ALIGN_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_HCENTER) == 0)
        return Control::ALIGN_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_RIGHT) == 0)
        return Control::ALIGN_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP) == 0)
        return Control::ALIGN_TOP;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER) == 0)
        return Control::ALIGN_VCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM) == 0)
        return Control::ALIGN_BOTTOM;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_LEFT) == 0)
        return Control::ALIGN_TOP_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT) == 0)
        return Control::ALIGN_VCENTER_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT) == 0)
        return Control::ALIGN_BOTTOM_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER) == 0)
        return Control::ALIGN_TOP_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER) == 0)
        return Control::ALIGN_VCENTER_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER) == 0)
        return Control::ALIGN_BOTTOM_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT) == 0)
        return Control::ALIGN_TOP_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT) == 0)
        return Control::ALIGN_VCENTER_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT) == 0)
        return Control::ALIGN_BOTTOM_RIGHT;
    return Control::ALIGN_LEFT;
}

const char* lua_stringFromEnum_ControlAlignment(Control::Alignment e)
{
    if (e == Control::ALIGN_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_LEFT;
    if (e == Control::ALIGN_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_HCENTER;
    if (e == Control::ALIGN_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_RIGHT;
    if (e == Control::ALIGN_TOP)
        return luaEnumString_ControlAlignment_ALIGN_TOP;
    if (e == Control::ALIGN_VCENTER)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER;
    if (e == Control::ALIGN_BOTTOM)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM;
    if (e == Control::ALIGN_TOP_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_TOP_LEFT;
    if (e == Control::ALIGN_VCENTER_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT;
    if (e == Control::ALIGN_BOTTOM_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT;
    if (e == Control::ALIGN_TOP_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER;
    if (e == Control::ALIGN_VCENTER_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER;
    if (e == Control::ALIGN_BOTTOM_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER;
    if (e == Control::ALIGN_TOP_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT;
    if (e == Control::ALIGN_VCENTER_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT;
    if (e == Control::ALIGN_BOTTOM_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT;
    return enumStringEmpty;
}

}

