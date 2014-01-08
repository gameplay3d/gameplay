#include "Base.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace gameplay
{

BoundingSphere::BoundingSphere()
    : radius(0)
{
}

BoundingSphere::BoundingSphere(const Vector3& center, float radius)
{
    set(center, radius);
}

BoundingSphere::BoundingSphere(const BoundingSphere& copy)
{
    set(copy);
}

BoundingSphere::~BoundingSphere()
{
}

const BoundingSphere& BoundingSphere::empty()
{
    static BoundingSphere s;
    return s;
}

bool BoundingSphere::intersects(const BoundingSphere& sphere) const
{
    // If the distance between the spheres' centers is less than or equal
    // to the sum of their radii, then the spheres intersect.
    float vx = sphere.center.x - center.x;
    float vy = sphere.center.y - center.y;
    float vz = sphere.center.z - center.z;

    return sqrt(vx * vx + vy * vy + vz * vz) <= (radius + sphere.radius);
}

bool BoundingSphere::intersects(const BoundingBox& box) const
{
    // Determine what point is closest; if the distance to that
    // point is less than the radius, then this sphere intersects.
    float cpX = center.x;
    float cpY = center.y;
    float cpZ = center.z;

    const Vector3& boxMin = box.min;
    const Vector3& boxMax = box.max;
    // Closest x value.
    if (center.x < boxMin.x)
    {
        cpX = boxMin.x;
    }
    else if (center.x > boxMax.x)
    {
        cpX = boxMax.x;
    }

    // Closest y value.
    if (center.y < boxMin.y)
    {
        cpY = boxMin.y;
    }
    else if (center.y > boxMax.y)
    {
        cpY = boxMax.y;
    }

    // Closest z value.
    if (center.z < boxMin.z)
    {
        cpZ = boxMin.z;
    }
    else if (center.z > boxMax.z)
    {
        cpZ = boxMax.z;
    }

    // Find the distance to the closest point and see if it is less than or equal to the radius.
    cpX -= center.x;
    cpY -= center.y;
    cpZ -= center.z;

    return sqrt(cpX * cpX + cpY * cpY + cpZ * cpZ) <= radius;
}

bool BoundingSphere::intersects(const Frustum& frustum) const
{
    // The sphere must either intersect or be in the positive half-space of all six planes of the frustum.
    return (intersects(frustum.getNear()) != Plane::INTERSECTS_BACK &&
            intersects(frustum.getFar()) != Plane::INTERSECTS_BACK &&
            intersects(frustum.getLeft()) != Plane::INTERSECTS_BACK &&
            intersects(frustum.getRight()) != Plane::INTERSECTS_BACK &&
            intersects(frustum.getBottom()) != Plane::INTERSECTS_BACK &&
            intersects(frustum.getTop()) != Plane::INTERSECTS_BACK );
}

float BoundingSphere::intersects(const Plane& plane) const
{
    float distance = plane.distance(center);

    if (fabsf(distance) <= radius)
    {
        return Plane::INTERSECTS_INTERSECTING;
    }
    else if (distance > 0.0f)
    {
        return Plane::INTERSECTS_FRONT;
    }
    else
    {
        return Plane::INTERSECTS_BACK;
    }
}

float BoundingSphere::intersects(const Ray& ray) const
{
    const Vector3& origin = ray.getOrigin();
    const Vector3& direction = ray.getDirection();

    // Calculate the vector and the square of the distance from the ray's origin to this sphere's center.
    float vx = origin.x - center.x;
    float vy = origin.y - center.y;
    float vz = origin.z - center.z;
    float d2 = vx * vx + vy * vy + vz * vz;

    // Solve the quadratic equation using the ray's and sphere's equations together.
    // Since the ray's direction is guaranteed to be 1 by the Ray, we don't need to
    // calculate and use A (A=ray.getDirection().lengthSquared()).
    float B = 2.0f * (vx * direction.x + vy * direction.y + vz * direction.z);
    float C = d2 - radius * radius;
    float discriminant = B * B - 4.0f * C;

    // If the discriminant is negative, then there is no intersection.
    if (discriminant < 0.0f)
    {
        return Ray::INTERSECTS_NONE;
    }
    else
    {
        // The intersection is at the smaller positive root.
        float sqrtDisc = sqrt(discriminant);
        float t0 = (-B - sqrtDisc) * 0.5f;
        float t1 = (-B + sqrtDisc) * 0.5f;
        return (t0 > 0.0f && t0 < t1) ? t0 : t1;
    }
}

bool BoundingSphere::isEmpty() const
{
    return radius == 0.0f;
}

void BoundingSphere::merge(const BoundingSphere& sphere)
{
    if (sphere.isEmpty())
        return;

    // Calculate the distance between the two centers.
    float vx = center.x - sphere.center.x;
    float vy = center.y - sphere.center.y;
    float vz = center.z - sphere.center.z;
    float d = sqrt(vx * vx + vy * vy + vz * vz);

    // If one sphere is contained inside the other, set to the larger sphere.
    if (d <= (sphere.radius - radius))
    {
        center = sphere.center;
        radius = sphere.radius;
        return;
    }
    else if (d <= (radius - sphere.radius))
    {
        return;
    }

    // Calculate the unit vector between the two centers.
    GP_ASSERT(d != 0.0f);
    float dI = 1.0f / d;
    vx *= dI;
    vy *= dI;
    vz *= dI;

    // Calculate the new radius.
    float r = (radius + sphere.radius + d) * 0.5f;

    // Calculate the new center.
    float scaleFactor = (r - sphere.radius);
    vx = vx * scaleFactor + sphere.center.x;
    vy = vy * scaleFactor + sphere.center.y;
    vz = vz * scaleFactor + sphere.center.z;

    // Set the new center and radius.
    center.x = vx;
    center.y = vy;
    center.z = vz;
    radius = r;
}

void BoundingSphere::merge(const BoundingBox& box)
{
    if (box.isEmpty())
        return;

    const Vector3& min = box.min;
    const Vector3& max = box.max;

    // Find the corner of the bounding box that is farthest away from this sphere's center.
    float v1x = min.x - center.x;
    float v1y = min.y - center.y;
    float v1z = min.z - center.z;
    float v2x = max.x - center.x;
    float v2y = max.y - center.y;
    float v2z = max.z - center.z;
    float fx = min.x;
    float fy = min.y;
    float fz = min.z;

    if (v2x > v1x)
    {
        fx = max.x;
    }
    if (v2y > v1y)
    {
        fy = max.y;
    }
    if (v2z > v1z)
    {
        fz = max.z;
    }

    // Calculate the unit vector and the distance between the center and the farthest point.
    v1x = center.x - fx;
    v1y = center.y - fy;
    v1z = center.z - fz;
    float distance = sqrt(v1x * v1x + v1y * v1y + v1z * v1z);

    // If the box is inside the sphere, we are done.
    if (distance <= radius)
    {
        return;
    }

    // Calculate the unit vector between the center and the farthest point.
    GP_ASSERT(distance != 0.0f);
    float dI = 1.0f / distance;
    v1x *= dI;
    v1y *= dI;
    v1z *= dI;

    // Calculate the new radius.
    float r = (radius + distance) * 0.5f;

    // Calculate the new center.
    v1x = v1x * r + fx;
    v1y = v1y * r + fy;
    v1z = v1z * r + fz;

    // Set the new center and radius.
    center.x = v1x;
    center.y = v1y;
    center.z = v1z;
    radius = r;
}

void BoundingSphere::set(const Vector3& center, float radius)
{
    this->center = center;
    this->radius = radius;
}

void BoundingSphere::set(const BoundingSphere& sphere)
{
    center = sphere.center;
    radius = sphere.radius;
}

void BoundingSphere::set(const BoundingBox& box)
{
    center.x = (box.min.x + box.max.x) * 0.5f;
    center.y = (box.min.y + box.max.y) * 0.5f;
    center.z = (box.min.z + box.max.z) * 0.5f;
    radius = center.distance(box.max);
}

void BoundingSphere::transform(const Matrix& matrix)
{
    // Translate the center point.
    matrix.transformPoint(center, &center);

    // Scale the sphere's radius by the scale fo the matrix
    Vector3 scale;
    matrix.decompose(&scale, NULL, NULL);
    float r = radius * scale.x;
    r = max(r, radius * scale.y);
    r = max(r, radius * scale.z);
    radius = r;
}

float BoundingSphere::distance(const BoundingSphere& sphere, const Vector3& point)
{
    return sqrt((point.x - sphere.center.x) * (point.x - sphere.center.x) +
                 (point.y - sphere.center.y) * (point.y - sphere.center.x) +
                 (point.z - sphere.center.z) * (point.z - sphere.center.x));
}

bool BoundingSphere::contains(const BoundingSphere& sphere, Vector3* points, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        if (distance(sphere, points[i]) > sphere.radius)
        {
            return false;
        }
    }
    return true;
}

}
