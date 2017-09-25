#include "Base.h"
#include "Mesh.h"

namespace gameplay
{

Mesh::Mesh() :
	_primitiveTopology(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLES),
	_vertexCount(0),
	_dynamic(false),
	_vertexBuffer(nullptr)
{
}

Mesh::~Mesh()
{
}

Mesh::Part::Part() : 
	_primitiveTopology(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLES),
	_indexFormat(Graphics::INDEX_FORMAT_UNSIGNED_INT),
	_indexCount(0),
	_dynamic(false),
	_indexBuffer(nullptr)
{
}

Mesh::Part::~Part()
{
}

Graphics::PrimitiveTopology Mesh::Part::getPrimitiveTopology() const
{
	return _primitiveTopology;
}

Graphics::IndexFormat Mesh::Part::getIndexFormat() const
{
	return _indexFormat;
}

size_t Mesh::Part::getIndexCount() const
{
	return _indexCount;
}

bool Mesh::Part::isDynamic() const
{
	return _dynamic;
}

void Mesh::Part::setIndexData(const void* indexData, size_t indexStart, size_t indexCount)
{
}

const VertexFormat& Mesh::getVertexFormat() const
{
	return _vertexFormat;
}

size_t Mesh::getVertexCount() const
{
	return _vertexCount;
}

size_t Mesh::getVertexStride() const
{
	return _vertexFormat.getStride();
}

bool Mesh::isDynamic() const
{
	return _dynamic;
}

Graphics::PrimitiveTopology Mesh::getPrimitiveTopology() const
{
	return _primitiveTopology;
}

void Mesh::setPrimitiveTopology(Graphics::PrimitiveTopology primitiveTopology)
{
	_primitiveTopology = primitiveTopology;
}

void Mesh::setVertexData(const void* vertexData, size_t vertexStart, size_t vertexCount)
{
}

Mesh::Part* Mesh::addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic)
{
	return nullptr;
}

size_t Mesh::getPartCount() const
{
	return _parts.size();
}

Mesh::Part* Mesh::getPart(size_t index)
{
	return _parts[index];
}

}