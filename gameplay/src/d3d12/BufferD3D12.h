#pragma once

#include "Buffer.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Buffer.
 */
class BufferD3D12 : public Buffer
{
public:

	/**
	 * Constructor.
	 */
	BufferD3D12();

	/**
	 * Constructor.
	 */
	BufferD3D12(Usage usage, size_t size, size_t stride, bool hostVisible,
		        ID3D12Device* device, 
		        ID3D12Resource* buffer);
	/**
	 * Destructor.
	 */
	~BufferD3D12();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;

	ID3D12Device* _device;
	ID3D12Resource* _buffer;
	void* _hostMemory;
	union
	{
		D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;
		D3D12_CONSTANT_BUFFER_VIEW_DESC _constantBufferView;
	};
};

}