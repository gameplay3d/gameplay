/*
 * Ray.cpp
 */

#include "Base.h"
#include "Ray.h"
#include "Plane.h"
#include "Frustum.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace gameplay
{

Ray::Ray()
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

float Ray::intersects(const BoundingSphere& sphere) const
{
    return sphere.intersects(*this);
}

float Ray::intersects(const BoundingBox& box) const
{
    return box.intersects(*this);
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
    if ( (nOD < 0.0f && nD < 0.0f) || (fOD < 0.0f && fD < 0.0f) ||
        (lOD < 0.0f && lD < 0.0f)  || (rOD < 0.0f && rD < 0.0f) ||
        (bOD < 0.0f && bD < 0.0f)  || (tOD < 0.0f && tD < 0.0f) )
    {
        return RAY_INTERSECTS_NONE;
    }

    // Otherwise, the intersection distance is the minimum positive intersection distance.
    float d = (nD > 0.0f) ? nD : 0.0f;
    d = (fD > 0.0f) ? ( (d == 0.0f) ? fD : fminf(fD, d) ) : d;
    d = (lD > 0.0f) ? ( (d == 0.0f) ? lD : fminf(lD, d) ) : d;
    d = (rD > 0.0f) ? ( (d == 0.0f) ? rD : fminf(rD, d) ) : d;
    d = (tD > 0.0f) ? ( (d == 0.0f) ? bD : fminf(bD, d) ) : d;
    d = (bD > 0.0f) ? ( (d == 0.0f) ? tD : fminf(tD, d) ) : d;

    return d;
}

float Ray::intersects(const Plane& plane) const
{
    return plane.intersects(*this);
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
    matrix.transformPoint(&_origin);
    matrix.transformNormal(&_direction);
    _direction.normalize();
}

void Ray::normalize()
{
    if (_direction.isZero())
        return;

    // Normalize the ray's direction vector.
    float normalizeFactor = 1.0f / sqrtf(_direction.x * _direction.x + _direction.y * _direction.y + _direction.z * _direction.z);

    if ( normalizeFactor != 1.0f )
    {
        _direction.x *= normalizeFactor;
        _direction.y *= normalizeFactor;
        _direction.z *= normalizeFactor;
    }
}

}
