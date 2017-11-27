#pragma once

#include "Sampler.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Sampler.
 */
class SamplerMTL : public Sampler
{
public:

    /**
     * Constructor.
     */
	SamplerMTL();

    /**
     * Constructor.
     */
    SamplerMTL(id<MTLDevice> device, id<MTLSamplerState> samplerState);

    /**
	 * Destructor.
     */
    ~SamplerMTL();

	id<MTLDevice>  _device;
    id<MTLSamplerState> _samplerState;
};

}
