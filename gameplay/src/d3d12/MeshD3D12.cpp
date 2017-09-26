#include "Base.h"
#include "MeshD3D12.h"

namespace gameplay
{

MeshD3D12::PartD3D12::PartD3D12() : 
	Part(),
	_indexBuffer(nullptr)
{
}

MeshD3D12::PartD3D12::PartD3D12(std::shared_ptr<Mesh> mesh,
								Graphics::PrimitiveTopology primitiveTopology, 
								Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic,
								ID3D12Resource* indexBuffer) :
	Part(mesh, primitiveTopology, indexFormat, indexCount, dynamic),
	_indexBuffer(indexBuffer)
{
}

MeshD3D12::PartD3D12::~PartD3D12()
{
}

void* MeshD3D12::PartD3D12::mapIndexBuffer(size_t rangeStart, size_t rangeSize)
{
	return nullptr;
}

void MeshD3D12::PartD3D12::setIndexData(void* data)
{
}

bool MeshD3D12::PartD3D12::unmapIndexBuffer()
{
	return false;
}

MeshD3D12::MeshD3D12() :
	Mesh(),
	_device(nullptr),
	_vertexBuffer(nullptr)
{
}

MeshD3D12::MeshD3D12(const VertexFormat& vertexFormat, size_t vertexCount, bool dynamic,
					 ID3D12Device* device, 
					 ID3D12Resource* vertexBuffer) : 
	Mesh(vertexFormat, vertexCount, dynamic), 
	_device(_device),
	_vertexBuffer(vertexBuffer)
{
}

MeshD3D12::~MeshD3D12()
{
}

void* MeshD3D12::mapVertexBuffer(size_t rangeStart, size_t rangeSize)
{
	return nullptr;
}

void MeshD3D12::setVertexData(void* data)
{
}

bool MeshD3D12::unmapVertexBuffer()
{
	return false;
}

Mesh::Part* MeshD3D12::addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic)
{
	return nullptr;
}

}