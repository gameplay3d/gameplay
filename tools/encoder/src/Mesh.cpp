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
    write((unsigned int)_vertexFormat.size(), file);
    for (std::vector<VertexElement>::iterator i = _vertexFormat.begin(); i != _vertexFormat.end(); ++i)
    {
        i->writeBinary(file);
    }
    // vertices
    writeBinaryVertices(file);
    // parts
    writeBinaryObjects(parts, file);
}

void Mesh::writeBinaryVertices(FILE* file)
{
    if (vertices.size() > 0)
    {
        // Assumes that all vertices are the same size.
        // Write the number of bytes for the vertex data
        const Vertex& vertex = vertices.front();
        write((unsigned int)(vertices.size() * vertex.byteSize()), file); // (vertex count) * (vertex size)

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
        writeZero(file);
    }

    // Write bounds
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
        for (std::vector<VertexElement>::iterator i = _vertexFormat.begin(); i != _vertexFormat.end(); ++i)
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

bool Mesh::hasNormals() const
{
    return !vertices.empty() && vertices[0].hasNormal;
}

bool Mesh::hasVertexColors() const
{
    return !vertices.empty() && vertices[0].hasDiffuse;
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

    LOG(2, "Computing bounds for mesh: %s\n", getId().c_str());

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
