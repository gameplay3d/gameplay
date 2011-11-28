#include "Base.h"
#include "Curve.h"
#include "Transform.h"

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
            break;
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

        delete[] _quaternionOffsets;    // delete old array
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

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = from->value[i] * eq1 + from->outValue[i] * eq2 + to->inValue[i] * eq3 + to->value[i] * eq4;
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
                dst[i] = from->value[i] * eq1 + from->outValue[i] * eq2 + to->inValue[i] * eq3 + to->value[i] * eq4;
                i++;
            }
            // Handle quaternion component.
            //float interpTime = from->time * eq1 + from->outValue[i] * eq2 + to->inValue[i] * eq3 + to->time * eq4;
            interpolateQuaternion(s, (from->value + i), (to->value + i), (dst + i));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        while (i < _componentCount)
        {
            dst[i] = from->value[i] * eq1 + from->outValue[i] * eq2 + to->inValue[i] * eq3 + to->value[i] * eq4;
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

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = c0->value[i] * eq0 + c1->value[i] * eq1 + c2->value[i] * eq2 + c3->value[i] * eq3;
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
                dst[i] = c0->value[i] * eq0 + c1->value[i] * eq1 + c2->value[i] * eq2 + c3->value[i] * eq3;
                i++;
            }
            // Handle quaternion component.
            float interpTime;
            if (c0->time == c1->time)
                interpTime = -c0->time * eq0 + c1->time * eq1 + c2->time * eq2 + c3->time * eq3;
            else if (c2->time == c3->time)
                interpTime = c0->time * eq0 + c1->time * eq1 + c2->time * eq2  - c3->time * eq3;
            else
                interpTime = c0->time * eq0 + c1->time * eq1 + c2->time * eq2 + c3->time * eq3;
            
            interpolateQuaternion(s, (c1->value + quaternionOffset) , (c2->value + quaternionOffset), (dst + quaternionOffset));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);
        
        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = c0->value[i] * eq0 + c1->value[i] * eq1 + c2->value[i] * eq2 + c3->value[i] * eq3;
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

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * from->outValue[i] + h11 * to->inValue[i];
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
                dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * from->outValue[i] + h11 * to->inValue[i];
                i++;
            }
            // Handle quaternion component.
            float interpTime = h01 * 1.0f + h10 * from->outValue[quaternionOffset] + h11 * to->inValue[quaternionOffset];
            interpolateQuaternion(interpTime, (from->value + quaternionOffset), (to->value + quaternionOffset), (dst + quaternionOffset));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
            
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);
       
        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * from->outValue[i] + h11 * to->inValue[i];
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

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i];
        }
    }
    else
    {
        // Interpolate values as scalars up to first quaternion offset.
        unsigned int quaternionOffsetIndex = 0;
        unsigned int quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        unsigned int i = 0;
        float interpTime = h01 * 1.0f; // Can drop all other terms because they will compute to 0. Only need to compute once.
        do {
            while (i < quaternionOffset)
            {
                dst[i] = h00 * from->value[i] + h01 * to->value[i];
                i++;
            }
            // We've hit a quaternion component, so handle it. increase the component counter by 4, and increase quaternionOffsetIndex
            interpolateQuaternion(interpTime, (from->value + quaternionOffset), (to->value + quaternionOffset), (dst + quaternionOffset));
            i += 4;
            quaternionOffsetIndex++;
            quaternionOffset = _quaternionOffsets[quaternionOffsetIndex];
        } while (quaternionOffsetIndex < _quaternionOffsetsCount);

        // Handle remaining scalar values.
        while (i < _componentCount)
        {
            dst[i] = h00 * from->value[i] + h01 * to->value[i];
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

    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            if (index == 0)
            {
                outValue = to->value[i] - from->value[i];
            }
            else
            {
                outValue = (to->value[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                inValue = to->value[i] - from->value[i];
            }
            else
            {
                inValue = ((to + 1)->value[i] - from->value[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * outValue + h11 * inValue;
        }
    }
    else
    {
        // Calculates in/out values for interpolating the time for the quaternion component.
        // Only need to calculate this once.
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
                    outValue = to->value[i] - from->value[i];
                }
                else
                {
                    outValue = (to->value[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
                }

                if (index == _pointCount - 2)
                {
                    inValue = to->value[i] - from->value[i];
                }
                else
                {
                    inValue = ((to + 1)->value[i] - from->value[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
                }

                dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * outValue + h11 * inValue;
                i++;
            }
            
            float interpTime = h01 * 1.0f + h10 * outValue + h11 * inValue;
            interpolateQuaternion(interpTime, (from->value + quaternionOffset), (to->value + quaternionOffset), (dst + quaternionOffset));
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
                outValue = to->value[i] - from->value[i];
            }
            else
            {
                outValue = (to->value[i] - (from - 1)->value[i]) * ((from->time - (from - 1)->time) / (to->time - (from - 1)->time));
            }

            if (index == _pointCount - 2)
            {
                inValue = to->value[i] - from->value[i];
            }
            else
            {
                inValue = ((to + 1)->value[i] - from->value[i]) * ((to->time - from->time) / ((to + 1)->time - from->time));
            }

            dst[i] = h00 * from->value[i] + h01 * to->value[i] + h10 * outValue + h11 * inValue;
            i++;
        }
    }
}

void Curve::interpolateLinear(float s, Point* from, Point* to, float* dst) const
{
    if (!_quaternionOffsets)
    {
        for (unsigned int i = 0; i < _componentCount; i++)
        {
            dst[i] = from->value[i] + (to->value[i] - from->value[i]) * s; 
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
                dst[i] = from->value[i] + (to->value[i] - from->value[i]) * s; 
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
            dst[i] = from->value[i] + (to->value[i] - from->value[i]) * s; 
            i++;
        }
    }
}

void Curve::interpolateQuaternion(float s, float* from, float* to, float* dst) const
{
    Quaternion quatFrom(from);
    Quaternion quatTo(to);
    Quaternion result;

    // Normalize the quaternions.
    quatFrom.normalize();
    quatTo.normalize();
        
    // Evaluate.
    if (s >= 0)
        Quaternion::slerp(quatFrom, quatTo, s, &result);
    else
        Quaternion::slerp(quatTo, quatFrom, -s, &result);

    // Place in destination.
    dst[0] = result.x;
    dst[1] = result.y;
    dst[2] = result.z;
    dst[3] = result.w;
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
