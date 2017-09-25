#pragma once

#include "Buffer.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Direct3D impl of a Buffer.
 */
class BufferDirect3D : public Buffer
{
public:

	/**
	 * Constructor.
	 */
	BufferDirect3D(Type type, 
				   size_t size,
				   bool dynamic,
				   ID3D12Device* device, 
				   ID3D12Resource* buffer);

	/**
	 * Destructor.
	 */
	~BufferDirect3D();

	ID3D12Device* _device;
	ID3D12Resource* _buffer;
	union
	{
		D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;
		D3D12_CONSTANT_BUFFER_VIEW_DESC _constantBufferView;
	};
	
};

}