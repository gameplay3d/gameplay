#include "Base.h"
#include "MeshVK.h"

namespace gameplay
{

MeshVK::PartVK::PartVK() :
	Part(),
	_indexBuffer(nullptr)
{
}

MeshVK::PartVK::PartVK(std::shared_ptr<Mesh> mesh,
					   Graphics::PrimitiveTopology primitiveTopology, 
					   Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic, 
					   VkBuffer indexBuffer) :
	Part(mesh, primitiveTopology, indexFormat, indexCount, dynamic),
	_indexBuffer(indexBuffer)

{
}

MeshVK::PartVK::~PartVK()
{
}

void* MeshVK::PartVK::mapIndexBuffer(size_t rangeStart, size_t rangeSize)
{
	return nullptr;
}

void MeshVK::PartVK::setIndexData(void* indexData)
{
}

bool MeshVK::PartVK::unmapIndexBuffer()
{
	return false;
}

MeshVK::MeshVK() :
	Mesh(),
	_device(nullptr),
	_vertexBuffer(nullptr)
{
}

MeshVK::MeshVK(const VertexFormat& vertexFormat, size_t vertexCount, bool dynamic,
			   VkDevice device, 
			   VkBuffer vertexBuffer) : 
	Mesh(vertexFormat, vertexCount, dynamic),
	_device(device), 
	_vertexBuffer(vertexBuffer)
{
}

MeshVK::~MeshVK()
{
}

void* MeshVK::mapVertexBuffer(size_t rangeStart, size_t rangeSize)
{
	return nullptr;
}

void MeshVK::setVertexData(void* indexData)
{
}

bool MeshVK::unmapVertexBuffer()
{
	return false;
}

Mesh::Part* MeshVK::addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic)
{
	return nullptr;
}

}