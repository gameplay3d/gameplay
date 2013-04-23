#include "Base.h"
#include "lua_CurveInterpolationType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_CurveInterpolationType_BEZIER = "BEZIER";
static const char* luaEnumString_CurveInterpolationType_BSPLINE = "BSPLINE";
static const char* luaEnumString_CurveInterpolationType_FLAT = "FLAT";
static const char* luaEnumString_CurveInterpolationType_HERMITE = "HERMITE";
static const char* luaEnumString_CurveInterpolationType_LINEAR = "LINEAR";
static const char* luaEnumString_CurveInterpolationType_SMOOTH = "SMOOTH";
static const char* luaEnumString_CurveInterpolationType_STEP = "STEP";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_IN = "QUADRATIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_OUT = "QUADRATIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT = "QUADRATIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN = "QUADRATIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_CUBIC_IN = "CUBIC_IN";
static const char* luaEnumString_CurveInterpolationType_CUBIC_OUT = "CUBIC_OUT";
static const char* luaEnumString_CurveInterpolationType_CUBIC_IN_OUT = "CUBIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_CUBIC_OUT_IN = "CUBIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_IN = "QUARTIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_OUT = "QUARTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT = "QUARTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN = "QUARTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_IN = "QUINTIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_OUT = "QUINTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT = "QUINTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN = "QUINTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_SINE_IN = "SINE_IN";
static const char* luaEnumString_CurveInterpolationType_SINE_OUT = "SINE_OUT";
static const char* luaEnumString_CurveInterpolationType_SINE_IN_OUT = "SINE_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_SINE_OUT_IN = "SINE_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_IN = "EXPONENTIAL_IN";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT = "EXPONENTIAL_OUT";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT = "EXPONENTIAL_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN = "EXPONENTIAL_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_IN = "CIRCULAR_IN";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_OUT = "CIRCULAR_OUT";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT = "CIRCULAR_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN = "CIRCULAR_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_IN = "ELASTIC_IN";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_OUT = "ELASTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT = "ELASTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN = "ELASTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_IN = "OVERSHOOT_IN";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_OUT = "OVERSHOOT_OUT";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT = "OVERSHOOT_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN = "OVERSHOOT_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_IN = "BOUNCE_IN";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_OUT = "BOUNCE_OUT";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT = "BOUNCE_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN = "BOUNCE_OUT_IN";

Curve::InterpolationType lua_enumFromString_CurveInterpolationType(const char* s)
{
    if (strcmp(s, luaEnumString_CurveInterpolationType_BEZIER) == 0)
        return Curve::BEZIER;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BSPLINE) == 0)
        return Curve::BSPLINE;
    if (strcmp(s, luaEnumString_CurveInterpolationType_FLAT) == 0)
        return Curve::FLAT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_HERMITE) == 0)
        return Curve::HERMITE;
    if (strcmp(s, luaEnumString_CurveInterpolationType_LINEAR) == 0)
        return Curve::LINEAR;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SMOOTH) == 0)
        return Curve::SMOOTH;
    if (strcmp(s, luaEnumString_CurveInterpolationType_STEP) == 0)
        return Curve::STEP;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_IN) == 0)
        return Curve::QUADRATIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_OUT) == 0)
        return Curve::QUADRATIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT) == 0)
        return Curve::QUADRATIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN) == 0)
        return Curve::QUADRATIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_IN) == 0)
        return Curve::CUBIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_OUT) == 0)
        return Curve::CUBIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_IN_OUT) == 0)
        return Curve::CUBIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_OUT_IN) == 0)
        return Curve::CUBIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_IN) == 0)
        return Curve::QUARTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_OUT) == 0)
        return Curve::QUARTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT) == 0)
        return Curve::QUARTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN) == 0)
        return Curve::QUARTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_IN) == 0)
        return Curve::QUINTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_OUT) == 0)
        return Curve::QUINTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT) == 0)
        return Curve::QUINTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN) == 0)
        return Curve::QUINTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_IN) == 0)
        return Curve::SINE_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_OUT) == 0)
        return Curve::SINE_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_IN_OUT) == 0)
        return Curve::SINE_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_OUT_IN) == 0)
        return Curve::SINE_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_IN) == 0)
        return Curve::EXPONENTIAL_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT) == 0)
        return Curve::EXPONENTIAL_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT) == 0)
        return Curve::EXPONENTIAL_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN) == 0)
        return Curve::EXPONENTIAL_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_IN) == 0)
        return Curve::CIRCULAR_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_OUT) == 0)
        return Curve::CIRCULAR_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT) == 0)
        return Curve::CIRCULAR_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN) == 0)
        return Curve::CIRCULAR_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_IN) == 0)
        return Curve::ELASTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_OUT) == 0)
        return Curve::ELASTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT) == 0)
        return Curve::ELASTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN) == 0)
        return Curve::ELASTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_IN) == 0)
        return Curve::OVERSHOOT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_OUT) == 0)
        return Curve::OVERSHOOT_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT) == 0)
        return Curve::OVERSHOOT_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN) == 0)
        return Curve::OVERSHOOT_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_IN) == 0)
        return Curve::BOUNCE_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_OUT) == 0)
        return Curve::BOUNCE_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT) == 0)
        return Curve::BOUNCE_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN) == 0)
        return Curve::BOUNCE_OUT_IN;
    return Curve::BEZIER;
}

