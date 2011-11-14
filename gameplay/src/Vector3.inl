/** 
 * Vector3.inl
 */

#include "Vector3.h"
#include "Matrix.h"

namespace gameplay
{

inline Vector3 Vector3::operator+(const Vector3& v)
{
    Vector3 result(*this);
    result.add(v);
    return result;
}

inline Vector3& Vector3::operator+=(const Vector3& v)
{
    add(v);
    return *this;
}

inline Vector3 Vector3::operator-(const Vector3& v)
{
    Vector3 result(*this);
    result.subtract(v);
    return result;
}

inline Vector3& Vector3::operator-=(const Vector3& v)
{
    subtract(v);
    return *this;
}

inline Vector3 Vector3::operator-()
{
    Vector3 result(*this);
    result.negate();
    return result;
}

inline Vector3 Vector3::operator*(float x)
{
    Vector3 result(*this);
    result.scale(x);
    return result;
}

inline Vector3& Vector3::operator*=(float x)
{
    scale(x);
    return *this;
}

}
