#pragma once

#include "CommandList.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Defines a Metal impl of CommandList.
 */
class CommandListMTL : public CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandListMTL();

	/**
	 * Destructor.
	 */
	~CommandListMTL();

	/**
	 * @see CommandList::beginCommands
	 */
	void beginCommands();

	/**
	 * @see CommandList::endCommands
	 */
	void endCommands();

};

}