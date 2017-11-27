#pragma once

#include "RenderPass.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a RenderPass.
 */
class RenderPassMTL : public RenderPass
{
public:

    /**
     * Constructor.
     */
	RenderPassMTL();

    /**
     * Constructor.
     */
    RenderPassMTL(id<MTLDevice> device, MTLRenderPassDescriptor* renderPassDescriptor);

    /**
	 * Destructor.
     */
    ~RenderPassMTL();

	id<MTLDevice>  _device;
    MTLRenderPassDescriptor* _renderPassDescriptor;
};

}
