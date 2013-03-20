#ifndef BOUNDINGVOLUME_H_
#define BOUNDINGVOLUME_H_

#include "Vector3.h"
#include "Matrix.h"

namespace gameplay
{

/**
 * Represents a 3D bounding volumes, which defines both a
 * bounding sphere and an axis-aligned bounding box (AABB).
 */
class BoundingVolume
{
public:

    /**
     * Radius of the bounding sphere.
     */
    float radius;

    /**
     * Center point of the bounding sphere.
     */
    Vector3 center;

    /**
     * Minimum point of the AABB.
     */
    Vector3 min;

    /**
     * Maximum point of the AABB.
     */
    Vector3 max;

    /**
     * Constructor.
     */
    BoundingVolume();

    /**
     * Transforms this bounding volume by the specified matrix.
     */
    void transform(const Matrix& m);

    /**
     * Merges this bounding volume with the specified one and
     * stores the result in this BoundingVolume.
     */
    void merge(const BoundingVolume& v);
};

}

#endif
