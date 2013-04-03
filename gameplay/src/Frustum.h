#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include "Matrix.h"
#include "Ray.h"
#include "Plane.h"

namespace gameplay
{

/**
 * Defines a 3-dimensional frustum.
 *
 * A frustum in computer graphics is generally a volume of 3D space,
 * defined as the part of a rectangular pyramid that lies between
 * two planes perpendicular to its center line. A frustum is often used
 * to represent what a "camera" sees in your 3D space.
 *
 * The Frustum class allows you to define a bounding frustum using a combined
 * matrix that is generally the product of a view matrix and a projection matrix.
 *
 * You can query a Frustum object for any one of its bounding planes,
 * for its corners, and for whether it intersects with a given object.
 * Since objects that don't intersect with your view frustum generally
 * don't need to be rendered, culling them quickly can save you a lot of
 * rendering time.
 */
class Frustum
{
public:

    /**
     * Constructs the default frustum (corresponds to the identity matrix).
     */
    Frustum();

    /**
     * Constructs a new frustum from the specified view projection matrix.
     *
     * @param matrix The view projection matrix to create this frustum from.
     */
    Frustum(const Matrix& matrix);

    /**
     * Constructs a new frustum from the given frustum.
     * 
     * @param frustum The frustum to create this frustum from.
     */
    Frustum(const Frustum& frustum);

    /**
     * Destructor.
     */
    ~Frustum();

    /**
     * Gets the near plane of the frustum.
     *
     * @return near The near plane.
     */
    const Plane& getNear() const;

    /**
     * Gets the far plane of the frustum.
     *
     * @return far The far plane.
     */
    const Plane& getFar() const;

    /**
     * Gets the left plane of the frustum.
     *
     * @return left The left plane.
     */
    const Plane& getLeft() const;

    /**
     * Gets the right plane of the frustum.
     *
     * @return right The right plane.
     */
    const Plane& getRight() const;

    /**
     * Gets the bottom plane of the frustum.
     *
     * @return bottom The bottom plane.
     */
    const Plane& getBottom() const;

    /**
     * Gets the top plane of the frustum.
     *
     * @return top The top plane.
     */
    const Plane& getTop() const;

    /**
     * Gets the projection matrix corresponding to the frustum in the specified matrix.
     * 
     * @param dst The projection matrix to copy into.
     */
    void getMatrix(Matrix* dst) const;

    /**
     * Gets the corners of the frustum in the specified array.
     *
     * The corners are stored in the following order:
     * (N-near, F-far, L-left, R-right, B-bottom, T-top)
     * LTN, LBN, RBN, RTN, RTF, RBF, LBF, LTF.
     * 
     * @param corners The array (of at least size 8) to store the corners in.
     */
    void getCorners(Vector3* corners) const;

    /**
     * Gets the corners of the frustum's near plane in the specified array.
     *
     * The corners are stored in the following order:
     * left-top, left-bottom, right-bottom, right-top.
     *
     * @param corners The array (of at least size 4) to store the corners in.
     */
    void getNearCorners(Vector3* corners) const;

    /**
     * Gets the corners of the frustum's far plane in the specified array.
     *
     * The corners are stored in the following order:
     * right-top, right-bottom, left-bottom, left-top.
     *
     * @param corners The array (of at least size 4) to store the corners in.
     */
    void getFarCorners(Vector3* corners) const;

    /**
     * Tests whether this frustum intersects the specified point.
     *
     * @param point The point to test intersection with.
     *
     * @return true if the specified point intersects this frustum; false otherwise.
     */
    bool intersects(const Vector3& point) const;

    /**
     * Tests whether this frustum intersects the specified point.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     *
     * @return true if the specified point intersects this frustum; false otherwise.
     */
    bool intersects(float x, float y, float z) const;

    /**
     * Tests whether this frustum intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * 
     * @return true if the specified bounding sphere intersects this frustum; false otherwise.
     */
    bool intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this frustum intersects the specified bounding box.
     *
     * @param box The bounding box to test intersection with.
     * 
     * @return true if the specified bounding box intersects this frustum; false otherwise.
     */
    bool intersects(const BoundingBox& box) const;

    /**
     * Tests whether this frustum intersects the specified plane.
     *
     * @param plane The plane to test intersection with.
     * 
     * @return Plane::INTERSECTS_BACK if the specified bounding object is in the negative half-space of
     *  this plane, Plane::INTERSECTS_FRONT if it is in the positive half-space of this plane,
     *  and Plane::INTERSECTS_INTERSECTING if it intersects this plane.
     */
    float intersects(const Plane& plane) const;

    /**
     * Tests whether this frustum intersects the specified ray.
     *
     * @param ray The ray to test intersection with.
     * 
     * @return Plane::INTERSECTS_BACK if the specified ray is in the negative half-space of
     *  this plane, Plane::INTERSECTS_FRONT if it is in the positive half-space of this plane,
     *  and Plane::INTERSECTS_INTERSECTING if it intersects this plane.
     */
    float intersects(const Ray& ray) const;

    /**
     * Sets this frustum to the specified frustum.
     *
     * @param frustum The frustum to set to.
     */
    void set(const Frustum& frustum);

    /**
     * Sets the frustum to the frustum corresponding to the specified view projection matrix.
     *
     * @param matrix The view projection matrix.
     */
    void set(const Matrix& matrix);

private:

    /**
     * Updates the planes of the frustum.
     */
    void updatePlanes();

    Plane _near;
    Plane _far;
    Plane _bottom;
    Plane _top;
    Plane _left;
    Plane _right;
    Matrix _matrix;
};

}

#endif
