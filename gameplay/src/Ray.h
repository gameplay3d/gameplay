#pragma once

#include "Vector3.h"

namespace gameplay
{

class Frustum;
class Plane;
class BoundingSphere;
class BoundingBox;

/**
 * Defines a 3-dimensional ray.
 *
 * Rays direction vector are always normalized.
 */
class Ray
{
public:

    /**
     * Represents when a entity does not intersect a ray.
     */
    static const int kIntersectsNone = -1;

    /**
     * Constructor.
     *
     * The origin is (0, 0, 0) and direction is (0, 0, 1)
     */
    Ray();

    /**
     * Constructor.
     *
     * @param origin The ray's origin.
     * @param direction The ray's direction.
     */
    Ray(const Vector3& origin, const Vector3& direction);

    /**
     * Constructor.
     *
     * @param copy The ray to copy.
     */
    Ray(const Ray& copy);

    /**
     * Destructor.
     */
    ~Ray();

    /**
     * Gets the ray's origin.
     *
     * @return The ray's origin.
     */
    const Vector3& getOrigin() const;

    /**
     * Sets the ray's origin to the given point.
     *
     * @param origin The new origin.
     */
    void setOrigin(const Vector3& origin);

    /**
     * Gets the ray's direction.
     *
     * @return The ray's direction.
     */
    const Vector3& getDirection() const;

    /**
     * Sets the ray's direction to the given vector.
     *
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3& direction);

    /**
     * Tests whether this ray intersects the specified frustum.
     *
     * @param frustum The frustum to test intersection with.
     * @return The distance from the origin of this ray to the frustum or
     *     kIntersectsNone if this ray does not intersect the frustum.
     */
    float intersects(const Frustum& frustum) const;

    /**
     * Tests whether this ray intersects the specified plane and returns the distance
     * from the origin of the ray to the plane.
     *
     * @param plane The plane to test intersection with.
     * @return The distance from the origin of this ray to the plane or
     *     kIntersectsNone if this ray does not intersect the plane.
     */
    float intersects(const Plane& plane) const;

    /**
     * Tests whether this ray intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * @return The distance from the origin of this ray to the bounding object or
     *     kIntersectsNone if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this ray intersects the specified bounding box.
     *
     * @param box The bounding box to test intersection with.
     * 
     * @return The distance from the origin of this ray to the bounding object or
     *     kIntersectsNone if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingBox& box) const;

    /**
     * Sets this ray to the specified values.
     *
     * @param origin The ray's origin.
     * @param direction The ray's direction.
     */
    void set(const Vector3& origin, const Vector3& direction);

    /**
     * Sets this ray to the given ray.
     *
     * @param ray The ray to copy.
     */
    void set(const Ray& ray);

    /**
     * Transforms this ray by the given transformation matrix.
     *
     * @param matrix The transformation matrix to transform by.
     */
    void transform(const Matrix& matrix);

    /**
     * operator =
     */
    Ray& operator=(const Ray& v);

    /**
     * Transforms this ray by the given matrix.
     * 
     * @param matrix The matrix to transform by.
     * @return This ray, after the transformation occurs.
     */
    Ray& operator*=(const Matrix& matrix);

private:

    void normalize();

    Vector3 _origin;
    Vector3 _direction;
};

/**
 * Transforms the given ray by the given matrix.
 * 
 * @param matrix The matrix to transform by.
 * @param ray The ray to transform.
 * @return The resulting transformed ray.
 */
const Ray operator*(const Matrix& matrix, const Ray& ray);

}
