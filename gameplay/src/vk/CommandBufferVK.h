#pragma once

#include "CommandBuffer.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 *  Defines a Vulkan impl of CommandBuffer.
 */
class CommandBufferVK : public CommandBuffer
{
public:

	/**
	 * Constructor.
	 */
	CommandBufferVK();

	/**
	 * Constructor.
	 */
	CommandBufferVK(VkDevice device, 
					VkCommandPool commandPool, 
					VkCommandBuffer commandBuffer);

	/**
	 * Destructor.
	 */
	~CommandBufferVK();

	VkDevice _device;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
};

}