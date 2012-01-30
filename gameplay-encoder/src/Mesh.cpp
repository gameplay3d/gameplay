#include "Base.h"
#include "Mesh.h"
#include "Model.h"

namespace gameplay
{

Mesh::Mesh(void) : model(NULL)
{
}

Mesh::~Mesh(void)
{
}

unsigned int Mesh::getTypeId(void) const
{
    return MESH_ID;
}

const char* Mesh::getElementName(void) const
{
    return "Mesh";
}

void Mesh::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    // vertex formats
    write(_vertexFormat.size(), file);
    for (std::vector<VertexElement>::iterator i = _vertexFormat.begin(); i != _vertexFormat.end(); i++)
    {
        i->writeBinary(file);
    }
    // vertices
    writeBinaryVertices(file);
    // parts
    writeBinaryObjects(parts, file);
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
#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int
intersect_triangle(const float orig[3], const float dir[3],
                   const float vert0[3], const float vert1[3], const float vert2[3],
                   float *t, float *u, float *v)
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

// Performs an intersection test between a ray and the given mesh part
// and stores the result in "point".
bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const std::vector<Vertex>& vertices, const std::vector<MeshPart*>& parts, Vector3* point)
{
    const float* orig = &rayOrigin.x;
    const float* dir = &rayDirection.x;

    for (unsigned int i = 0, partCount = parts.size(); i < partCount; ++i)
    {
        MeshPart* part = parts[i];

        for (unsigned int j = 0, indexCount = part->getIndicesCount(); j < indexCount; j += 3)
        {
            const float* v0 = &vertices[part->getIndex( j )].position.x;
            const float* v1 = &vertices[part->getIndex(j+1)].position.x;
            const float* v2 = &vertices[part->getIndex(j+2)].position.x;

            float t, u, v;
            if (intersect_triangle(orig, dir, v0, v1, v2, &t, &u, &v))
            {
                // Found an intersection!
                if (point)
                {
                    Vector3 rd(rayDirection);
                    rd.scale(t);
                    Vector3::add(rayOrigin, rd, point);
                }
                return true;
            }
        }
    }

    return false;
}

