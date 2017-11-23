#pragma once

#include "CommandPool.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 *  Defines a Vulkan impl of CommandPool.
 */
class CommandPoolVK : public CommandPool
{
public:

	/**
	 * Constructor.
	 */
	CommandPoolVK();

	/**
	 * Constructor.
	 */
	CommandPoolVK(VkDevice device, 
				  VkCommandPool commandPool);

	/**
	 * Destructor.
	 */
	~CommandPoolVK();

   /**
     * @see CommandPool::createCommandBuffer
     */
    std::shared_ptr<CommandBuffer> createCommandBuffer();

    /**
     * @see CommandPool::destroyCommandBuffer
     */
    void destroyCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);

	VkDevice _device;
	VkCommandPool _commandPool;
};

}