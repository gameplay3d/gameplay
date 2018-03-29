#include "Base.h"
#include "Curve.h"
#include "Quaternion.h"
#include "MathUtil.h"

namespace gameplay
{

Curve* Curve::create(size_t pointCount, size_t componentCount)
{
    return new Curve(pointCount, componentCount);
}

Curve::Curve(size_t pointCount, size_t componentCount) : 
    _pointCount(pointCount),
    _componentCount(componentCount),
    _componentSize(sizeof(float)*componentCount),
    _quaternionOffset(nullptr),
    _points(nullptr)
{
    _points = new Point[_pointCount];
    for (size_t i = 0; i < _pointCount; i++)
    {
        _points[i].time = 0.0f;
        _points[i].value = new float[_componentCount];
        _points[i].inValue = new float[_componentCount];
        _points[i].outValue = new float[_componentCount];
        _points[i].interpolation = Interpolation::eLinear;
    }
    _points[_pointCount - 1].time = 1.0f;
}

Curve::~Curve()
{
    GP_SAFE_DELETE_ARRAY(_points);
    GP_SAFE_DELETE_ARRAY(_quaternionOffset);
}

Curve::Point::Point() : 
    time{0.0f},
    value{nullptr},
    inValue{nullptr},
    outValue{nullptr},
    interpolation{Interpolation::eLinear}
{
}

Curve::Point::~Point()
{
    GP_SAFE_DELETE_ARRAY(value);
    GP_SAFE_DELETE_ARRAY(inValue);
    GP_SAFE_DELETE_ARRAY(outValue);
}

size_t Curve::getPointCount() const
{
    return _pointCount;
}

size_t Curve::getComponentCount() const
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

float Curve::getPointTime(size_t index) const
{
    assert(index < _pointCount);
    return _points[index].time;
}


Curve::Interpolation Curve::getPointInterpolation(size_t index) const
{
    assert(index < _pointCount);
    return _points[index].interpolation;
}

void Curve::getPointValues(size_t index, float* value, float* inValue, float* outValue) const
{
    assert(index < _pointCount);
    
    if (value)
    {
        std::memcpy(value, _points[index].value, _componentSize);
    }
    if (inValue)
    {
        std::memcpy(inValue, _points[index].inValue, _componentSize);
    }
    if (outValue)
    {
        std::memcpy(outValue, _points[index].outValue, _componentSize);
    }
}

void Curve::setPoint(size_t index, float time, float* value, Interpolation interpolation)
{
    setPoint(index, time, value, interpolation, nullptr, nullptr);
}

void Curve::setPoint(size_t index, float time, float* value, Interpolation interpolation, float* inValue, float* outValue)
{
    assert(index < _pointCount && time >= 0.0f && time <= 1.0f && !(_pointCount > 1 && index == 0 && time != 0.0f) && !(_pointCount != 1 && index == _pointCount - 1 && time != 1.0f));

    _points[index].time = time;
    _points[index].interpolation = interpolation;

    if (value)
    {
        std::memcpy(_points[index].value, value, _componentSize);
    }
    if (inValue)
    {
        std::memcpy(_points[index].inValue, inValue, _componentSize);
    }
    if (outValue)
    {
        std::memcpy(_points[index].outValue, outValue, _componentSize);
    }
}

void Curve::setTangent(size_t index, Interpolation interpolation, float* inValue, float* outValue)
{
    assert(index < _pointCount);

    _points[index].interpolation = interpolation;

    if (inValue)
    {
        std::memcpy(_points[index].inValue, inValue, _componentSize);
    }
    if (outValue)
    {
        std::memcpy(_points[index].outValue, outValue, _componentSize);
    }
}

void Curve::evaluate(float time, float* dst) const
{
    assert(dst);

    evaluate(time, 0.0f, 1.0f, 0.0f, dst);
}

void Curve::evaluate(float time, float startTime, float endTime, float loopBlendTime, float* dst) const
{
    assert(dst && startTime >= 0.0f && startTime <= endTime && endTime <= 1.0f && loopBlendTime >= 0.0f);

    // If there's only one point on the curve, return its value.
    if (_pointCount == 1)
    {
        std::memcpy(dst, _points[0].value, _componentSize);
        return;
    }
    uint32_t min = 0;
    uint32_t max = (uint32_t)_pointCount - 1;
    float localTime = time;
    if (startTime > 0.0f || endTime < 1.0f)
    {
        // Evaluating a sub section of the curve
        min = determineIndex(startTime, 0, max);
        max = determineIndex(endTime, min, max);

        // Convert time to fall within the subregion
        localTime = _points[min].time + (_points[max].time - _points[min].time) * time;
    }
    if (loopBlendTime == 0.0f)
    {
        // If no loop blend time is specified, clamp time to end points
        if (localTime < _points[min].time)
        {
            localTime = _points[min].time;
        }
        else if (localTime > _points[max].time)
        {
            localTime = _points[max].time;
        }
    }
    // If an exact endpoint was specified, skip interpolation and return the value directly
    if (localTime == _points[min].time)
    {
        std::memcpy(dst, _points[min].value, _componentSize);
        return;
    }
    if (localTime == _points[max].time)
    {
        std::memcpy(dst, _points[max].value, _componentSize);
        return;
    }

    Point* from;
    Point* to;
    float scale;
    float t;
    size_t index;

    if (localTime > _points[max].time)
    {
        // Looping forward
        index = max;
        from = &_points[max];
        to = &_points[min];

        // Calculate the fractional time between the two points.
        t = (localTime - from->time) / loopBlendTime;
    }
    else if (localTime < _points[min].time)
    {
        // Looping in reverse
        index = min;
        from = &_points[min];
        to = &_points[max];

        // Calculate the fractional time between the two points.
        t = (from->time - localTime) / loopBlendTime;
    }
    else
    {
        // Locate the points we are interpolating between using a binary search.
        index = determineIndex(localTime, min, max);
        from = &_points[index];
        to = &_points[index == max ? index : index+1];

        // Calculate the fractional time between the two points.
        scale = (to->time - from->time);
        t = (localTime - from->time) / scale;
    }
    // Calculate the value of the curve discretely if appropriate.
    switch (from->interpolation)
    {
    case Interpolation::eBezier:
        {
            interpolateBezier(t, from, to, dst);
            return;
        }
    case Interpolation::eBspline:
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
    case Interpolation::eFlat:
        {
            interpolateHermiteFlat(t, from, to, dst);
            return;
        }
    case Interpolation::eHermite:
        {
            interpolateHermite(t, from, to, dst);
            return;
        }
    case Interpolation::eLinear:
        {
            // Can just break here because linear formula follows switch
            break;
        }
    case Interpolation::eSmooth:
        {
            interpolateHermiteSmooth(t, index, from, to, dst);
            return;
        }
    case Interpolation::eStep:
        {
            std::memcpy(dst, from->value, _componentSize);
            return;
        }
    case Interpolation::eQuadraticIn:
        {
            t *= t;
            break;
        }
    case Interpolation::eQuadraticOut:
        {
            t *= -(t - 2.0f);
            break;
        }
    case Interpolation::eQuadraticInOut:
        {
            float tx2 = t * 2.0f;

            if (tx2 < 1.0f)
            {
                t = 0.5f * (tx2 * tx2);
            }
            else
            {
                float temp = tx2 - 1.0f;
                t = 0.5f * (-( temp * (temp - 2.0f)) + 1.0f);
            }
            break;
        }
    case Interpolation::eQuadraticOutIn:
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
    case Interpolation::eCubicIn:
        {
            t *= t * t;
            break;
        }
    case Interpolation::eCubicOut:
        {
            t--;
            t = t * t * t + 1;
            break;
        }
    case Interpolation::eCubicInOut:
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
    case Interpolation::eCubicOutIn:
        {
            t = (2.0f * t - 1.0f);
            t = (t * t * t + 1) * 0.5f;
            break;
        }
    case Interpolation::eQuarticIn:
        {
            t *= t * t * t;
            break;
        }
    case Interpolation::eQuarticOut:
        {
            t--;
            t = -(t * t * t * t) + 1.0f;
            break;
        }
    case Interpolation::eQuarticInOut:
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
    case Interpolation::eQuarticOutIn:
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
    case Interpolation::eQuinticIn:
        {
            t *= t * t * t * t;
            break;
        }
    case Interpolation::eQuinticOut:
        {
            t--;
            t = t * t * t * t * t + 1.0f;
            break;
        }
    case Interpolation::eQuinticInOut:
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
    case Interpolation::eQuinticOutIn:
        {
            t = 2.0f * t - 1.0f;
            t = 0.5f * (t * t * t * t * t + 1.0f);
            break;
        }
    case Interpolation::eSineIn:
        {
            t = -(std::cos(t * GP_MATH_PIOVER2) - 1.0f);
            break;
        }
    case Interpolation::eSineOut:
        {
            t = std::sin(t * GP_MATH_PIOVER2);
            break;
        }
    case Interpolation::eSineInOut:
        {
            t = -0.5f * (std::cos(GP_MATH_PI * t) - 1.0f);
            break;
        }
    case Interpolation::eSineOutIn:
        {
            if (t < 0.5f)
            {
                t = 0.5f * std::sin(GP_MATH_PI * t);
            }
            else
            {
                t = -0.5f * std::cos(GP_MATH_PIOVER2 * (2.0f * t - 1.0f)) + 1.0f;
            }
            break;
        }
    case Interpolation::eExponentialIn:
        {
            if (t != 0.0f)
            {
                t = std::exp(10.0f * (t - 1.0f));
            }
            break;
        }
    case Interpolation::eExponentialOut:
        {
            if (t != 1.0f)
            {
                t = -std::exp(-10.0f * t) + 1.0f;
            }
            break;
        }
    case Interpolation::eExponentialInOut:
        {
            if (t != 0.0f && t != 1.0f)
            {
                if (t < 0.5f)
                {
                    t = 0.5f * std::exp(10.0f * (2.0f * t - 1.0f));
                }
                else
                {
                    t = -0.5f * std::exp(10.0f * (-2.0f * t + 1.0f)) + 1.0f;
                }
            }
            break;
        }
    case  Interpolation::eExponentialOutIn:
        {
            if (t != 0.0f && t != 1.0f)
            {
                if (t < 0.5f)
                {
                    t = -0.5f * std::exp(-20.0f * t) + 0.5f;
                }
                else
                {
                    t = 0.5f * std::exp(20.0f * (t - 1.0f)) + 0.5f;
                }
            }
            break;
        }
    case Interpolation::eCircularIn:
        {
            t = -(std::sqrt(1.0f - t * t) - 1.0f);
            break;
        }
    case Interpolation::eCircularOut:
        {
            t--;
            t = std::sqrt(1.0f - t * t);
            break;
        }
    case Interpolation::eCircularInOut:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * (-std::sqrt((1.0f - t * t)) + 1.0f);
            }
            else
            {
                t -= 2.0f;
                t = 0.5f * (std::sqrt((1.0f - t * t)) + 1.0f);
            }
            break;
        }
    case Interpolation::eCircularOutIn:
        {
            t = 2.0f * t - 1.0f;
            if (t < 0.0f)
            {
                t = 0.5f * std::sqrt(1.0f - t * t);
            }
            else
            {
                t = 0.5f * (2.0f - std::sqrt(1.0f - t * t));
            }
            break;
        }
    case Interpolation::eElasticIn:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t = t - 1.0f;
                t = -1.0f * (std::exp(10.0f * t) * std::sin( (t - 0.075f) * GP_MATH_PIX2 / 0.3f ) );
            }
            break;
        }
    case Interpolation::eElasticOut:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t = std::exp(-10.0f * t) * std::sin((t - 0.075f) * GP_MATH_PIX2 / 0.3f) + 1.0f;
            }
            break;
        }
    case Interpolation::eElasticInOut:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t = 2.0f * t - 1.0f;
                if (t < 0.0f)
                {
                    t = -0.5f * (std::exp((10 * t)) * std::sin(((t - 0.1125f) * GP_MATH_PIX2 / 0.45f)));
                }
                else
                {
                    t = 0.5f * std::exp((-10 * t)) * std::sin(((t - 0.1125f) * GP_MATH_PIX2 / 0.45f)) + 1.0f;
                }
            }
            break;
        }
    case Interpolation::eElasticOutIn:
        {
            if (t != 0.0f && t != 1.0f)
            {
                t *= 2.0f;
                if (t < 1.0f)
                {
                    t = 0.5f * (std::exp((-10 * t)) * std::sin(((t - 0.1125f) * (GP_MATH_PIX2) / 0.45f))) + 0.5f;
                }
                else
                {
                    t = 0.5f * (std::exp((10 *(t - 2))) * std::sin(((t - 0.1125f) * (GP_MATH_PIX2) / 0.45f))) + 0.5f;
                }
            }
            break;
        }
    case Interpolation::eOvershootIn:
        {
            t = t * t * (2.70158f * t - 1.70158f);
            break;
        }
    case Interpolation::eOvershootOut:
        {
            t--;
            t = t * t * (2.70158f * t + 1.70158f) + 1;
            break;
        }
    case Interpolation::eOvershootInOut:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t * (3.5949095f * t - 2.5949095f);
            }
            else
            {
                t -= 2.0f;
                t = 0.5f * (t * t * (3.5949095f * t + 2.5949095f) + 2.0f);
            }
            break;
        }
    case Interpolation::eOvershootOutIn:
        {
            t = 2.0f * t - 1.0f;
            if (t < 0.0f)
            {
                t = 0.5f * (t * t * (3.5949095f * t + 2.5949095f) + 1.0f);
            }
            else
            {
                t = 0.5f * (t * t * (3.5949095f * t - 2.5949095f) + 1.0f);
            }
            break;
        }
    case Interpolation::eBounceIn:
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
    case Interpolation::eBounceOut:
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
    case Interpolation::eBounceInOut:
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
                t = 0.5f * t + 0.5f;
            }
            break;
        }
    case Interpolation::eBounceOutIn:
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
    interpolateLinear(t, from, to, dst);
}

