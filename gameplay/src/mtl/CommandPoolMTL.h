#pragma once

#include "CommandPool.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Defines a Metal impl of CommandPool.
 */
class CommandPoolMTL : public CommandPool
{
public:

	/**
	 * Constructor.
	 */
	CommandPoolMTL();

	/**
	 * Destructor.
	 */
	~CommandPoolMTL();
};

}