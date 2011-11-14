/** 
 * Vector4.inl
 */

#include "Matrix.h"
#include "Vector4.h"

namespace gameplay
{

inline Vector4 Vector4::operator+(const Vector4& v)
{
    Vector4 result(*this);
    result.add(v);
    return result;
}

inline Vector4& Vector4::operator+=(const Vector4& v)
{
    add(v);
    return *this;
}

inline Vector4 Vector4::operator-(const Vector4& v)
{
    Vector4 result(*this);
    result.subtract(v);
    return result;
}

inline Vector4& Vector4::operator-=(const Vector4& v)
{
    subtract(v);
    return *this;
}

inline Vector4 Vector4::operator-()
{
    Vector4 result(*this);
    result.negate();
    return result;
}

inline Vector4 Vector4::operator*(float x)
{
    Vector4 result(*this);
    result.scale(x);
    return result;
}

inline Vector4& Vector4::operator*=(float x)
{
    scale(x);
    return *this;
}

}