void Curve::setQuaternionOffset(size_t offset)
{
    assert(offset <= (_componentCount - 4));

    if (!_quaternionOffset)
    {
        _quaternionOffset = new size_t[1];
    }
    *_quaternionOffset = offset;
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

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
        }
        // Handle quaternion component.
        float interpTime = MathUtil::bezier(eq1, eq2, eq3, eq4, from->time, outValue[i], to->time, inValue[i]);
        interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
        // Handle remaining components (if any) as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::bezier(eq1, eq2, eq3, eq4, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
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

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (c1Value[i] == c2Value[i])
            {
                dst[i] = c1Value[i];
            }
            else
            {
                dst[i] = MathUtil::bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {
            if (c1Value[i] == c2Value[i])
            {
                dst[i] = c1Value[i];
            }
            else
            {
                dst[i] = MathUtil::bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
            }
        }
        // Handle quaternion component.
        float interpTime;
        if (c0->time == c1->time)
        {
            interpTime = MathUtil::bspline(eq0, eq1, eq2, eq3, -c0->time, c1->time, c2->time, c3->time);
        }
        else if (c2->time == c3->time)
        {
            interpTime = MathUtil::bspline(eq0, eq1, eq2, eq3, c0->time, c1->time, c2->time, -c3->time);
        }
        else
        {
            interpTime = MathUtil::bspline(eq0, eq1, eq2, eq3, c0->time, c1->time, c2->time, c3->time);
        }
        interpolateQuaternion(s, (c1Value + i) , (c2Value + i), (dst + i));
        // Handle remaining components (if any) as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (c1Value[i] == c2Value[i])
            {
                dst[i] = c1Value[i];
            }
            else
            {
                dst[i] = MathUtil::bspline(eq0, eq1, eq2, eq3, c0Value[i], c1Value[i], c2Value[i], c3Value[i]);
            }
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

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
        }
        // Handle quaternion component.
        float interpTime = MathUtil::hermite(h00, h01, h10, h11, from->time, outValue[i], to->time, inValue[i]);
        interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
        // Handle remaining components (if any) as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermite(h00, h01, h10, h11, fromValue[i], outValue[i], toValue[i], inValue[i]);
            }
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

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermiteFlat(h00, h01, fromValue[i], toValue[i]);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermiteFlat(h00, h01, fromValue[i], toValue[i]);
            }
        }
        // Handle quaternion component.
        float interpTime = MathUtil::hermiteFlat(h00, h01, from->time, to->time);
        interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
        // Handle remaining components (if any) as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::hermiteFlat(h00, h01, fromValue[i], toValue[i]);
            }
        }
    }
}

