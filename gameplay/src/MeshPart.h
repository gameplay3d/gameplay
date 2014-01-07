#ifndef MESHPART_H_
#define MESHPART_H_

#include "Mesh.h"

namespace gameplay
{

/**
 * Defines a part of a mesh describing the way the
 * mesh's vertices are connected together.
 */
class MeshPart
{
    friend class Mesh;
    friend class Model;

public:

    /**
     * Destructor.
     */
    ~MeshPart();

    /**
     * Gets the part index in mesh.
     *
     * @return The part offset index.
     */
    unsigned int getMeshIndex() const;

    /**
     * Gets the type of primitive to define how the indices are connected.
     *
     * @return The type of primitive.
     */
    Mesh::PrimitiveType getPrimitiveType() const;

    /**
     * Gets the number of indices in the part.
     *
     * @return The number of indices in the part.
     */
    unsigned int getIndexCount() const;

    /**
     * Returns the format of the part indices.
     *
     * @return The part index format.
     */
    Mesh::IndexFormat getIndexFormat() const;

    /**
     * Returns a handle to the index buffer for the mesh part.
     *
     * @return The index buffer object handle.
     */
    IndexBufferHandle getIndexBuffer() const;

    /**
     * Determines if the indices are dynamic.
     *
     * @return true if the part is dynamic; false otherwise.
     */
    bool isDynamic() const;

    /**
     * Sets the specified index data into the mapped index buffer.
     *
     * @param indexData The index data to be set.
     * @param indexStart The index to start from.
     * @param indexCount The number of indices to be set.
     * @script{ignore}
     */
    void setIndexData(const void* indexData, unsigned int indexStart, unsigned int indexCount);

private:

    /**
     * Constructor.
     */
    MeshPart();

    /**
     * Constructor.
     */
    MeshPart(const MeshPart& copy);

    /**
     * Creates a mesh part for the specified mesh.
     *
     * @param mesh The mesh that this is part of.
     * @param meshIndex The index of the part within the mesh.
     * @param primitiveType The primitive type.
     * @param indexFormat The index format.
     * @param indexCount The number of indices.
     * @param dynamic true if the part if dynamic; false otherwise.
     */
    static MeshPart* create(Mesh* mesh, unsigned int meshIndex, Mesh::PrimitiveType primitiveType, Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic = false);

    Mesh* _mesh;
    unsigned int _meshIndex;
    Mesh::PrimitiveType _primitiveType;
    Mesh::IndexFormat _indexFormat;
    unsigned int _indexCount;
    IndexBufferHandle _indexBuffer;
    bool _dynamic;
};

}

#endif
