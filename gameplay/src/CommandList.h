#pragma once

namespace gameplay
{

/**
 * Defines a command list for recording and storing gpu commands.
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
	 * Begins recording command into the command list.
	 */
	virtual void beginCommands() = 0;

	/**
	 * Ends recording command into the command list.
	 */
	virtual void endCommands() = 0;
};

}