#include "Base.h"
#include "Mesh.h"

namespace gameplay
{

Mesh::Part::Part() : 
	_mesh(nullptr),
	_primitiveTopology(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLES),
	_indexFormat(Graphics::INDEX_FORMAT_UNSIGNED_INT),
	_indexCount(0),
	_dynamic(false)
{
}

Mesh::Part::Part(std::shared_ptr<Mesh> mesh,
			 Graphics::PrimitiveTopology primitiveTopology, 
			 Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic) : 
	_mesh(mesh),
	_primitiveTopology(primitiveTopology),
	_indexFormat(indexFormat),
	_indexCount(indexCount),
	_dynamic(dynamic)
{
}

Mesh::Part::~Part()
{
}

std::shared_ptr<Mesh> Mesh::Part::getMesh() const
{
	return _mesh;
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

Mesh::Mesh() :
	_primitiveTopology(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLES),
	_vertexCount(0),
	_dynamic(false)
{
}

Mesh::Mesh(const VertexFormat& vertexFormat, size_t vertexCount, bool dynamic) :
	_primitiveTopology(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLES),
	_vertexFormat(vertexFormat),
	_vertexCount(vertexCount),
	_dynamic(dynamic)
{
}

Mesh::~Mesh()
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

size_t Mesh::getPartCount() const
{
	return _parts.size();
}

Mesh::Part* Mesh::getPart(size_t index)
{
	return _parts[index];
}

}