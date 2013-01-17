#include "Base.h"
#include "Heightmap.h"
#include "GPBFile.h"
#include "Thread.h"

namespace gameplay
{

// Number of threads to spawn for the heightmap generator
#define THREAD_COUNT 8

// Thread data structure
struct HeightmapThreadData
{
    float rayHeight;                    // [in]
    const Vector3* rayDirection;        // [in]
    const std::vector<Mesh*>* meshes;   // [in]
    const BoundingVolume* bounds;       // [in]
    float minX;                         // [in]
    float maxX;                         // [in]
    float minZ;                         // [in]
    float maxZ;                         // [in]
    float stepX;                        // [in]
    float stepZ;                        // [in]
    float minHeight;                    // [out]
    float maxHeight;                    // [out]
    float* heights;                     // [in][out]
    int width;                          // [in]
    int height;                         // [in]
    int heightIndex;                    // [in]
};

// Globals used by thread
int __processedHeightmapScanLines = 0;
int __totalHeightmapScanlines = 0;
int __failedRayCasts = 0;

// Forward declarations
int generateHeightmapChunk(void* threadData);
bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const Vector3& boxMin, const Vector3& boxMax, float* distance = NULL);
int intersect_triangle(const float orig[3], const float dir[3], const float vert0[3], const float vert1[3], const float vert2[3], float *t, float *u, float *v);
bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const std::vector<Vertex>& vertices, const std::vector<MeshPart*>& parts, Vector3* point);

void Heightmap::generate(const std::vector<std::string>& nodeIds, int width, int height, const char* filename, bool highP)
{
    LOG(1, "Generating heightmap: %s...\n", filename);

    // Initialize state variables
    __processedHeightmapScanLines = 0;
    __totalHeightmapScanlines = 0;
    __failedRayCasts = 0;

    GPBFile* gpbFile = GPBFile::getInstance();

    // Lookup nodes in GPB file and compute a single bounding volume that encapsulates all meshes
    // to be included in the heightmap generation.
    BoundingVolume bounds;
    bounds.min.set(FLT_MAX, FLT_MAX, FLT_MAX);
    bounds.max.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    std::vector<Mesh*> meshes;
    for (unsigned int j = 0, ncount = nodeIds.size(); j < ncount; ++j)
    {
        Node* node = gpbFile->getNode(nodeIds[j].c_str());
        if (node)
        {
            Mesh* mesh = node->getModel() ? node->getModel()->getMesh() : NULL;
            if (mesh)
            {
                // Add this mesh and update our bounding volume
                if (meshes.size() == 0)
                    bounds = mesh->bounds;
                else
                    bounds.merge(mesh->bounds);

                meshes.push_back(mesh);
            }
            else
            {
                LOG(1, "WARNING: Node passed to heightmap argument does not have a mesh: %s\n", nodeIds[j].c_str());
            }
        }
        else
        {
            LOG(1, "WARNING: Failed to locate node for heightmap argument: %s\n", nodeIds[j].c_str());
        }
    }

    if (meshes.size() == 0)
    {
        LOG(1, "WARNING: Skipping generation of heightmap '%s'. No nodes found.\n", filename);
        return;
    }

    // Shoot rays down from a point just above the max Y position of the mesh.
    // Compute ray-triangle intersection tests against the ray and this mesh to 
    // generate heightmap data.
    Vector3 rayOrigin(0, bounds.max.y + 10, 0);
    Vector3 rayDirection(0, -1, 0);

    float minX = bounds.min.x;
    float maxX = bounds.max.x;
    float minZ = bounds.min.z;
    float maxZ = bounds.max.z;
    int size = width * height;
    float* heights = new float[size];
    float minHeight = FLT_MAX;
    float maxHeight = -FLT_MAX;

    __totalHeightmapScanlines = height;

    // Determine # of threads to spawn
    int threadCount = min(THREAD_COUNT, height);

    // Split the work into separate threads to make max use of available cpu cores and speed up computation.
    HeightmapThreadData* threadData = new HeightmapThreadData[threadCount];
    THREAD_HANDLE* threads = new THREAD_HANDLE[threadCount];
    int stepSize = height / threadCount;
    for (int i = 0, remaining = height; i < threadCount; ++i, remaining -= stepSize)
    {
        HeightmapThreadData& data = threadData[i];
        data.rayHeight = rayOrigin.y;
        data.rayDirection = &rayDirection;
        data.bounds = &bounds;
        data.meshes = &meshes;
        data.minX = minX;
        data.maxX = maxX;
        data.minZ = minZ + (stepSize * i);
        data.maxZ = data.minZ + stepSize - 1;
        if (i == threadCount - 1)
            data.maxZ = maxZ;
        data.stepX = (maxX - minX) / width;
        data.stepZ = (maxZ - minZ) / height;
        data.heights = heights;
        data.width = width;
        data.height = remaining > stepSize ? stepSize : remaining;
        data.heightIndex = width * (stepSize * i);

        // Start the processing thread
        if (!createThread(&threads[i], &generateHeightmapChunk, &data))
        {
            LOG(1, "ERROR: Failed to spawn worker thread for generation of heightmap: %s\n", filename);
            return;
        }
    }

    // Wait for all threads to terminate
    waitForThreads(threadCount, threads);

    // Close all thread handles and free memory allocations.
    for (int i = 0; i < threadCount; ++i)
        closeThread(threads[i]);

    // Update min/max height from all completed threads
    for (int i = 0; i < threadCount; ++i)
    {
        if (threadData[i].minHeight < minHeight)
            minHeight = threadData[i].minHeight;
        if (threadData[i].maxHeight > maxHeight)
            maxHeight = threadData[i].maxHeight;
    }

    LOG(1, "\r\tDone.\n");

    if (__failedRayCasts)
    {
        LOG(2, "Warning: %d triangle intersections failed for heightmap: %s\n", __failedRayCasts, filename);

        // Go through and clamp any height values that are set to -FLT_MAX to the min recorded height value
        // (otherwise the range of height values will be far too large).
        for (int i = 0; i < size; ++i)
        {
            if (heights[i] == -FLT_MAX)
                heights[i] = minHeight;
        }
    }
    
    // Normalize the max height value
    maxHeight = maxHeight - minHeight;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        LOG(1, "Error: Failed to open file for writing: %s\n", filename);
        goto error;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        LOG(1, "Error: Write struct creation failed: %s\n", filename);
        goto error;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        LOG(1, "Error: Info struct creation failed: %s\n", filename);
        goto error;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for a single row of image data
    row = (png_bytep)malloc(3 * width * sizeof(png_byte));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Write height value normalized between 0-255 (between min and max height)
            float h = heights[y*width + x];
            float nh = (h - minHeight) / maxHeight;
            int pos = x*3;
            if (highP)
            {
                // high precision packed 24-bit (RGB)
                int bits = (int)(nh * 16777215.0f); // 2^24-1
                
                row[pos+2] = (png_byte)(bits & 0xff);
                bits >>= 8;
                row[pos+1] = (png_byte)(bits & 0xff);
                bits >>= 8;
                row[pos] = (png_byte)(bits & 0xff);
            }
            else
            {
                // standard precision 8-bit (grayscale)
                png_byte b = (png_byte)(nh * 255.0f);
                row[pos] = row[pos+1] = row[pos+2] = b;
            }
        }
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, NULL);
    LOG(1, "Saved heightmap: %s\n", filename);

