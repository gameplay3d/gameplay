#pragma once

#include "Fence.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Fence.
 */
class FenceMTL : public Fence
{
public:

    /**
     * Constructor.
     */
	FenceMTL();

    /**
     * Constructor.
     */
    FenceMTL(id<MTLDevice> device, id<MTLFence> fence);

    /**
	 * Destructor.
     */
    ~FenceMTL();

	id<MTLDevice>  _device;
    id<MTLFence> _fence;
};

}
