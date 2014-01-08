#ifndef LUA_CURVEINTERPOLATIONTYPE_H_
#define LUA_CURVEINTERPOLATIONTYPE_H_

#include "Curve.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Curve::InterpolationType.
Curve::InterpolationType lua_enumFromString_CurveInterpolationType(const char* s);
const char* lua_stringFromEnum_CurveInterpolationType(Curve::InterpolationType e);

}

#endif
