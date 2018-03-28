#include "Base.h"
#include "Ray.h"
#include "Plane.h"
#include "Frustum.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace gameplay
{

Ray::Ray() : 
    _origin(0, 0, 0),
    _direction(0, 0, 1)
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction)
{
    set(origin, direction);
}

Ray::Ray(const Ray& copy)
{
    set(copy);
}

Ray::~Ray()
{
}

const Vector3& Ray::getOrigin() const
{
    return _origin;
}

void Ray::setOrigin(const Vector3& origin)
{
    _origin = origin;
}

const Vector3& Ray::getDirection() const
{
    return _direction;
}

void Ray::setDirection(const Vector3& direction)
{
    _direction = direction;
    normalize();
}

float Ray::intersects(const Frustum& frustum) const
{
    Plane n = frustum.getNear();
    float nD = intersects(n);
    float nOD = n.distance(_origin);

    Plane f = frustum.getFar();
    float fD = intersects(f);
    float fOD = f.distance(_origin);

    Plane l = frustum.getLeft();
    float lD = intersects(l);
    float lOD = l.distance(_origin);

    Plane r = frustum.getRight();
    float rD = intersects(r);
    float rOD = r.distance(_origin);

    Plane b = frustum.getBottom();
    float bD = intersects(b);
    float bOD = b.distance(_origin);

    Plane t = frustum.getTop();
    float tD = intersects(t);
    float tOD = t.distance(_origin);

    // If the ray's origin is in the negative half-space of one of the frustum's planes
    // and it does not intersect that same plane, then it does not intersect the frustum.
    if ((nOD < 0.0f && nD < 0.0f) || (fOD < 0.0f && fD < 0.0f) ||
        (lOD < 0.0f && lD < 0.0f)  || (rOD < 0.0f && rD < 0.0f) ||
        (bOD < 0.0f && bD < 0.0f)  || (tOD < 0.0f && tD < 0.0f))
    {
        return (float)Ray::kIntersectsNone;
    }
    // Otherwise, the intersection distance is the minimum positive intersection distance.
    float d = (nD > 0.0f) ? nD : 0.0f;
    d = (fD > 0.0f) ? ((d == 0.0f) ? fD : std::min(fD, d)) : d;
    d = (lD > 0.0f) ? ((d == 0.0f) ? lD : std::min(lD, d)) : d;
    d = (rD > 0.0f) ? ((d == 0.0f) ? rD : std::min(rD, d)) : d;
    d = (tD > 0.0f) ? ((d == 0.0f) ? bD : std::min(bD, d)) : d;
    d = (bD > 0.0f) ? ((d == 0.0f) ? tD : std::min(tD, d)) : d;

    return d;
}

float Ray::intersects(const Plane& plane) const
{
    const Vector3& normal = plane.getNormal();
    // If the origin of the ray is on the plane then the distance is zero.
    float alpha = (normal.dot(_origin) + plane.getDistance());
    if (std::fabs(alpha) < GP_MATH_EPSILON)
    {
        return 0.0f;
    }
    float dot = normal.dot(_direction);
    // If the dot product of the plane's normal and this ray's direction is zero,
    // then the ray is parallel to the plane and does not intersect it.
    if (dot == 0.0f)
    {
        return (float)kIntersectsNone;
    }
    // Calculate the distance along the ray's direction vector to the point where
    // the ray intersects the plane (if it is negative the plane is behind the ray).
    float d = -alpha / dot;
    if (d < 0.0f)
    {
        return (float)kIntersectsNone;
    }
    return d;
}

float Ray::intersects(const BoundingSphere& sphere) const
{
    return sphere.intersects(*this);
}

float Ray::intersects(const BoundingBox& box) const
{
    return box.intersects(*this);
}

void Ray::set(const Vector3& origin, const Vector3& direction)
{
    _origin = origin;
    _direction = direction;
    normalize();
}

void Ray::set(const Ray& ray)
{
    _origin = ray._origin;
    _direction = ray._direction;
    normalize();
}

void Ray::transform(const Matrix& matrix)
{
    matrix.transformPoint(_origin, &_origin);
    matrix.transformVector(_direction, &_direction);
    _direction.normalize();
}

void Ray::normalize()
{
    if (_direction.isZero())
        GP_ERROR("Invalid ray object; a ray's direction must be non-zero.");

    // Normalize the ray's direction vector.
    float normalizeFactor = 1.0f / std::sqrt(_direction.x * _direction.x + _direction.y * _direction.y + _direction.z * _direction.z);
    if (normalizeFactor != 1.0f)
    {
        _direction.x *= normalizeFactor;
        _direction.y *= normalizeFactor;
        _direction.z *= normalizeFactor;
    }
}

Ray& Ray::operator=(const Ray& r)
{
    if(&r == this)
        return *this;

    _origin = r._origin;
    _direction = r._direction;

    return *this;
}

Ray& Ray::operator*=(const Matrix& matrix)
{
    transform(matrix);
    return *this;
}

const Ray operator*(const Matrix& matrix, const Ray& ray)
{
    Ray r(ray);
    r.transform(matrix);
    return r;
}

}
