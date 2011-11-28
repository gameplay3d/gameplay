#include "Ray.h"

namespace gameplay
{

inline Ray& Ray::operator*=(const Matrix& matrix)
{
    transform(matrix);
    return *this;
}

inline Ray operator*(const Matrix& matrix, const Ray& ray)
{
    Ray r(ray);
    r.transform(matrix);
    return r;
}

}
