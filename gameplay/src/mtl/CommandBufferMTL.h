#pragma once

#include "CommandBuffer.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a CommandBuffer.
 */
class CommandBufferMTL : public CommandBuffer
{
public:

    /**
     * Constructor.
     */
	CommandBufferMTL();

    /**
     * Constructor.
     */
    CommandBufferMTL(id<MTLDevice> device, id<MTLComamndBuffer> commandBuffer);

    /**
	 * Destructor.
     */
    ~FenceMTL();

	id<MTLDevice>  _device;
    id<MTLComamndBuffer> _commandBuffer;
};

}