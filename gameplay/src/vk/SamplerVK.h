#pragma once

#include "Sampler.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Sampler.
 */
class SamplerVK : public Sampler
{
public:

    /**
     * Constructor.
     */
	SamplerVK();

    /**
     * Constructor.
     */
    SamplerVK(Filter filterMag,
			  Filter filterMin,
			  Filter filterMip,
			  AddressMode addressModeU,
			  AddressMode addressModeV,
			  AddressMode addressModeW,
			  BorderColor borderColor,
			  bool compareEnabled,
			  CompareFunc compareFunc,
			  bool anisotropyEnabled,
			  float anisotropyMax,
			  float lodMin,
			  float lodMax,
			  float lodMipBias,
		      VkDevice device, 
              VkSampler sampler);

    /**
	 * Destructor.
     */
    ~SamplerVK();

	VkDevice _device;
	VkSampler _sampler;
};

}