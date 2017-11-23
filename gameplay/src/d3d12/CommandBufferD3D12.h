#pragma once

#include "CommandBuffer.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Defines a Direct3D 12 impl of CommandBuffer.
 */
class CommandBufferD3D12 : public CommandBuffer
{
public:

	/**
	 * Constructor.
	 */
	CommandBufferD3D12();

	/**
	 * Constructor.
	 */
	CommandBufferD3D12(ID3D12Device* device, 
					   ID3D12CommandAllocator* commandAllocator, 
					   ID3D12GraphicsCommandList* commandList);
	/**
	 * Destructor.
	 */
	~CommandBufferD3D12();

	ID3D12Device* _device;
	ID3D12CommandAllocator* _commandAllocator;
	ID3D12GraphicsCommandList* _commandList;
};

}