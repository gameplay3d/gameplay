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
	 * Destructor.
	 */
	~CommandListVK();

	void beginCommands();

	void endCommands();

};

}