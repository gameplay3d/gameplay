#pragma once

#include "CommandPool.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Defines a Direct3D 12 impl of CommandPool.
 */
class CommandPoolD3D12 : public CommandPool
{
public:

	/**
	 * Constructor.
	 */
	CommandPoolD3D12();

	/**
	 * Constructor.
	 */
	CommandPoolD3D12(ID3D12Device* device, 
					 ID3D12CommandAllocator* commandAllocator);
	/**
	 * Destructor.
	 */
	~CommandPoolD3D12();

    /**
     * @see CommandPool::createCommandBuffer
     */
    std::shared_ptr<CommandBuffer> createCommandBuffer();

    /**
     * @see CommandPool::destroyCommandBuffer
     */
    void destroyCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);

	ID3D12Device* _device;
	ID3D12CommandAllocator* _commandAllocator;
};

}