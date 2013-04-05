#ifndef MESH_H_
#define MESH_H_

#include "Base.h"
#include "Object.h"
#include "MeshPart.h"
#include "VertexElement.h"
#include "BoundingVolume.h"

namespace gameplay
{

class Model;

class Mesh : public Object
{
    friend class Model;

public:

    /**
     * Constructor.
     */
    Mesh(void);

    /**
     * Destructor.
     */
    virtual ~Mesh(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;

    virtual void writeBinary(FILE* file);
    void writeBinaryVertices(FILE* file);

    virtual void writeText(FILE* file);
    void writeText(FILE* file, const Vertex& vertex);
    void writeText(FILE* file, const Vector3& v);

    void addMeshPart(MeshPart* part);
    void addMeshPart(Vertex* vertex);
    void addVetexAttribute(unsigned int usage, unsigned int count);

    size_t getVertexCount() const;
    const Vertex& getVertex(unsigned int index) const;

    size_t getVertexElementCount() const;
    const VertexElement& getVertexElement(unsigned int index) const;

    /**
     * Returns true if this MeshPart contains the given Vertex.
     */
    bool contains(const Vertex& vertex) const;

    /**
     * Adds a vertex to this MeshPart and returns the index.
     */
    unsigned int addVertex(const Vertex& vertex);

    unsigned int getVertexIndex(const Vertex& vertex);

    bool hasNormals() const;
    bool hasVertexColors() const;

    void computeBounds();

    Model* model;
    std::vector<Vertex> vertices;
    std::vector<MeshPart*> parts;
    BoundingVolume bounds;
    std::map<Vertex, unsigned int> vertexLookupTable;

private:
    std::vector<VertexElement> _vertexFormat;

};

}

#endif
