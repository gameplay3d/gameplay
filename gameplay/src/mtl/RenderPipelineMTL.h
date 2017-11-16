#pragma once

#include "RenderPipeline.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a RenderPipeline.
 */
class RenderPipelineMTL : public RenderPipeline
{
public:

    /**
     * Constructor.
     */
	RenderPipelineMTL();

    /**
     * Constructor.
     */
    RenderPipelineMTL(id<MTLDevice> device, id<MTLRenderPipelineState> pipelineState);

    /**
	 * Destructor.
     */
    ~PipelineMTL();

	id<MTLDevice>  _device;
    id<MTLRenderPipelineState> _pipelineState;
};

}