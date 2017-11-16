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
                    VkDescriptorSetLayout layout, 
                    VkDescriptorSet set, 
                    VkDescriptorPool pool);
    /**
	 * Destructor.
     */
    ~DescriptorSetVK();

	VkDevice _device;
    VkDescriptorSetLayout _layout;
    VkDescriptorSet _set;
    VkDescriptorPool _pool;
};

}