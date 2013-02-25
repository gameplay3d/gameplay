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
    GP_ASSERT(dst);
    dst->set(_matrix);
}

void Frustum::getCorners(Vector3* corners) const
{
    getNearCorners(corners);
    getFarCorners(corners + 4);
}

void Frustum::getNearCorners(Vector3* corners) const
{
    GP_ASSERT(corners);

    Plane::intersection(_near, _left, _top, &corners[0]);
    Plane::intersection(_near, _left, _bottom, &corners[1]);
    Plane::intersection(_near, _right, _bottom, &corners[2]);
    Plane::intersection(_near, _right, _top, &corners[3]);
}

void Frustum::getFarCorners(Vector3* corners) const
{
    GP_ASSERT(corners);

    Plane::intersection(_far, _right, _top, &corners[0]);
    Plane::intersection(_far, _right, _bottom, &corners[1]);
    Plane::intersection(_far, _left, _bottom, &corners[2]);
    Plane::intersection(_far, _left, _top, &corners[3]);
}

bool Frustum::intersects(const Vector3& point) const
{
    if (_near.distance(point) <= 0)
        return false;
    if (_far.distance(point) <= 0)
        return false;
    if (_left.distance(point) <= 0)
        return false;
    if (_right.distance(point) <= 0)
        return false;
    if (_top.distance(point) <= 0)
        return false;
    if (_bottom.distance(point) <= 0)
        return false;

    return true;
}

bool Frustum::intersects(float x, float y, float z) const
{
    return intersects(Vector3(x, y, z));
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

void Frustum::updatePlanes()
{
    _near.set(Vector3(_matrix.m[3] + _matrix.m[2], _matrix.m[7] + _matrix.m[6], _matrix.m[11] + _matrix.m[10]), _matrix.m[15] + _matrix.m[14]);
    _far.set(Vector3(_matrix.m[3] - _matrix.m[2], _matrix.m[7] - _matrix.m[6], _matrix.m[11] - _matrix.m[10]), _matrix.m[15] - _matrix.m[14]);
    _bottom.set(Vector3(_matrix.m[3] + _matrix.m[1], _matrix.m[7] + _matrix.m[5], _matrix.m[11] + _matrix.m[9]), _matrix.m[15] + _matrix.m[13]);
    _top.set(Vector3(_matrix.m[3] - _matrix.m[1], _matrix.m[7] - _matrix.m[5], _matrix.m[11] - _matrix.m[9]), _matrix.m[15] - _matrix.m[13]);
    _left.set(Vector3(_matrix.m[3] + _matrix.m[0], _matrix.m[7] + _matrix.m[4], _matrix.m[11] + _matrix.m[8]), _matrix.m[15] + _matrix.m[12]);
    _right.set(Vector3(_matrix.m[3] - _matrix.m[0], _matrix.m[7] - _matrix.m[4], _matrix.m[11] - _matrix.m[8]), _matrix.m[15] - _matrix.m[12]);
}

void Frustum::set(const Matrix& matrix)
{
    _matrix.set(matrix);

    // Update the planes.
    updatePlanes();
}

}