const char* lua_stringFromEnum_CurveInterpolationType(Curve::InterpolationType e)
{
    if (e == Curve::BEZIER)
        return luaEnumString_CurveInterpolationType_BEZIER;
    if (e == Curve::BSPLINE)
        return luaEnumString_CurveInterpolationType_BSPLINE;
    if (e == Curve::FLAT)
        return luaEnumString_CurveInterpolationType_FLAT;
    if (e == Curve::HERMITE)
        return luaEnumString_CurveInterpolationType_HERMITE;
    if (e == Curve::LINEAR)
        return luaEnumString_CurveInterpolationType_LINEAR;
    if (e == Curve::SMOOTH)
        return luaEnumString_CurveInterpolationType_SMOOTH;
    if (e == Curve::STEP)
        return luaEnumString_CurveInterpolationType_STEP;
    if (e == Curve::QUADRATIC_IN)
        return luaEnumString_CurveInterpolationType_QUADRATIC_IN;
    if (e == Curve::QUADRATIC_OUT)
        return luaEnumString_CurveInterpolationType_QUADRATIC_OUT;
    if (e == Curve::QUADRATIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT;
    if (e == Curve::QUADRATIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN;
    if (e == Curve::CUBIC_IN)
        return luaEnumString_CurveInterpolationType_CUBIC_IN;
    if (e == Curve::CUBIC_OUT)
        return luaEnumString_CurveInterpolationType_CUBIC_OUT;
    if (e == Curve::CUBIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_CUBIC_IN_OUT;
    if (e == Curve::CUBIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_CUBIC_OUT_IN;
    if (e == Curve::QUARTIC_IN)
        return luaEnumString_CurveInterpolationType_QUARTIC_IN;
    if (e == Curve::QUARTIC_OUT)
        return luaEnumString_CurveInterpolationType_QUARTIC_OUT;
    if (e == Curve::QUARTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT;
    if (e == Curve::QUARTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN;
    if (e == Curve::QUINTIC_IN)
        return luaEnumString_CurveInterpolationType_QUINTIC_IN;
    if (e == Curve::QUINTIC_OUT)
        return luaEnumString_CurveInterpolationType_QUINTIC_OUT;
    if (e == Curve::QUINTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT;
    if (e == Curve::QUINTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN;
    if (e == Curve::SINE_IN)
        return luaEnumString_CurveInterpolationType_SINE_IN;
    if (e == Curve::SINE_OUT)
        return luaEnumString_CurveInterpolationType_SINE_OUT;
    if (e == Curve::SINE_IN_OUT)
        return luaEnumString_CurveInterpolationType_SINE_IN_OUT;
    if (e == Curve::SINE_OUT_IN)
        return luaEnumString_CurveInterpolationType_SINE_OUT_IN;
    if (e == Curve::EXPONENTIAL_IN)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_IN;
    if (e == Curve::EXPONENTIAL_OUT)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT;
    if (e == Curve::EXPONENTIAL_IN_OUT)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT;
    if (e == Curve::EXPONENTIAL_OUT_IN)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN;
    if (e == Curve::CIRCULAR_IN)
        return luaEnumString_CurveInterpolationType_CIRCULAR_IN;
    if (e == Curve::CIRCULAR_OUT)
        return luaEnumString_CurveInterpolationType_CIRCULAR_OUT;
    if (e == Curve::CIRCULAR_IN_OUT)
        return luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT;
    if (e == Curve::CIRCULAR_OUT_IN)
        return luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN;
    if (e == Curve::ELASTIC_IN)
        return luaEnumString_CurveInterpolationType_ELASTIC_IN;
    if (e == Curve::ELASTIC_OUT)
        return luaEnumString_CurveInterpolationType_ELASTIC_OUT;
    if (e == Curve::ELASTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT;
    if (e == Curve::ELASTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN;
    if (e == Curve::OVERSHOOT_IN)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_IN;
    if (e == Curve::OVERSHOOT_OUT)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_OUT;
    if (e == Curve::OVERSHOOT_IN_OUT)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT;
    if (e == Curve::OVERSHOOT_OUT_IN)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN;
    if (e == Curve::BOUNCE_IN)
        return luaEnumString_CurveInterpolationType_BOUNCE_IN;
    if (e == Curve::BOUNCE_OUT)
        return luaEnumString_CurveInterpolationType_BOUNCE_OUT;
    if (e == Curve::BOUNCE_IN_OUT)
        return luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT;
    if (e == Curve::BOUNCE_OUT_IN)
        return luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN;
    return enumStringEmpty;
}

}

