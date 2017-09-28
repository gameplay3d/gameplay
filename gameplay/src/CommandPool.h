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

protected:

	bool _transient;
};

}