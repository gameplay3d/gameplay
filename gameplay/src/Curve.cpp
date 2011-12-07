// Purposely not including Base.h here, or any other gameplay dependencies
// so this class can be reused between gameplay and gameplay-encoder.
#include "Curve.h"
#include <cassert>
#include <cmath>
#include <memory>

using std::memcpy;
using std::fabs;
using std::sqrt;
using std::cos;
using std::sin;
using std::exp;

#ifndef NULL
#define NULL 0
#endif

#ifndef MATH_PI
#define MATH_PI 3.14159265358979323846f
#endif

#ifndef MATH_PIOVER2 
#define MATH_PIOVER2 1.57079632679489661923f
#endif

#ifndef MATH_PIX2
#define MATH_PIX2 6.28318530717958647693f
#endif

// Object deletion macro
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) \
    if (x) \
    { \
        delete x; \
        x = NULL; \
    }
#endif

// Array deletion macro
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) \
    if (x) \
    { \
        delete[] x; \
        x = NULL; \
    }
#endif


namespace gameplay
{

Curve::Curve(unsigned int pointCount, unsigned int componentCount)
    : _pointCount(pointCount), _componentCount(componentCount), _componentSize(0), _quaternionOffsets(NULL), _quaternionOffsetsCount(0), _points(NULL)
{
    assert(pointCount > 1 && componentCount > 0);

    _componentSize = sizeof(float)*componentCount;
    _points = new Point[_pointCount];

    for (unsigned int i = 0; i < _pointCount; i++)
    {
        _points[i].time = 0.0f;
        _points[i].value = new float[_componentCount];
        _points[i].inValue = new float[_componentCount];
        _points[i].outValue = new float[_componentCount];
        _points[i].type = LINEAR;
    }
    _points[_pointCount - 1].time = 1.0f;
}

Curve::~Curve()
{
    SAFE_DELETE_ARRAY(_points);
    SAFE_DELETE_ARRAY(_quaternionOffsets);
}

Curve::Point::Point()
    : time(0.0f), value(NULL), inValue(NULL), outValue(NULL)
{
}

Curve::Point::~Point()
{
    SAFE_DELETE_ARRAY(value);
    SAFE_DELETE_ARRAY(inValue);
    SAFE_DELETE_ARRAY(outValue);
}

unsigned int Curve::getPointCount() const
{
    return _pointCount;
}

unsigned int Curve::getComponentCount() const
{
    return _componentCount;
}

float Curve::getStartTime() const
{
    return _points[0].time;
}

float Curve::getEndTime() const
{
    return _points[_pointCount-1].time;
}

void Curve::setPoint(unsigned int index, float time, float* value, InterpolationType type)
{
    setPoint(index, time, value, type, NULL, NULL);
}

void Curve::setPoint(unsigned int index, float time, float* value, InterpolationType type, float* inValue, float* outValue)
{
    assert(index < _pointCount && time >= 0.0f && time <= 1.0f && !(index == 0 && time != 0.0f) && !(index == _pointCount - 1 && time != 1.0f));

    _points[index].time = time;
    _points[index].type = type;

    if (value)
        memcpy(_points[index].value, value, _componentSize);

    if (inValue)
        memcpy(_points[index].inValue, inValue, _componentSize);

    if (outValue)
        memcpy(_points[index].outValue, outValue, _componentSize);
}

void Curve::setTangent(unsigned int index, InterpolationType type, float* inValue, float* outValue)
{
    assert(index < _pointCount);

    _points[index].type = type;

    if (inValue)
        memcpy(_points[index].inValue, inValue, _componentSize);

    if (outValue)
        memcpy(_points[index].outValue, outValue, _componentSize);
}

void Curve::evaluate(float time, float* dst) const
{
    assert(dst && time >= 0 && time <= 1.0f);

    // Check if we are at or beyond the bounds of the curve.
    if (time <= _points[0].time)
    {
        memcpy(dst, _points[0].value, _componentSize);
        return;
    }
    else if (time >= _points[_pointCount - 1].time)
    {
        memcpy(dst, _points[_pointCount - 1].value, _componentSize);
        return;
    }

    // Locate the points we are interpolating between using a binary search.
    unsigned int index = determineIndex(time);
    
    Point* from = _points + index;
    Point* to = _points + (index + 1);

    // Calculate the fractional time between the two points.
    float scale = (to->time - from->time);
    float t = (time - from->time) / scale;

    // Calculate the value of the curve discretely if appropriate.
    switch (from->type)
    {
        case BEZIER:
        {
            interpolateBezier(t, from, to, dst);
            return;
        }
        case BSPLINE:
        {
            Point* c0;
            Point* c1;
            if (index == 0)
            {
                c0 = from;
            }
            else
            {
                c0 = (_points + index - 1);
            }
            
            if (index == _pointCount - 2)
            {
                c1 = to;
            }
            else
            {
                c1 = (_points + index + 2);
            }
            interpolateBSpline(t, c0, from, to, c1, dst);
            return;
        }
        case FLAT:
        {
            interpolateHermiteFlat(t, from, to, dst);
            return;
        }
        case HERMITE:
        {
            interpolateHermite(t, from, to, dst);
            return;
        }
        case LINEAR:
        {
            interpolateLinear(t, from, to, dst);
            break;
        }
        case SMOOTH:
        {
            interpolateHermiteSmooth(t, index, from, to, dst);
            break;
        }
        case STEP:
        {
            memcpy(dst, from->value, _componentSize);
            break;
        }
        case QUADRATIC_IN:
        {
            t *= t;
            break;
        }
        case QUADRATIC_OUT:
        {
            t *= (t - 2.0f);
            break;
        }
        case QUADRATIC_IN_OUT:
        {
            if (t *= 2.0f < 1.0f)
            {
                t *= t * 0.5f;
            }
            else
            {
                t -= 1.0f;
                t = (-(t * (t - 2.0f)) + 1.0f) * 0.5f;
            }
            break;
        }
        case QUADRATIC_OUT_IN:
        {
            if (t < 0.5f)
            {
                t = 2.0f * t * (1.0f - t);
            }
            else
            {
                t = 1.0f + 2.0f * t * (t - 1.0f);
            }
            break;
        }
        case CUBIC_IN:
        {
            t *= t * t;
            break;
        }
        case CUBIC_OUT:
        {
            t--;
            t = t * t * t + 1;
            break;
        }
        case CUBIC_IN_OUT:
        {
            if ((t *= 2.0f) < 1.0f)
            {
                t = t * t * t * 0.5f;
            }
            else
            {
                t -= 2.0f;
                t = (t * t * t + 2.0f) * 0.5f;
            }
            break;
        }
        case CUBIC_OUT_IN:
        {
            t = (2.0f * t - 1.0f);
            t = (t * t * t + 1) * 0.5f;
            break;
        }
        case QUARTIC_IN:
        {
            t *= t * t * t;
            break;
        }
        case QUARTIC_OUT:
        {
            t--;
            t = -(t * t * t * t) + 1.0f;
            break;
        }
        case QUARTIC_IN_OUT:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t * t * t;
            }
            else
            {
                t -= 2.0f;
                t = -0.5f * (t * t * t * t - 2.0f);
            }
            break;
        }
        case QUARTIC_OUT_IN:
        {
            t = 2.0f * t - 1.0f;
            if (t < 0.0f)
            {
                t = 0.5f * (-(t * t) * t * t + 1.0f);
            }
            else
            {
                t = 0.5f * (t * t * t * t + 1.0f);
            }
            break;
        }
        case QUINTIC_IN:
        {
            t *= t * t * t * t;
            break;
        }
        case QUINTIC_OUT:
        {
            t--;
            t = t * t * t * t * t + 1.0f;
            break;
        }
        case QUINTIC_IN_OUT:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t * t * t * t;
            }
            else
            {
                t -= 2.0f;
                t = 0.5f * (t * t * t * t * t + 2.0f);
            }
            break;
        }
        case QUINTIC_OUT_IN:
        {
            t = 2.0f * t - 1.0f;
            t = 0.5f * (t * t * t * t * t + 1.0f);
            break;
        }
        case SINE_IN:
        {
            t = -(cos(t * MATH_PIOVER2) - 1.0f);
            break;
        }
        case SINE_OUT:
        {
            t = sin(t * MATH_PIOVER2);
            break;
        }
        case SINE_IN_OUT:
        {
            t = -0.5f * (cos(MATH_PI * t) - 1.0f);
            break;
        }
        case SINE_OUT_IN:
        {
            if (t < 0.5f)
            {
                t = 0.5f * sin(MATH_PI * t);
            }
            else
            {
                t = -0.5f * cos(MATH_PIOVER2 * (2.0f * t - 1.0f)) + 1.0f;
            }
            break;
        }
        case EXPONENTIAL_IN:
        {
            if (t != 0.0f)
            {
                t = exp(10.0f * (t - 1.0f));
            }
            break;
        }
        case EXPONENTIAL_OUT:
        {
            if (t != 1.0f)
            {
                t = -exp(-10.0f * t) + 1.0f;
            }
            break;
        }
        case EXPONENTIAL_IN_OUT:
        {
            if (t != 0.0f && t != 1.0f)
            {
                if (t < 0.5f)
                {
                    t = 0.5f * exp(10.0f * (2.0f * t - 1.0f));
                }
                else
                {
                    t = -0.5f * exp(10.0f * (2.0f * t + 1.0f)) + 1.0f;
                }
            }
            break;
        }
        case EXPONENTIAL_OUT_IN:
        {
            if (t != 0.0f && t != 1.0f)
            {
                if (t < 0.5f)
                {
                    t = -0.5f * exp(-20.0f * t) + 0.5f;
                }
                else
                {
                    t = 0.5f * exp(20.0f * (t - 1.0f)) + 0.5f;
                }
            }
            break;
        }
        case CIRCULAR_IN:
        {
            t = -(sqrt(1.0f - t * t) - 1.0f);
            break;
        }
        case CIRCULAR_OUT:
        {
            t--;
            t = sqrt(1.0f - t * t);
            break;
        }
        case CIRCULAR_IN_OUT:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * -(sqrt(1.0f - t * t) + 1.0f);
            }
            else
            {
                t -= 2.0f;
                t = 0.5f * (sqrt(1.0f - t * t) + 1.0f);
            }
            break;
        }
        case CIRCULAR_OUT_IN:
        {
            t = 2.0f * t - 1.0f;
            if (t < 0.0f)
            {
                t = 0.5f * sqrt(1.0f - t * t);
            }
            else
            {
                t = 0.5f * (2.0f - sqrt(1.0f - t * t));
            }
            break;
        }
        case ELASTIC_IN:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t--;
                t = -exp(10.0f * t) * sin(t - 0.075f) * (MATH_PIX2 / 0.3f);
            }
            break;
        }
        case ELASTIC_OUT:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t = exp(-10.0f * t) * sin((t - 0.075f) * MATH_PIX2 / 0.3f) + 1.0f;
            }
            break;
        }
        case ELASTIC_IN_OUT:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t = 2.0f * t - 1.0f;
                if (t < 0.0f)
                {
                    t = -0.5f * (exp((10 * time)) * sin(((time - 0.1125f) * MATH_PIX2 / 0.45f)));
                }
                else
                {
                    t = 0.5f * exp((-10 * time)) * sin(((time - 0.1125f) * MATH_PIX2 / 0.45f)) + 1.0f;
                }
            }
            break;
        }
        case ELASTIC_OUT_IN:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t *= 2.0f;
                if (time < 1.0f)
                {
                    t = 0.5f * (exp(-10.0f * t) * sin((t - 0.1125f) * MATH_PIX2 / 0.45f)) + 0.5f;
                }
                else
                {
                    t = 0.5f * exp(10.0f * (t - 2.0f)) * sin((t - 0.1125f) * MATH_PIX2 / 0.45f) + 0.5f;
                }
            }
            break;
        }
        case OVERSHOOT_IN:
        {
            t = t * t * 2.70158f * t - 1.70158f;
            break;
        }
        case OVERSHOOT_OUT:
        {
            t--;
            t = t * t * 2.70158f * t + 1.70158f;
            break;
        }
        case OVERSHOOT_IN_OUT:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t * (3.5949095f * t - 2.5949095f);
            }
            else
            {
                t -= 2.0f;
                t = 0.5f * t * t * (3.5949095f * t + 2.5949095f) + 2.0f;
            }
            break;
        }
        case OVERSHOOT_OUT_IN:
        {
            t = 2.0f * t - 1.0f;
            if (t < 0.0f)
            {
                t = 0.5f * t * t * (3.5949095f * t + 2.5949095f) + 1.0f;
            }
            else
            {
                t = 0.5f * t * t * (3.5949095f * t - 2.5949095f) + 1.0f;
            }
            break;
        }
        case BOUNCE_IN:
        {
            t = 1.0f - t;

            if (t < 0.36363636363636365f)
            {
                t = 7.5625f * t * t;
            }
            else if (t < 0.7272727272727273f)
            {
                t -= 0.5454545454545454f;
                t = 7.5625f * t * t + 0.75f;
            }
            else if (t < 0.9090909090909091f)
            {
                t -= 0.8181818181818182f;
                t = 7.5625f * t * t + 0.9375f;
            }
            else
            {
                t -= 0.9545454545454546f;
                t = 7.5625f * t * t + 0.984375f;
            }

            t = 1.0f - t;
            break;
        }
        case BOUNCE_OUT:
        {
            if (t < 0.36363636363636365f)
            {
                t = 7.5625f * t * t;
            }
            else if (t < 0.7272727272727273f)
            {
                t -= 0.5454545454545454f;
                t = 7.5625f * t * t + 0.75f;
            }
            else if (t < 0.9090909090909091f)
            {
                t -= 0.8181818181818182f;
                t = 7.5625f * t * t + 0.9375f;
            }
            else
            {
                t -= 0.9545454545454546f;
                t = 7.5625f * t * t + 0.984375f;
            }
            break;
        }
        case BOUNCE_IN_OUT:
        {
            if (t < 0.5f)
            {
                t = 1.0f - t * 2.0f;

                if (t < 0.36363636363636365f)
                {
                    t = 7.5625f * t * t;
                }
                else if (t < 0.7272727272727273f)
                {
                    t -= 0.5454545454545454f;
                    t = 7.5625f * t * t + 0.75f;
                }
                else if (t < 0.9090909090909091f)
                {
                    t -= 0.8181818181818182f;
                    t = 7.5625f * t * t + 0.9375f;
                }
                else
                {
                    t -= 0.9545454545454546f;
                    t = 7.5625f * t * t + 0.984375f;
                }

                t = (1.0f - t) * 0.5f;
            }
            else
            {
                t = t * 2.0f - 1.0f;
                if (t < 0.36363636363636365f)
                {
                    t = 7.5625f * t * t;
                }
                else if (t < 0.7272727272727273f)
                {
                    t -= 0.5454545454545454f;
                    t = 7.5625f * t * t + 0.75f;
                }
                else if (t < 0.9090909090909091f)
                {
                    t -= 0.8181818181818182f;
                    t = 7.5625f * t * t + 0.9375f;
                }
                else
                {
                    t -= 0.9545454545454546f;
                    t = 7.5625f * t * t + 0.984375f;
                }

                t = (t + 0.5f) * 0.5f;
            }
            break;
        }
        case BOUNCE_OUT_IN:
        {
            if (t < 0.1818181818f)
            {
                t = 15.125f * t * t;
            }
            else if (t < 0.3636363636f)
            {
                t = 1.5f + (-8.250000001f + 15.125f * t) * t;
            }
            else if (t < 0.4545454546f)
            {
                t = 3.0f + (-12.375f + 15.125f * t) * t;
            }
            else if (t < 0.5f)
            {
                t = 3.9375f + (-14.4375f + 15.125f * t) * t;
            }
            else if (t <= 0.5454545455f)
            {
                t = -3.625000004f + (15.81250001f - 15.125f * t) * t;
            }
            else if (t <= 0.6363636365f)
            {
                t = -4.75f + (17.875f - 15.125f * t) * t;
            }
            else if (t <= 0.8181818180f)
            {
                t = -7.374999995f + (21.99999999f - 15.125f * t) * t;
            }
            else
            {
                t = -14.125f + (30.25f - 15.125f * t) * t;
            }
            break;
        }
    }

    float* fromValue = from->value;
    float* toValue = to->value;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = lerp(t, fromValue[i], toValue[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        
        do {
            while (i < quaternionOffset)
            {
                dst[i] = lerp(t, fromValue[i], toValue[i]);
                i++;
            }
            // Handle quaternion component.
            float interpTime = lerp(t, fromValue[i], toValue[i]);
            interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        while (i < _componentCount)
        {
            dst[i] = lerp(t, fromValue[i], toValue[i]);
            i++;
        }
    }
}

void Curve::addQuaternionOffset(unsigned int offset)
{
    if (!_quaternionOffsets)
    {
        _quaternionOffsetsCount = 1;
        _quaternionOffsets = new unsigned int[_quaternionOffsetsCount];
        _quaternionOffsets[0] = offset;
    }
    else
    {
        assert((offset >= _componentCount - 4) && (offset > (_quaternionOffsets[_quaternionOffsetsCount - 1] + 3)));

        unsigned int oldSize = _quaternionOffsetsCount;
        _quaternionOffsetsCount++;
        
        unsigned int* newArray = new unsigned int[_quaternionOffsetsCount];
        memcpy(newArray, _quaternionOffsets, sizeof(unsigned int) * oldSize);
        
        // set new offset.
        newArray[oldSize] = offset;

        SAFE_DELETE_ARRAY(_quaternionOffsets);    // delete old array
        _quaternionOffsets = newArray;  // point to new array.
    }
}

void Curve::interpolateBezier(float s, Point* from, Point* to, float* dst) const
{
    float s_2 = s * s;
    float eq0 = 1 - s;
    float eq0_2 = eq0 * eq0;
    float eq1 = eq0_2 * eq0;
    float eq2 = 3 * s * eq0_2;
    float eq3 = 3 * s_2 * eq0;
    float eq4 = s_2 * s;

    float* fromValue = from->value;
    float* toValue = to->value;
    float* outValue = from->outValue;
    float* inValue = to->inValue;


    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        
        do {
            while (i < quaternionOffset)
            {
                dst[i] = bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
                i++;
            }
            // Handle quaternion component.
            float interpTime = bezier(eq1, eq2, eq3, eq4, from->time, outValue[i], to->time, inValue[i]);
            interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        while (i < _componentCount)
        {
            dst[i] = bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
            i++;
        }
    }
}

void Curve::interpolateBSpline(float s, Point* c0, Point* c1, Point* c2, Point* c3, float* dst) const
{   
    float s_2 = s * s;
    float s_3 = s_2 * s;
    float eq0 = (-s_3 + 3 * s_2 - 3 * s + 1) / 6.0f;
    float eq1 = (3 * s_3 - 6 * s_2 + 4) / 6.0f;
    float eq2 = (-3 * s_3 + 3 * s_2 + 3 * s + 1) / 6.0f;
    float eq3 = s_3 / 6.0f;

    float* c0Value = c0->value;
    float* c1Value = c1->value;
    float* c2Value = c2->value;
    float* c3Value = c3->value;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        do {
            while (i < quaternionOffset)
            {
                dst[i] = bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
                i++;
            }
            // Handle quaternion component.
            float interpTime;
            if (c0->time == c1->time)
                interpTime = bspline(eq0, eq1, eq2, eq3, -c0->time, c1->time, c2->time, c3->time);
            else if (c2->time == c3->time)
                interpTime = bspline(eq0, eq1, eq2, eq3, c0->time, c1->time, c2->time, -c3->time); 
            else
                interpTime = bspline(eq0, eq1, eq2, eq3, c0->time, c1->time, c2->time, c3->time);
            
            interpolateQuaternion(s, (c1Value + i) , (c2Value + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);
        
        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
            i++;
        }
    }
}

void Curve::interpolateHermite(float s, Point* from, Point* to, float* dst) const
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1
    float h10 = s_3 - 2 * s_2 + s;       // basis function 2
    float h11 = s_3 - s_2;               // basis function 3

    float* fromValue = from->value;
    float* toValue = to->value;
    float* outValue = from->outValue;
    float* inValue = to->inValue;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;

        do {
            while (i < quaternionOffset)
            {
                dst[i] = hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
                i++;
            }
            // Handle quaternion component.
            float interpTime = hermite(h00, h01, h10, h11, from->time, outValue[i], to->time, inValue[i]);
            interpolateQuaternion(interpTime, (from->value + i), (to->value + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
            
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);
       
        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
            i++;
        }
    }
}

