#pragma once

#include "DescriptorSet.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a DescriptorSet.
 */
class DescriptorSetMTL : public DescriptorSet
{
public:

    /**
     * Constructor.
     */
	DescriptorSetMTL();

    /**
     * Constructor.
     */
    DescriptorSetMTL(id<MTLDevice> device);

    /**
	 * Destructor.
     */
    ~DescriptorSetMTL();

	id<MTLDevice>  _device;
};

}
