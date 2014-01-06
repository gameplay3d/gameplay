#ifndef LUA_CAMERATYPE_H_
#define LUA_CAMERATYPE_H_

#include "Camera.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Camera::Type.
Camera::Type lua_enumFromString_CameraType(const char* s);
const char* lua_stringFromEnum_CameraType(Camera::Type e);

}

#endif