void Curve::interpolateHermiteSmooth(float s, size_t index, Point* from, Point* to, float* dst) const
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

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
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

                dst[i] = MathUtil::hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {   
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
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
                dst[i] = MathUtil::hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
            }
        }
        // Handle quaternion component.
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
        float interpTime = MathUtil::hermiteSmooth(h00, h01, h10, h11, from->time, outValue, to->time, inValue);
        interpolateQuaternion(interpTime, (fromValue + i), (toValue + i), (dst + i));
        // Handle remaining components (if any) as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
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

                dst[i] = MathUtil::hermiteSmooth(h00, h01, h10, h11, fromValue[i], outValue, toValue[i], inValue);
            }
        }
    }
}

void Curve::interpolateLinear(float s, Point* from, Point* to, float* dst) const
{
    float* fromValue = from->value;
    float* toValue = to->value;

    if (!_quaternionOffset)
    {
        for (size_t i = 0; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
            {
                dst[i] = fromValue[i];
            }
            else
            {
                dst[i] = MathUtil::lerp(s, fromValue[i], toValue[i]);
            }
        }
    }
    else
    {
        // Interpolate any values up to the quaternion offset as scalars.
        size_t quaternionOffset = *_quaternionOffset;
        size_t i = 0;
        for (i = 0; i < quaternionOffset; i++)
        {
            if (fromValue[i] == toValue[i])
                dst[i] = fromValue[i];
            else
                dst[i] = MathUtil::lerp(s, fromValue[i], toValue[i]);
        }
        // Handle quaternion component.
        interpolateQuaternion(s, (fromValue + i), (toValue + i), (dst + i));
        // handle any remaining components as scalars
        for (i += 4; i < _componentCount; i++)
        {
            if (fromValue[i] == toValue[i])
                dst[i] = fromValue[i];
            else
                dst[i] = MathUtil::lerp(s, fromValue[i], toValue[i]);
        }
    }
}

void Curve::interpolateQuaternion(float s, float* from, float* to, float* dst) const
{
    // Evaluate.
    if (s >= 0)
        Quaternion::slerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], s, dst, dst + 1, dst + 2, dst + 3);
    else
        Quaternion::slerp(to[0], to[1], to[2], to[3], from[0], from[1], from[2], from[3], s, dst, dst + 1, dst + 2, dst + 3);
}

int Curve::determineIndex(float time, uint32_t min, uint32_t max) const
{
    uint32_t mid;
    // Do a binary search to determine the index.
    do 
    {
        mid = (min + max) >> 1;

        if (time >= _points[mid].time && time < _points[mid + 1].time)
            return mid;
        else if (time < _points[mid].time)
            max = mid - 1;
        else
            min = mid + 1;
    } while (min <= max);
    
    return max;
}

}
