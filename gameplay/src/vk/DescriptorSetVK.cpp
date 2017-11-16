#include "Base.h"
#include "DescriptorSetVK.h"

namespace gameplay
{

DescriptorSetVK::DescriptorSetVK() :
	DescriptorSet(),
	_device(nullptr),
    _layout(nullptr),
    _set(nullptr),
    _pool(nullptr)
{
}

DescriptorSetVK::DescriptorSetVK(const Descriptor* descriptors, 
								 size_t descriptorCount,
							     VkDevice device, 
                                 VkDescriptorSetLayout layout, 
                                 VkDescriptorSet set, 
                                 VkDescriptorPool pool) :
	DescriptorSet(descriptors, descriptorCount),
	_device(device),
    _layout(layout),
    _set(set),
    _pool(pool)
{
}

DescriptorSetVK::~DescriptorSetVK()
{
}

}