#pragma once

#include "CommandList.h"

namespace gameplay
{

/**
 * Defines a command pool for creating and maintaining a 
 * pool of async gpu operations.
 */
class CommandPool
{
public:	

	/**
	 * Constructor.
	 */
	CommandPool();

	/**
	 * Destructor.
	 */
	~CommandPool();

	/**
	 * Determines if this command pool is for creating transient (short-lived) command lists.
	 *
	 * @return true if this command pool is for creating transient (short-lived) command lists, false if otherise.
	 */
	bool isTransient() const;

	/**
	 * Creates a command list that is used for encoding command that are submitted to the gpu.
	 *
	 * @param transient true if this command pool is for creating transient (short-lived) command lists, false if otherise.
	 * @return The created commandlist.
	 */
	virtual std::shared_ptr<CommandList> createCommandList(bool secondary = false) = 0;

	/**
	 * Destroys a command list.
	 *
	 * @param commandList The list to be destroyed.
	 */
	virtual void destroyCommandList(std::shared_ptr<CommandList> commandList) = 0;

protected:

	bool _transient;
};

}