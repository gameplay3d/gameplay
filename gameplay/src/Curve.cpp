/*
 * Curve.cpp
 */

#include "Base.h"
#include "Curve.h"
#include "Transform.h"

#define ANIMATION_ROTATE_OFFSET 0
#define ANIMATION_SRT_OFFSET 3

namespace gameplay
{

Curve::Curve(unsigned int pointCount, unsigned int componentCount)
    : _pointCount(pointCount), _componentCount(componentCount), _rotationOffset(-1)
{
    assert(pointCount > 1 && componentCount > 0);

    _componentSize = sizeof(float)*componentCount;
    _points = new Point[_pointCount];

    for (unsigned int i = 0; i < _pointCount; i++)
    {
        _points[i].time = 0.0f;
        _points[i].value = new float[_componentCount];
        _points[i].tangentIn = new float[_componentCount];
        _points[i].tangentOut = new float[_componentCount];
        _points[i].type = LINEAR;
    }
    _points[_pointCount - 1].time = 1.0f;
}

Curve::~Curve()
{
    SAFE_DELETE_ARRAY(_points);
}

Curve::Point::Point()
    : time(0.0f), value(NULL), tangentIn(NULL), tangentOut(NULL)
{
}

Curve::Point::~Point()
{
    SAFE_DELETE_ARRAY(value);
    SAFE_DELETE_ARRAY(tangentIn);
    SAFE_DELETE_ARRAY(tangentOut);
}

unsigned int Curve::getPointCount() const
{
    return _pointCount;
}

unsigned int Curve::getComponentCount() const
{
    return _componentCount;
}

void Curve::setPoint(unsigned int index, float time, float* value, InterpolationType type)
{
    setPoint(index, time, value, type, NULL, NULL);
}

void Curve::setPoint(unsigned int index, float time, float* value, InterpolationType type, float* tangentIn, float* tangentOut)
{
    assert(index < _pointCount && time >= 0.0f && time <= 1.0f && !(index == 0 && time != 0.0f) && !(index == _pointCount - 1 && time != 1.0f));

    _points[index].time = time;
    _points[index].type = type;

    if (value)
        memcpy(_points[index].value, value, _componentSize);

    if (tangentIn)
        memcpy(_points[index].tangentIn, tangentIn, _componentSize);

    if (tangentOut)
        memcpy(_points[index].tangentOut, tangentOut, _componentSize);
}

void Curve::setTangent(unsigned int index, InterpolationType type, float* tangentIn, float* tangentOut)
{
    assert(index < _pointCount);

    _points[index].type = type;

    if (tangentIn)
        memcpy(_points[index].tangentIn, tangentIn, _componentSize);

    if (tangentOut)
        memcpy(_points[index].tangentOut, tangentOut, _componentSize);
}

void Curve::evaluate(float time, float* dst)
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
            break;
        }
        case BSPLINE:
        {
            Point* c0;
            Point* c1;
            if (index == 0)
            {
                c0 = to;
            }
            else
            {
                c0 = _points + index - 1;
            }
            
            if (index == _pointCount - 2)
            {
                c1 = from;
            }
            else
            {
                c1 = _points + index + 2;
            }
            interpolateBSpline(t, c0, from, to, c1, dst);
            break;
        }
        case FLAT:
        {
            interpolateHermiteFlat(t, from, to, dst);
            break;
        }
        case HERMITE:
        {
            interpolateHermite(t, from, to, dst);
            break;
        }
        case LINEAR:
        {
            for (unsigned int i = 0; i < _componentCount; i++)
            {
                if ((int) i == _rotationOffset)
                {
                    interpolateQuaternion(t, from, to, dst + i);
                    // Increment counter by 3 because we've handled all the rotation components at once.
                    i += 3;
                }
                else
                {
                    dst[i] = from->value[i] + (to->value[i] - from->value[i]) * t; 
                }
            }
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
    }
}

void Curve::interpolateBezier(float s, Point* from, Point* to, float* dst)
{
    float s_2 = s * s;
    float eq0 = 1 - s;
    float eq0_2 = eq0 * eq0;
    float eq1 = eq0_2 * eq0;
    float eq2 = 3 * s * eq0_2;
    float eq3 = 3 * s_2 * eq0;
    float eq4 = s_2 * s;

    // Evaluate the curve.
    for (unsigned int i = 0; i < _componentCount; i++)
    {
        if ((int) i == _rotationOffset)
        {
            float interpTime = from->tangentOut[i] * eq2 + to->tangentIn[i] * eq3 + 1.0f * eq4;
            interpolateQuaternion(interpTime, from, to, dst + i);
            // Increment counter by 3 because we've handled all the rotation components at once.
            i += 3;
        }
        else
        {
            dst[i] = from->value[i] * eq1 + from->tangentOut[i] * eq2 + to->tangentIn[i] * eq3 + to->value[i] * eq4;
        }
    }
}

