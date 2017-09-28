#pragma once

#include "CommandList.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 *  Defines a Vulkan impl of CommandList.
 */
class CommandListVK : public CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandListVK();

	/**
	 * Constructor.
	 */
	CommandListVK(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer);

	/**
	 * Destructor.
	 */
	~CommandListVK();

	/**
	 * @see CommandList::beginCommands
	 */
	void beginCommands();

	/**
	 * @see CommandList::endCommands
	 */
	void endCommands();

	VkDevice _device;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
};

}