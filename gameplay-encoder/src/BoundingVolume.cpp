#include "Base.h"
#include "BoundingVolume.h"

namespace gameplay
{

BoundingVolume::BoundingVolume()
    : radius(0.0f)
{
}

void updateMinMax(Vector3* point, Vector3* min, Vector3* max)
{
    // Leftmost point.
    if (point->x < min->x)
    {
        min->x = point->x;
    }

    // Rightmost point.
    if (point->x > max->x)
    {
        max->x = point->x;
    }

    // Lowest point.
    if (point->y < min->y)
    {
        min->y = point->y;
    }

    // Highest point.
    if (point->y > max->y)
    {
        max->y = point->y;
    }

    // Farthest point.
    if (point->z < min->z)
    {
        min->z = point->z;
    }

    // Nearest point.
    if (point->z > max->z)
    {
        max->z = point->z;
    }
}

void BoundingVolume::transform(const Matrix& m)
{
    // Transform the bounding sphere
    m.transformPoint(center, &center);
    Vector3 translate;
    m.decompose(&translate, NULL, NULL);
    float r = radius * translate.x;
    r = std::max(radius, radius * translate.y);
    r = std::max(radius, radius * translate.z);
    radius = r;

    // Transform the bounding box
    Vector3 corners[8];
    corners[0].set(min.x, max.y, max.z);
    // Left-bottom-front.
    corners[1].set(min.x, min.y, max.z);
    // Right-bottom-front.
    corners[2].set(max.x, min.y, max.z);
    // Right-top-front.
    corners[3].set(max.x, max.y, max.z);
    // Right-top-back.
    corners[4].set(max.x, max.y, min.z);
    // Right-bottom-back.
    corners[5].set(max.x, min.y, min.z);
    // Left-bottom-back.
    corners[6].set(min.x, min.y, min.z);
    // Left-top-back.
    corners[7].set(min.x, max.y, min.z);

    // Transform the corners, recalculating the min and max points along the way.
    m.transformPoint(corners[0], &corners[0]);
    Vector3 newMin = corners[0];
    Vector3 newMax = corners[0];
    for (int i = 1; i < 8; i++)
    {
        m.transformPoint(corners[i], &corners[i]);
        updateMinMax(&corners[i], &newMin, &newMax);
    }
    min = newMin;
    max = newMax;
}

void BoundingVolume::merge(const BoundingVolume& v)
{
    // Merge the box portion
    min.x = std::min(min.x, v.min.x);
    min.y = std::min(min.y, v.min.y);
    min.z = std::min(min.z, v.min.z);
    max.x = std::max(max.x, v.max.x);
    max.y = std::max(max.y, v.max.y);
    max.z = std::max(max.z, v.max.z);

    // Merge the sphere portion
    // Calculate the distance between the two centers.
    float vx = center.x - v.center.x;
    float vy = center.y - v.center.y;
    float vz = center.z - v.center.z;
    float d = sqrt(vx * vx + vy * vy + vz * vz);

    // If one sphere is contained inside the other, set to the larger sphere.
    if (d <= (v.radius - radius))
    {
        // Use targert volume
        radius = v.radius;
        center = v.center;
    }
    else if (d <= (radius - v.radius))
    {
        // No change
    }
    else
    {
        // Calculate the unit vector between the two centers.
        float dI = 1.0f / d;
        vx *= dI;
        vy *= dI;
        vz *= dI;

        // Calculate the new radius.
        float r = (radius + v.radius + d) * 0.5f;

        // Calculate the new center.
        float scaleFactor = (r - v.radius);
        vx = vx * scaleFactor + v.center.x;
        vy = vy * scaleFactor + v.center.y;
        vz = vz * scaleFactor + v.center.z;

        // Set the new center and radius.
        center.x = vx;
        center.y = vy;
        center.z = vz;
        radius = r;
    }
}

}