error:
    if (threadData)
        delete[] threadData;
    if (threads)
        delete[] threads;
    if (heights)
        delete[] heights;
    if (fp)
        fclose(fp);
    if (row)
        free(row);
    if (info_ptr)
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

int generateHeightmapChunk(void* threadData)
{
    HeightmapThreadData* data = (HeightmapThreadData*)threadData;

    Vector3 rayOrigin(0, data->rayHeight, 0);
    const Vector3& rayDirection = *data->rayDirection;
    const std::vector<Mesh*>& meshes = *data->meshes;
    float* heights = data->heights;

    Vector3 intersectionPoint;
    float minHeight = FLT_MAX;
    float maxHeight = -FLT_MAX;
    int index = data->heightIndex;

    int zi = 0;
    for (float z = data->minZ; zi < data->height; z += data->stepZ, ++zi)
    {
        LOG(1, "\r\t%d%%", (int)(((float)__processedHeightmapScanLines / __totalHeightmapScanlines) * 100.0f));

        rayOrigin.z = z;

        int xi = 0;
        for (float x = data->minX; xi < data->width; x += data->stepX, ++xi)
        {
            float h = -FLT_MAX;
            rayOrigin.x = x;

            for (unsigned int i = 0, count = meshes.size(); i < count; ++i)
            {
                // Pick the highest intersecting Y value of all meshes
                Mesh* mesh = meshes[i];

                // Perform a quick ray/bounding box test to quick-out
                if (!intersect(rayOrigin, rayDirection, mesh->bounds.min, mesh->bounds.max))
                    continue;

                // Compute the intersection point of ray with mesh
                if (intersect(rayOrigin, rayDirection, mesh->vertices, mesh->parts, &intersectionPoint))
                {
                    if (intersectionPoint.y > h)
                    {
                        h = intersectionPoint.y;

                        // Update min/max height values
                        if (h < minHeight)
                            minHeight = h;
                        if (h > maxHeight)
                            maxHeight = h;
                    }
                }
            }

            // Update the glboal height array
            heights[index++] = h;

            if (h == -FLT_MAX)
                ++__failedRayCasts;
        }

        ++__processedHeightmapScanLines;
    }

    // Update min/max height for this thread data
    data->minHeight = minHeight;
    data->maxHeight = maxHeight;

    return 0;
}

