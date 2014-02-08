#ifndef BOUNDINGSPHERE_H_
#define BOUNDINGSPHERE_H_

#include "Frustum.h"

namespace gameplay
{

/**
 * Defines a 3-dimensional bounding sphere.
 */
class BoundingSphere
{
public:

    /**
     * The center point.
     */
    Vector3 center;

    /**
     * The sphere radius.
     */
    float radius;

    /**
     *  Constructs a new bounding sphere initialized to all zeros.
     */
    BoundingSphere();

    /**
     * Constructs a new bounding sphere initialized to the specified values.
     *
     * @param center The center of the sphere.
     * @param radius The radius of the sphere.
     */
    BoundingSphere(const Vector3& center, float radius);

    /**
     * Constructs a bounding sphere from the given bounding sphere.
     *
     * @param copy The bounding sphere to copy.
     */
    BoundingSphere(const BoundingSphere& copy);

    /**
     * Destructor.
     */
    ~BoundingSphere();

    /**
     * Returns an empty bounding sphere.
     */
    static const BoundingSphere& empty();

    /**
     * Tests whether this bounding sphere intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * 
     * @return true if the specified bounding sphere intersects this bounding sphere; false otherwise.
     */
    bool intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this bounding sphere intersects the specified bounding box.
     *
     * @param box The bounding box to test intersection with.
     * 
     * @return true if the specified bounding box intersects this bounding sphere; false otherwise.
     */
    bool intersects(const BoundingBox& box) const;

    /**
     * Tests whether this bounding sphere intersects the specified frustum.
     *
     * @param frustum The frustum to test intersection with.
     * 
     * @return true if this bounding sphere intersects the specified frustum; false otherwise.
     */
    bool intersects(const Frustum& frustum) const;

    /**
     * Tests whether this bounding sphere intersects the specified plane.
     *
     * @param plane The plane to test intersection with.
     * 
     * @return Plane::INTERSECTS_BACK INTERSECTS_BACK if this bounding sphere is in the negative half-space of
     *  the plane, Plane::INTERSECTS_FRONT INTERSECTS_FRONT if it is in the positive half-space of the plane,
     *  and Plane::INTERSECTS_INTERSECTING INTERSECTS_INTERSECTING if it intersects the plane.
     */
    float intersects(const Plane& plane) const;

    /**
     * Tests whether this bounding sphere intersects the specified ray.
     *
     * @param ray The ray to test intersection with.
     * 
     * @return The distance from the origin of the ray to this bounding sphere or
     *  Ray::INTERSECTS_NONE INTERSECTS_NONE if the ray does not intersect this bounding sphere.
     */
    float intersects(const Ray& ray) const;

    /**
     * Determines if this bounding sphere is empty.
     *
     * @return true if this bounding sphere is empty; false otherwise.
     */
    bool isEmpty() const;

    /**
     * Sets this bounding sphere to the smallest bounding sphere
     * that contains both this bounding sphere and the specified bounding sphere.
     *
     * @param sphere The bounding sphere to merge with.
     */
    void merge(const BoundingSphere& sphere);

    /**
     * Sets this bounding sphere to the smallest bounding sphere
     * that contains both this bounding sphere and the specified bounding box.
     *
     * @param box The bounding box to merge with.
     */
    void merge(const BoundingBox& box);

    /**
     * Sets this bounding sphere to the specified values.
     *
     * @param center The center of the sphere.
     * @param radius The radius of the sphere.
     */
    void set(const Vector3& center, float radius);

    /**
     * Sets this bounding sphere to the specified bounding sphere.
     *
     * @param sphere The bounding sphere to set to.
     */
    void set(const BoundingSphere& sphere);

    /**
     * Sets this bounding sphere to tightly contain the specified bounding box.
     *
     * @param box The box to contain.
     */
    void set(const BoundingBox& box);

    /**
     * Transforms the bounding sphere by the given transformation matrix.
     *
     * @param matrix The transformation matrix to transform by.
     */
    void transform(const Matrix& matrix);

    /**
     * Transforms this bounding sphere by the given matrix.
     * 
     * @param matrix The matrix to transform by.
     * @return This bounding sphere, after the transformation occurs.
     */
    inline BoundingSphere& operator*=(const Matrix& matrix);

private:

    float distance(const BoundingSphere& sphere, const Vector3&);

    bool contains(const BoundingSphere& sphere, Vector3* points, unsigned int count);
};

/**
 * Transforms the given bounding sphere by the given matrix.
 * 
 * @param matrix The matrix to transform by.
 * @param sphere The bounding sphere to transform.
 * @return The resulting transformed bounding sphere.
 */
inline const BoundingSphere operator*(const Matrix& matrix, const BoundingSphere& sphere);

}

#include "BoundingSphere.inl"

#endif
