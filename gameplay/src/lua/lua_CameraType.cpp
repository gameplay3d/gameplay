#include "Base.h"
#include "lua_CameraType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_CameraType_PERSPECTIVE = "PERSPECTIVE";
static const char* luaEnumString_CameraType_ORTHOGRAPHIC = "ORTHOGRAPHIC";

Camera::Type lua_enumFromString_CameraType(const char* s)
{
    if (strcmp(s, luaEnumString_CameraType_PERSPECTIVE) == 0)
        return Camera::PERSPECTIVE;
    if (strcmp(s, luaEnumString_CameraType_ORTHOGRAPHIC) == 0)
        return Camera::ORTHOGRAPHIC;
    return Camera::PERSPECTIVE;
}

const char* lua_stringFromEnum_CameraType(Camera::Type e)
{
    if (e == Camera::PERSPECTIVE)
        return luaEnumString_CameraType_PERSPECTIVE;
    if (e == Camera::ORTHOGRAPHIC)
        return luaEnumString_CameraType_ORTHOGRAPHIC;
    return enumStringEmpty;
}

}