/////////////////////////////////////////////////////////////
// 
// Fast, Minimum Storage Ray-Triangle Intersection
// 
// Authors: Tomas Möller, Ben Trumbore
// http://jgt.akpeters.com/papers/MollerTrumbore97
//
// Implementation of algorithm from Real-Time Rendering (vol 1), pg. 305.
//
// Adapted slightly for use here.
// 
#ifndef EPSILON
#define EPSILON 0.000001
#endif
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 
int intersect_triangle(const float orig[3], const float dir[3], const float vert0[3], const float vert1[3], const float vert2[3], float *t, float *u, float *v)
{
   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0f / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;

   return 1;
}

// Performs an intersection test between a ray and the given mesh part and stores the result in "point".
bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const std::vector<Vertex>& vertices, const std::vector<MeshPart*>& parts, Vector3* point)
{
    const float* orig = &rayOrigin.x;
    const float* dir = &rayDirection.x;

    float minT = FLT_MAX;

    for (unsigned int i = 0, partCount = parts.size(); i < partCount; ++i)
    {
        MeshPart* part = parts[i];

        for (unsigned int j = 0, indexCount = part->getIndicesCount(); j < indexCount; j += 3)
        {
            const float* v0 = &vertices[part->getIndex( j )].position.x;
            const float* v1 = &vertices[part->getIndex(j+1)].position.x;
            const float* v2 = &vertices[part->getIndex(j+2)].position.x;

            // Perform a quick check (in 2D) to determine if the point is definitely NOT in the triangle
            float xmin, xmax, zmin, zmax;
            xmin = v0[0] < v1[0] ? v0[0] : v1[0]; xmin = xmin < v2[0] ? xmin : v2[0];
            xmax = v0[0] > v1[0] ? v0[0] : v1[0]; xmax = xmax > v2[0] ? xmax : v2[0];
            zmin = v0[2] < v1[2] ? v0[2] : v1[2]; zmin = zmin < v2[2] ? zmin : v2[2];
            zmax = v0[2] > v1[2] ? v0[2] : v1[2]; zmax = zmax > v2[2] ? zmax : v2[2];
            if (orig[0] < xmin || orig[0] > xmax || orig[2] < zmin || orig[2] > zmax)
                continue;

            // Perform a full ray/traingle intersection test in 3D to get the intersection point
            float t, u, v;
            if (intersect_triangle(orig, dir, v0, v1, v2, &t, &u, &v))
            {
                // Found an intersection!
                if (t < minT)
                {
                    minT = t;

                    if (point)
                    {
                        Vector3 rd(rayDirection);
                        rd.scale(t);
                        Vector3::add(rayOrigin, rd, point);
                    }
                }
                //return true;
            }
        }
    }

    return (minT != FLT_MAX);//false;
}

// Ray/Box intersection test.
bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const Vector3& boxMin, const Vector3& boxMax, float* distance)
{
    const Vector3& origin = rayOrigin;
    const Vector3& direction = rayDirection;
    const Vector3& min = boxMin;
    const Vector3& max = boxMax;

    // Intermediate calculation variables.
    float dnear = 0.0f;
    float dfar = 0.0f;
    float tmin = 0.0f;
    float tmax = 0.0f;

    // X direction.
    float div = 1.0f / direction.x;
    if (div >= 0.0f)
    {
        tmin = (min.x - origin.x) * div;
        tmax = (max.x - origin.x) * div;
    }
    else
    {
        tmin = (max.x - origin.x) * div;
        tmax = (min.x - origin.x) * div;
    }
    dnear = tmin;
    dfar = tmax;

    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
    {
        return false;
    }

    // Y direction.
    div = 1.0f / direction.y;
    if (div >= 0.0f)
    {
        tmin = (min.y - origin.y) * div;
        tmax = (max.y - origin.y) * div;
    }
    else
    {
        tmin = (max.y - origin.y) * div;
        tmax = (min.y - origin.y) * div;
    }

    // Update the near and far intersection distances.
    if (tmin > dnear)
    {
        dnear = tmin;
    }
    if (tmax < dfar)
    {
        dfar = tmax;
    }
    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
    {
        return false;
    }

    // Z direction.
    div = 1.0f / direction.z;
    if (div >= 0.0f)
    {
        tmin = (min.z - origin.z) * div;
        tmax = (max.z - origin.z) * div;
    }
    else
    {
        tmin = (max.z - origin.z) * div;
        tmax = (min.z - origin.z) * div;
    }

    // Update the near and far intersection distances.
    if (tmin > dnear)
    {
        dnear = tmin;
    }
    if (tmax < dfar)
    {
        dfar = tmax;
    }

    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
    {
        return false;
    }

    // The ray intersects the box
    if (distance)
        *distance = dnear;

    return true;
}

}