void Curve::interpolateHermiteFlat(float s, Point* from, Point* to, float* dst) const
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1

    float* fromValue = from->value;
    float* toValue = to->value;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = hermiteFlat(h00, h01, fromValue[i], toValue[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        float interpTime = hermiteFlat(h00, h01, from->time, to->time);
        do {
            while (i < quaternionOffset)
            {
                dst[i] = hermiteFlat(h00, h01, fromValue[i], toValue[i]);
                i++;
            }
            // We've hit a quaternion component, so handle it. increase the component counter by 4, and increase quaternionOffsetIndex
            interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = hermiteFlat(h00, h01, fromValue[i], toValue[i]);
            i++;
        }
    }
}

void Curve::interpolateHermiteSmooth(float s, unsigned int index, Point* from, Point* to, float* dst) const
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1
    float h10 = s_3 - 2 * s_2 + s;       // basis function 2
    float h11 = s_3 - s_2;               // basis function 3

    float inValue;
    float outValue;

    float* fromValue = from->value;
    float* toValue = to->value;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            if (index == 0)
            {
                outValue = toValue[i] - fromValue[i];
            }
            else
            {
                outValue = (toValue[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                inValue = toValue[i] - fromValue[i];
            }
            else
            {
                inValue = ((to + 1)->value[i] - fromValue[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            dst[i] = hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;

        do {
            // Handle scalar values up to the quaternion offset.
            while (i < quaternionOffset)
            {
                // Interpolate as scalar.
                if (index == 0)
                {
                    outValue = toValue[i] - fromValue[i];
                }
                else
                {
                    outValue = (toValue[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
                }

                if (index == _pointCount - 2)
                {
                    inValue = toValue[i] - fromValue[i];
                }
                else
                {
                    inValue = ((to + 1)->value[i] - fromValue[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
                }

                dst[i] = hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
                i++;
            }
            
            if (index == 0)
            {
                outValue = to->time - from->time;
            }
            else
            {
                outValue = (to->time - (from - 1)->time) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                inValue = to->time - from->time;
            }
            else
            {
                inValue = ((to + 1)->time - from->time) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            float interpTime = hermiteSmooth(h00, h01, h10, h11, from->time, outValue, to->time, inValue);
            interpolateQuaternion(interpTime, (from->value + i), (to->value + i), (dst + i));
            i+=4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);
        

        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            // Interpolate as scalar.
            if (index == 0)
            {
                outValue = toValue[i] - fromValue[i];
            }
            else
            {
                outValue = (toValue[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                inValue = toValue[i] - fromValue[i];
            }
            else
            {
                inValue = ((to + 1)->value[i] - fromValue[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            dst[i] = hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
            i++;
        }
    }
}

void Curve::interpolateLinear(float s, Point* from, Point* to, float* dst) const
{
    float* fromValue = from->value;
    float* toValue = to->value;

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = lerp(s, fromValue[i], toValue[i]);
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        do {
            // Loop through values until you hit the next quaternion offset.
            while (i < quaternionOffset)
            {
                dst[i] = lerp(s, fromValue[i], toValue[i]);
                i++;
            }
            // Handle quaternion component.
            interpolateQuaternion(s, (from->value + quaternionOffset), (to->value + quaternionOffset), (dst + quaternionOffset));        
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        // Loop through the last remaining values, if any.
        while (i < _componentCount)
        {
            dst[i] = lerp(s, fromValue[i], toValue[i]);
            i++;
        }
    }
}

void slerpQuat(float* q1, float* q2, float t, float* dst)
{
    // Fast slerp implementation by kwhatmough:
    // It contains no division operations, no trig, no inverse trig
    // and no sqrt. Not only does this code tolerate small constraint
    // errors in the input quaternions, it actually corrects for them.
    assert(dst);
    assert(!(t < 0.0f || t > 1.0f));

    if (t == 0.0f)
    {
        memcpy(dst, q1, sizeof(float) * 4);
        return;
    }
    else if (t == 1.0f)
    {
        memcpy(dst, q2, sizeof(float) * 4);
        return;
    }

    float halfY, alpha, beta;
    float u, f1, f2a, f2b;
    float ratio1, ratio2;
    float halfSecHalfTheta, versHalfTheta;
    float sqNotU, sqU;

    float cosTheta = q1[3] * q2[3] + q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2];

    // As usual in all slerp implementations, we fold theta.
    alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    halfY = 1.0f + alpha * cosTheta;

    // Here we bisect the interval, so we need to fold t as well.
    f2b = t - 0.5f;
    u = f2b >= 0 ? f2b : -f2b;
    f2a = u - f2b;
    f2b += u;
    u += u;
    f1 = 1.0f - u;

    // One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
    halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    // Evaluate series expansions of the coefficients.
    sqNotU = f1 * f1;
    ratio2 = 0.0000440917108f * versHalfTheta;
    ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

    // Perform the bisection and resolve the folding done earlier.
    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    beta = f1 + f2b;

    // Apply final coefficients to a and b as usual.
    float w = alpha * q1[3] + beta * q2[3];
    float x = alpha * q1[0] + beta * q2[0];
    float y = alpha * q1[1] + beta * q2[1];
    float z = alpha * q1[2] + beta * q2[2];

    // This final adjustment to the quaternion's length corrects for
    // any small constraint error in the inputs q1 and q2. But as you
    // can see, it comes at the cost of 9 additional multiplication
    // operations. If this error-correcting feature is not required,
    // the following code may be removed.
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    dst[3] = w * f1;
    dst[0] = x * f1;
    dst[1] = y * f1;
    dst[2] = z * f1;
}

void normalizeQuat(float* q)
{
    float n = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

    // Do we need to normalize?
    if (fabs(n) > 0.00001f && fabs(n - 1.0f) > 0.00001f)
    {
        n = sqrt(n);
        q[0] /= n;
        q[1] /= n;
        q[2] /= n;
        q[3] /= n;
    }
}

void Curve::interpolateQuaternion(float s, float* from, float* to, float* dst) const
{
    float quatFrom[4] = { from[0], from[1], from[2], from[3] };
    float quatTo[4] = { to[0], to[1], to[2], to[3] };

    // Normalize the quaternions.
    normalizeQuat(quatFrom);
    normalizeQuat(quatTo);
        
    // Evaluate.
    if (s >= 0)
        slerpQuat(quatFrom, quatTo, s, dst);
    else
        slerpQuat(quatTo, quatFrom, -s, dst);
}

int Curve::determineIndex(float time) const
{
    unsigned int min = 0;
    unsigned int max = _pointCount - 1;
    unsigned int mid = 0;

    // Do a binary search to determine the index.
    do 
    {
        mid = (min + max) >> 1;

        if (time >= _points[mid].time && time <= _points[mid + 1].time)
            return mid;
        else if (time < _points[mid].time)
            max = mid - 1;
        else
            min = mid + 1;
    } while (min <= max);
    
    // We should never hit this!
    return -1;
}

}
