#pragma once

#include "CommandList.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 *  Defines a Vulkan impl of CommandList.
 */
class CommandListVulkan : public CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandListVulkan();

	/**
	 * Destructor.
	 */
	~CommandListVulkan();

	/**
	 * @see CommandList::clear
	 */
	void clear();
};

}