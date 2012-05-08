#include "Base.h"
#include "Frustum.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace gameplay
{

Frustum::Frustum()
{
    set(Matrix::identity());
}

Frustum::Frustum(const Matrix& matrix)
{
    set(matrix);
}

Frustum::Frustum(const Frustum& frustum)
{
    set(frustum);
}

Frustum::~Frustum()
{
}

const Plane& Frustum::getNear() const
{
    return _near;
}

const Plane& Frustum::getFar() const
{
    return _far;
}

const Plane& Frustum::getLeft() const
{
    return _left;
}

const Plane& Frustum::getRight() const
{
    return _right;
}

const Plane& Frustum::getBottom() const
{
    return _bottom;
}

const Plane& Frustum::getTop() const
{
    return _top;
}

void Frustum::getMatrix(Matrix* dst) const
{
    dst->set(_matrix);
}

void Frustum::getCorners(Vector3* corners) const
{
    GP_ASSERT(corners);

    Plane::intersection(_near, _left, _top, &corners[0]);
    Plane::intersection(_near, _left, _bottom, &corners[1]);
    Plane::intersection(_near, _right, _bottom, &corners[2]);
    Plane::intersection(_near, _right, _top, &corners[3]);
    Plane::intersection(_far, _right, _top, &corners[4]);
    Plane::intersection(_far, _right, _bottom, &corners[5]);
    Plane::intersection(_far, _left, _bottom, &corners[6]);
    Plane::intersection(_far, _left, _top, &corners[7]);
}

bool Frustum::intersects(const BoundingSphere& sphere) const
{
    return sphere.intersects(*this);
}

bool Frustum::intersects(const BoundingBox& box) const
{
    return box.intersects(*this);
}

float Frustum::intersects(const Plane& plane) const
{
    return plane.intersects(*this);
}

float Frustum::intersects(const Ray& ray) const
{
    return ray.intersects(*this);
}

void Frustum::set(const Frustum& frustum)
{
    _near = frustum._near;
    _far = frustum._far;
    _bottom = frustum._bottom;
    _top = frustum._top;
    _left = frustum._left;
    _right = frustum._right;
    _matrix.set(frustum._matrix);
}

void updatePlane(const Matrix& matrix, Plane* dst)
{
    GP_ASSERT(dst);

    dst->setNormal(Vector3(matrix.m[3] + matrix.m[2], matrix.m[7] + matrix.m[6], matrix.m[11] + matrix.m[10]));
    dst->setDistance(matrix.m[15] + matrix.m[14]);

    Vector3 normal = dst->getNormal();
    if (!normal.isZero())
    {
        float normalizeFactor = 1.0f / sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (normalizeFactor != 1.0f)
        {
            dst->setNormal(Vector3(normal.x * normalizeFactor, normal.y * normalizeFactor, normal.z * normalizeFactor));
            dst->setDistance(dst->getDistance() * normalizeFactor);
        }
    }
}

void Frustum::set(const Matrix& matrix)
{
    _matrix.set(matrix);

    // Update the planes.
    updatePlane(matrix, &_near);
    updatePlane(matrix, &_far);
    updatePlane(matrix, &_bottom);
    updatePlane(matrix, &_top);
    updatePlane(matrix, &_left);
    updatePlane(matrix, &_right);
}

}
