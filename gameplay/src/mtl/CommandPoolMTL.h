#pragma once

#include "CommandPool.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a CommandPool.
 */
class CommandPoolrMTL : public CommandPool
{
public:

    /**
     * Constructor.
     */
	CommandPoolrMTL();

    /**
     * Constructor.
     */
    CommandPoolrMTL(id<MTLDevice> device);

    /**
	 * Destructor.
     */
    ~FenceMTL();

	id<MTLDevice>  _device;
};

}