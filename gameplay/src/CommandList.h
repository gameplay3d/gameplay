#pragma once

namespace gameplay
{

/**
 * Defines a command list for creating and storing async gpu operations.
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

	virtual void beginCommands() = 0;

	virtual void endCommands() = 0;

};

}