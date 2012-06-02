#include "MeshBatch.h"

namespace gameplay
{

Material* MeshBatch::getMaterial() const
{
    return _material;
}

template <class T>
void MeshBatch::add(T* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount)
{
    GP_ASSERT(vertices);
    GP_ASSERT(sizeof(T) == _vertexFormat.getVertexSize());
    
    unsigned int newVertexCount = _vertexCount + vertexCount;
    unsigned int newIndexCount = _indexCount + indexCount;
    if (_primitiveType == Mesh::TRIANGLE_STRIP && _vertexCount > 0)
        newIndexCount += 2; // need an extra 2 indices for connecting strips with degenerate triangles
    
    // Do we need to grow the batch?
    while (newVertexCount > _vertexCapacity || (_indexed && newIndexCount > _indexCapacity))
    {
        if (_growSize == 0)
            return; // growing disabled, just clip batch
        if (!resize(_capacity + _growSize))
            return; // failed to grow
    }
    
    // Copy vertex data.
    GP_ASSERT(_verticesPtr);
    unsigned int vBytes = vertexCount * _vertexFormat.getVertexSize();
    memcpy(_verticesPtr, vertices, vBytes);
    
    // Copy index data.
    if (_indexed)
    {
        GP_ASSERT(indices);
        GP_ASSERT(_indicesPtr);

        if (_vertexCount == 0)
        {
            // Simply copy values directly into the start of the index array.
            memcpy(_indicesPtr, indices, indexCount * sizeof(unsigned short));
        }
        else
        {
            if (_primitiveType == Mesh::TRIANGLE_STRIP)
            {
                // Create a degenerate triangle to connect separate triangle strips
                // by duplicating the previous and next vertices.
                _indicesPtr[0] = *(_indicesPtr-1);
                _indicesPtr[1] = _vertexCount;
                _indicesPtr += 2;
            }
            
            // Loop through all indices and insert them, with their values offset by
            // 'vertexCount' so that they are relative to the first newly inserted vertex.
            for (unsigned int i = 0; i < indexCount; ++i)
            {
                _indicesPtr[i] = indices[i] + _vertexCount;
            }
        }
        _indicesPtr += indexCount;
        _indexCount = newIndexCount;
    }
    
    _verticesPtr += vBytes;
    _vertexCount = newVertexCount;
}

}