void Curve::interpolateBSpline(float s, Point* c0, Point* c1, Point* c2, Point* c3, float* dst)
{   
    float s_2 = s * s;
    float s_3 = s_2 * s;
    float eq0 = (-s_3 + 3 * s_2 - 3 * s + 1) / 6.0f;
    float eq1 = (3 * s_3 - 6 * s_2 + 4) / 6.0f;
    float eq2 = (-3 * s_3 + 3 * s_2 + 3 * s + 1) / 6.0f;
    float eq3 = s_3 / 6.0f;

    // Handles the rotation component of a transform.
    for (unsigned int i = 0; i < _componentCount; i++)
    {
        if ((int) i == _rotationOffset)
        {
            Quaternion result;
            float interpTime = c0->time * eq0 + c1->time * eq1 + c2->time * eq2 + c3->time * eq3;
            interpolateQuaternion(interpTime, c1, c2, dst + i);
            // Increment counter by 3 because we've handled all the rotation components at once.
            i += 3;
        }
        else
        {
            dst[i] = c0->value[i] * eq0 + c1->value[i] * eq1 + c2->value[i] * eq2 + c3->value[i] * eq3;
        }
    }
}

void Curve::interpolateHermite(float s, Point* from, Point* to, float* dst)
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1
    float h10 = s_3 - 2 * s_2 + s;       // basis function 2
    float h11 = s_3 - s_2;               // basis function 3

    // Handles the rotation component of a transform.
    for (unsigned int i = 0; i < _componentCount; i++)
    {
        if ((int) i == _rotationOffset)
        {
            float interpTime = h01 * 1.0f + h10 * from->tangentOut[i] + h11 * to->tangentIn[i];
            interpolateQuaternion(interpTime, from, to, dst + i);
            // Increment counter by 3 because we've handled all the rotation components at once.
            i += 3;
        }
        else
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * from->tangentOut[i] + h11 * to->tangentIn[i];
        }
    }
}

void Curve::interpolateHermiteFlat(float s, Point* from, Point* to, float* dst)
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1

    // Handles the rotation component of a transform.
    for (unsigned int i = 0; i < _componentCount; i++)
    {
        if ((int) i == _rotationOffset)
        {
            float interpTime = h01 * 1.0f; // Can drop all other terms because they will compute to 0.
            interpolateQuaternion(interpTime, from, to, dst + i);
            // Increment counter by 3 because we've handled all the rotation components at once.
            i += 3;
        }
        else
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i];
        }
    }
}

void Curve::interpolateHermiteSmooth(float s, unsigned int index, Point* from, Point* to, float* dst)
{
    // Calculate the hermite basis functions.
    float s_2 = s * s;                   // t^2
    float s_3 = s_2 * s;                 // t^3
    float h00 = 2 * s_3 - 3 * s_2 + 1;   // basis function 0
    float h01 = -2 * s_3 + 3 * s_2;      // basis function 1
    float h10 = s_3 - 2 * s_2 + s;       // basis function 2
    float h11 = s_3 - s_2;               // basis function 3

    float tangentIn;
    float tangentOut;

    for (unsigned int i = 0; i < _componentCount; i++)
    {
        if ((int) i == _rotationOffset)
        {   
            if (index == 0)
            {
                tangentOut = to->time - from->time;
            }
            else
            {
                tangentOut = (to->time - (from - 1)->time) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                tangentIn = to->time - from->time;
            }
            else
            {
                tangentIn = ((to + 1)->time - from->time) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            float interpTime = h01 * 1.0f + h10 * tangentOut + h11 * tangentIn;
            interpolateQuaternion(interpTime, from, to, dst + i);

            // Increment counter by 3 because we've handled all the rotation components at once.
            i += 3;
        }
        else
        {
            if (index == 0)
            {
                tangentOut = to->value[i] - from->value[i];
            }
            else
            {
                tangentOut = (to->value[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                tangentIn = to->value[i] - from->value[i];
            }
            else
            {
                tangentIn = ((to + 1)->value[i] - from->value[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * tangentOut + h11 * tangentIn;
        }
    }
}

void Curve::interpolateQuaternion(float s, Point* from, Point* to, float* dst)
{
    Quaternion quatFrom(from->value + _rotationOffset);
    Quaternion quatTo(to->value + _rotationOffset);
    Quaternion result;

    // Normalize the quaternions.
    quatFrom.normalize();
    quatTo.normalize();
        
    // Evaluate.
    if (s > 0)
        Quaternion::slerp(quatFrom, quatTo, s, &result);
    else
        Quaternion::slerp(quatTo, quatFrom, -s, &result);

    // Place in destination.
    dst[0] = result.x;
    dst[1] = result.y;
    dst[2] = result.z;
    dst[3] = result.w;
}

void Curve::setRotationOffset(int propertyId)
{
    switch (propertyId)
    {
        case Transform::ANIMATE_ROTATE: 
        case Transform::ANIMATE_ROTATE_TRANSLATE:
            _rotationOffset = ANIMATION_ROTATE_OFFSET;
            break;
        case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
            _rotationOffset = ANIMATION_SRT_OFFSET;
            break;
    }
}

int Curve::determineIndex(float time)
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
