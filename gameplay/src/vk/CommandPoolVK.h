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
	 * Destructor.
	 */
	~CommandPoolVK();


	std::shared_ptr<CommandList> createCommandList();
};

}