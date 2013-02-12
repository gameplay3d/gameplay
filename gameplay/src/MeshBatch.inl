#include "MeshBatch.h"

namespace gameplay
{

Material* MeshBatch::getMaterial() const
{
    return _material;
}

template <class T>
void MeshBatch::add(const T* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
{
    GP_ASSERT(sizeof(T) == _vertexFormat.getVertexSize());
    add(vertices, sizeof(T), vertexCount, indices, indexCount);
}

}
