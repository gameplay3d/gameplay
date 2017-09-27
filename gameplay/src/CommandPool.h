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


	virtual std::shared_ptr<CommandList> createCommandList() = 0;

};

}