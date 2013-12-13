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
 * Defines a 3-dimensional ray.
 *
 * Rays direction vector are always normalized.
 */
class Ray
{
public:

    /**
     * Represents when a 3D entity does not intersect a ray.
     */
    static const int INTERSECTS_NONE = -1;

    /**
     * Constructs a new ray initialized to origin(0,0,0) and direction(0,0,1).
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
     * Constructs a new ray initialized to the specified values.
     * 
     * @param originX The x coordinate of the origin.
     * @param originY The y coordinate of the origin.
     * @param originZ The z coordinate of the origin.
     * @param dirX The x coordinate of the direction.
     * @param dirY The y coordinate of the direction.
     * @param dirZ The z coordinate of the direction.
     */
    Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

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
     * Sets the ray's origin.
     * 
     * @param x The x coordinate of the origin.
     * @param y The y coordinate of the origin.
     * @param z The z coordinate of the origin.
     */
    void setOrigin(float x, float y, float z);

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
     * Sets the ray's direction.
     * 
     * @param x The x coordinate of the direction.
     * @param y The y coordinate of the direction.
     * @param z The z coordinate of the direction.
     */
    void setDirection(float x, float y, float z);

    /**
     * Tests whether this ray intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * 
     * @return The distance from the origin of this ray to the bounding object or
     *     INTERSECTS_NONE if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this ray intersects the specified bounding box.
     *
     * @param box The bounding box to test intersection with.
     * 
     * @return The distance from the origin of this ray to the bounding object or
     *     INTERSECTS_NONE if this ray does not intersect the bounding object.
     */
    float intersects(const BoundingBox& box) const;

    /**
     * Tests whether this ray intersects the specified frustum.
     *
     * @param frustum The frustum to test intersection with.
     * 
     * @return The distance from the origin of this ray to the frustum or
     *     INTERSECTS_NONE if this ray does not intersect the frustum.
     */
    float intersects(const Frustum& frustum) const;

    /**
     * Tests whether this ray intersects the specified plane and returns the distance
     * from the origin of the ray to the plane.
     *
     * @param plane The plane to test intersection with.
     * 
     * @return The distance from the origin of this ray to the plane or
     *     INTERSECTS_NONE if this ray does not intersect the plane.
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

    /**
     * Transforms this ray by the given matrix.
     * 
     * @param matrix The matrix to transform by.
     * @return This ray, after the transformation occurs.
     */
    inline Ray& operator*=(const Matrix& matrix);

private:

    /**
     * Normalizes the ray.
     */
    void normalize();

    Vector3 _origin;        // The ray origin position.
    Vector3 _direction;     // The ray direction vector.
};

/**
 * Transforms the given ray by the given matrix.
 * 
 * @param matrix The matrix to transform by.
 * @param ray The ray to transform.
 * @return The resulting transformed ray.
 */
inline const Ray operator*(const Matrix& matrix, const Ray& ray);

}

#include "Ray.inl"

#endif
