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
	CommandPoolVK(VkDevice device, VkQueue queue, VkCommandPool pool);

	/**
	 * Destructor.
	 */
	~CommandPoolVK();

	/**
	 * @see CommandPool::createCommandList
	 */
	std::shared_ptr<CommandList> createCommandList(bool secondary = false);

	/**
	 * @see CommandPool::destroyCommandList
	 */
	void destroyCommandList(std::shared_ptr<CommandList> commandList);

	VkDevice _device;
	VkQueue _queue;
	VkCommandPool _commandPool;
};

}