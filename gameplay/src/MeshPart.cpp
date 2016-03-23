#include "Base.h"
#include "MeshPart.h"

namespace gameplay
{

MeshPart::MeshPart() :
    _mesh(NULL), _meshIndex(0), _primitiveType(Mesh::TRIANGLES), _indexCount(0), _indexBuffer(0), _dynamic(false)
{
}

MeshPart::~MeshPart()
{
    if (_indexBuffer)
    {
        glDeleteBuffers(1, &_indexBuffer);
    }
}

MeshPart* MeshPart::create(Mesh* mesh, unsigned int meshIndex, Mesh::PrimitiveType primitiveType,
    Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    // Create a VBO for our index buffer.
    GLuint vbo;
    GL_ASSERT( glGenBuffers(1, &vbo) );
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo) );

    unsigned int indexSize = 0;
    switch (indexFormat)
    {
    case Mesh::INDEX8:
        indexSize = 1;
        break;
    case Mesh::INDEX16:
        indexSize = 2;
        break;
    case Mesh::INDEX32:
        indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", indexFormat);
        glDeleteBuffers(1, &vbo);
        return NULL;
    }

    GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * indexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );

    MeshPart* part = new MeshPart();
    part->_mesh = mesh;
    part->_meshIndex = meshIndex;
    part->_primitiveType = primitiveType;
    part->_indexFormat = indexFormat;
    part->_indexCount = indexCount;
    part->_indexBuffer = vbo;
    part->_dynamic = dynamic;

    return part;
}

unsigned int MeshPart::getMeshIndex() const
{
    return _meshIndex;
}

Mesh::PrimitiveType MeshPart::getPrimitiveType() const
{
    return _primitiveType;
}

unsigned int MeshPart::getIndexCount() const
{
    return _indexCount;
}

Mesh::IndexFormat MeshPart::getIndexFormat() const
{
    return _indexFormat;
}

IndexBufferHandle MeshPart::getIndexBuffer() const
{
    return _indexBuffer;
}

void* MeshPart::mapIndexBuffer()
{
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer) );

    return (void*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool MeshPart::unmapIndexBuffer()
{
    return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void MeshPart::setIndexData(const void* indexData, unsigned int indexStart, unsigned int indexCount)
{
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer) );

    unsigned int indexSize = 0;
    switch (_indexFormat)
    {
    case Mesh::INDEX8:
        indexSize = 1;
        break;
    case Mesh::INDEX16:
        indexSize = 2;
        break;
    case Mesh::INDEX32:
        indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", _indexFormat);
        return;
    }

    if (indexStart == 0 && indexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * _indexCount, indexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (indexCount == 0)
        {
            indexCount = _indexCount - indexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexStart * indexSize, indexCount * indexSize, indexData) );
    }
}

bool MeshPart::isDynamic() const
{
    return _dynamic;
}

}
