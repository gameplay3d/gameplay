#pragma once

#include "DescriptorSet.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a DesctiptorSet.
 */
class DescriptorSetVK : public DescriptorSet
{
public:

    /**
     * Constructor.
     */
	DescriptorSetVK();

    /**
     * Constructor.
     */
    DescriptorSetVK(const Descriptor* descriptors, 
					size_t descriptorCount,
					VkDevice device, 
					VkDescriptorPool descriptorPool,
                    VkDescriptorSetLayout descriptorSetLayout,
					VkDescriptorSet descriptorSet);
    /**
	 * Destructor.
     */
    ~DescriptorSetVK();

	VkDevice _device;
	VkDescriptorPool _descriptorPool;
	VkDescriptorSetLayout _descriptorSetLayout;
	VkDescriptorSet _descriptorSet;
};

}