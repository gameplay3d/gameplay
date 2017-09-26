#pragma once

#include "Mesh.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Mesh.
 */
class MeshD3D12 : public Mesh
{
public:

	/**
	 * Direct3D impl of a Mesh::Part.
	 */
	class PartD3D12 : public Part
	{
	public:

		/**
		 * Constructor.
		 */
		PartD3D12();

		/**
		 * Constructor.
		 */
		PartD3D12(std::shared_ptr<Mesh> mesh,
			      Graphics::PrimitiveTopology primitiveTopology, 
			      Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic,
				  ID3D12Resource* indexBuffer);
		/**
		 * Destructor.
		 */
		~PartD3D12();

		/**
		 * @see Mesh::Part::mapIndexBuffer
		 */
		void* mapIndexBuffer(size_t rangeStart, size_t rangeSize);

		/**
		 * @see Mesh::Part::setIndexData
		 */
		void setIndexData(void* indexData);

		/**
		 * @see Mesh::Part::unmapIndexBuffer
		 */
		bool unmapIndexBuffer();

		ID3D12Resource* _indexBuffer;
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;
	};

	/**
	 * Constructor.
	 */
	MeshD3D12();

	/**
	 * Constructor.
	 */
	MeshD3D12(const VertexFormat& vertexFormat,
			  size_t vertexCount,
		      bool dynamic,
		      ID3D12Device* device, 
		      ID3D12Resource* vertexBuffer);
	/**
	 * Destructor.
	 */
	~MeshD3D12();

	/**
	 * @see Mesh::mapVertexBuffer
	 */
	void* mapVertexBuffer(size_t rangeStart, size_t rangeSize);

	/**
	 * @see Mesh::setVertexData
	 */
	void setVertexData(void* indexData);

	/**
	 * @see Mesh::unmapVertexBuffer
	 */
	bool unmapVertexBuffer();

	/**
	 * @see Mesh::addPart
	 */
	Mesh::Part* addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic);

	ID3D12Device* _device;
	ID3D12Resource* _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
};

}