void Mesh::generateHeightmap(const char* filename)
{
    // Shoot rays down from a point just above the max Y position of the mesh.
    // Compute ray-triangle intersection tests against the ray and this mesh to 
    // generate heightmap data.
    Vector3 rayOrigin(0, bounds.max.y + 10, 0);
    Vector3 rayDirection(0, -1, 0);
    Vector3 intersectionPoint;
    int minX = (int)ceil(bounds.min.x);
    int maxX = (int)floor(bounds.max.x);
    int minZ = (int)ceil(bounds.min.z);
    int maxZ = (int)floor(bounds.max.z);
    int width = maxX - minX + 1;
    int height = maxZ - minZ + 1;
    float* heights = new float[width * height];
    int index = 0;
    float minHeight = FLT_MAX;
    float maxHeight = -FLT_MAX;
    for (int z = minZ; z <= maxZ; z++)
    {
        rayOrigin.z = (float)z;
        for (int x = minX; x <= maxX; x++)
        {
            float h;
            rayOrigin.x = (float)x;
            if (intersect(rayOrigin, rayDirection, vertices, parts, &intersectionPoint))
            {
                h = intersectionPoint.y;
            }
            else
            {
                h = 0;
                fprintf(stderr, "Warning: Heightmap triangle intersection failed for (%d, %d).\n", x, z);
            }
            if (h < minHeight)
                minHeight = h;
            if (h > maxHeight)
                maxHeight = h;
            heights[index++] = h;
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
        fprintf(stderr, "Error: Failed to open file for writing: %s\n", filename);
        goto error;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fprintf(stderr, "Error: Write struct creation failed: %s\n", filename);
        goto error;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fprintf(stderr, "Error: Info struct creation failed: %s\n", filename);
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
            png_byte b = (png_byte)(nh * 255.0f);
            
            int pos = x*3;
            row[pos] = row[pos+1] = row[pos+2] = b;
        }
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, NULL);
    DEBUGPRINT_VARG("> Saved heightmap: %s\n", filename);

error:
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

void Mesh::writeBinaryVertices(FILE* file)
{
    if (vertices.size() > 0)
    {
        // Assumes that all vertices are the same size.
        // Write the number of bytes for the vertex data
        const Vertex& vertex = vertices.front();
        write(vertices.size() * vertex.byteSize(), file); // (vertex count) * (vertex size)

        // for each vertex
        for (std::vector<Vertex>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
        {
            // Write this vertex
            i->writeBinary(file);
        }
    }
    else
    {
        // No vertex data
        write((unsigned int)0, file);
    }

    // Write bounds
    computeBounds();
    write(&bounds.min.x, 3, file);
    write(&bounds.max.x, 3, file);
    write(&bounds.center.x, 3, file);
    write(bounds.radius, file);
}

void Mesh::writeText(FILE* file)
{
    fprintElementStart(file);

    // for each VertexFormat
    if (vertices.size() > 0 )
    {
        for (std::vector<VertexElement>::iterator i = _vertexFormat.begin(); i != _vertexFormat.end(); i++)
        {
            i->writeText(file);
        }
    }

    // for each Vertex
    fprintf(file, "<vertices count=\"%lu\">\n", vertices.size());
    for (std::vector<Vertex>::iterator i = vertices.begin(); i != vertices.end(); ++i)
    {
        i->writeText(file);
    }
    fprintf(file, "</vertices>\n");

    // write bounds
    computeBounds();
    fprintf(file, "<bounds>\n");
    fprintf(file, "<min>\n");
    writeVectorText(bounds.min, file);
    fprintf(file, "</min>\n");
    fprintf(file, "<max>\n");
    writeVectorText(bounds.max, file);
    fprintf(file, "</max>\n");
    fprintf(file, "<center>\n");
    writeVectorText(bounds.center, file);
    fprintf(file, "</center>\n");
    fprintf(file, "<radius>%f</radius>\n", bounds.radius);
    fprintf(file, "</bounds>\n");

    // for each MeshPart
    for (std::vector<MeshPart*>::iterator i = parts.begin(); i != parts.end(); ++i)
    {
        (*i)->writeText(file);
    }

    fprintElementEnd(file);
}

void Mesh::addMeshPart(MeshPart* part)
{
    parts.push_back(part);
}

void Mesh::addMeshPart(Vertex* vertex)
{
    vertices.push_back(*vertex);
}

void Mesh::addVetexAttribute(unsigned int usage, unsigned int count)
{
    _vertexFormat.push_back(VertexElement(usage, count));
}

size_t Mesh::getVertexCount() const
{
    return vertices.size();
}

const Vertex& Mesh::getVertex(unsigned int index) const
{
    return vertices[index];
}

size_t Mesh::getVertexElementCount() const
{
    return _vertexFormat.size();
}

const VertexElement& Mesh::getVertexElement(unsigned int index) const
{
    return _vertexFormat[index];
}

bool Mesh::contains(const Vertex& vertex) const
{
    return vertexLookupTable.count(vertex) > 0;
}

unsigned int Mesh::addVertex(const Vertex& vertex)
{
    unsigned int index = getVertexCount();
    vertices.push_back(vertex);
    vertexLookupTable[vertex] = index;
    return index;
}

unsigned int Mesh::getVertexIndex(const Vertex& vertex)
{
    std::map<Vertex,unsigned int>::iterator it;
    it = vertexLookupTable.find(vertex);
    return it->second;
}

void Mesh::computeBounds()
{
    // If we have a Model with a MeshSkin associated with it,
    // compute the bounds from the skin - otherwise compute
    // it from the local mesh data.
    if (model && model->getSkin())
    {
        model->getSkin()->computeBounds();
        return;
    }

    bounds.min.x = bounds.min.y = bounds.min.z = FLT_MAX;
    bounds.max.x = bounds.max.y = bounds.max.z = -FLT_MAX;
    bounds.center.x = bounds.center.y = bounds.center.z = 0.0f;
    bounds.radius = 0.0f;

    for (std::vector<Vertex>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
    {
        // Update min/max for this vertex
        if (i->position.x < bounds.min.x)
            bounds.min.x = i->position.x;
        if (i->position.y < bounds.min.y)
            bounds.min.y = i->position.y;
        if (i->position.z < bounds.min.z)
            bounds.min.z = i->position.z;
        if (i->position.x > bounds.max.x)
            bounds.max.x = i->position.x;
        if (i->position.y > bounds.max.y)
            bounds.max.y = i->position.y;
        if (i->position.z > bounds.max.z)
            bounds.max.z = i->position.z;
    }

    // Compute center point
    Vector3::add(bounds.min, bounds.max, &bounds.center);
    bounds.center.scale(0.5f);

    // Compute radius by looping through all points again and finding the max
    // distance between the center point and each vertex position
    for (std::vector<Vertex>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
    {
        float d = bounds.center.distanceSquared(i->position);
        if (d > bounds.radius)
        {
            bounds.radius = d;
        }
    }

    // Convert squared distance to distance for radius
    bounds.radius = sqrt(bounds.radius);
}

}
