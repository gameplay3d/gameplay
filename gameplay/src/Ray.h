/*
 * Ray.h
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector3.h"

namespace gameplay
{

class Frustum;
class Plane;
class BoundingSphere;
class BoundingBox;

/**
 * Represents when a 3D entity does not intersect a ray.
 */
#define RAY_INTERSECTS_NONE            -1

/**
 * Defines a 3-dimensional ray.
 *
 * This class guarantees that its direction vector is always normalized.
 */
class Ray
{
public:

    /**
     * Constructor.
     */
    Ray();

    /**
     * Constructs a new ray initialized to the specified values.
     *
     * @param origin The ray's origin.
     * @param direction The ray's direction.
     */
    Ray(const Vector3& origin, const Vector3& direction);

    /**
     * Constructs a new ray from the given ray.
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
     * Tests whether this ray intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * 
     * @return The distance from the origin of this ray to the bounding object or
     *     RAY_INTERSECTS_NONE if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this ray intersects the specified bounding box.
     *
     * @param box The bounding box to test intersection with.
     * 
     * @return The distance from the origin of this ray to the bounding object or
     *     RAY_INTERSECTS_NONE if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingBox& box) const;

    /**
     * Tests whether this ray intersects the specified frustum.
     *
     * @param frustum The frustum to test intersection with.
     * 
     * @return The distance from the origin of this ray to the frustum or
     *     RAY_INTERSECTS_NONE if this ray does not intersect the frustum.
     */
    float intersects(const Frustum& frustum) const;

    /**
     * Tests whether this ray intersects the specified plane.
     *
     * @param plane The plane to test intersection with.
     * 
     * @return The distance from the origin of this ray to the plane or
     *     RAY_INTERSECTS_NONE if this ray does not intersect the plane.
     */
    float intersects(const Plane& plane) const;

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

private:

    /**
     * Normalizes the ray.
     */
    void normalize();

    Vector3 _origin;
    Vector3 _direction;
};

}

#endif
