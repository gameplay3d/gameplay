#include "Plane.h"

namespace gameplay
{

inline Plane& Plane::operator*=(const Matrix& matrix)
{
    transform(matrix);
    return *this;
}

inline const Plane operator*(const Matrix& matrix, const Plane& plane)
{
    Plane p(plane);
    p.transform(matrix);
    return p;
}

}
