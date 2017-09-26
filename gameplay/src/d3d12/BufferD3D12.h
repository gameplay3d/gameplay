#pragma once

#include "Buffer.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

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
	BufferD3D12(Usage usage, size_t size, size_t stride, bool hostVisible, void* hostMemory,
		        ID3D12Device* device, 
		        ID3D12Resource* buffer);
	/**
	 * Destructor.
	 */
	~BufferD3D12();

	ID3D12Device* _device;
	ID3D12Resource* _buffer;
};

}