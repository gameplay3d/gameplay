#pragma once

namespace gameplay
{

/**
 * Defines a command list for processing
 * async gpu operations.
 */
class CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandList();

	/**
	 * Destructor.
	 */
	~CommandList();

	/**
	 * Clears the command list.
	 */
	virtual void clear() = 0;
};

}