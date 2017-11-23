#include "Base.h"
#include "DescriptorSetVK.h"

namespace gameplay
{

DescriptorSetVK::DescriptorSetVK() :
	DescriptorSet(),
	_device(nullptr),
    _descriptorPool(nullptr),
    _descriptorSet(nullptr),
    _descriptorSetLayout(nullptr)
{
}

DescriptorSetVK::DescriptorSetVK(const Descriptor* descriptors, 
								 size_t descriptorCount,
							     VkDevice device, 
                                 VkDescriptorPool descriptorPool,
								 VkDescriptorSetLayout descriptorSetLayout, 
								 VkDescriptorSet descriptorSet) :
	DescriptorSet(descriptors, descriptorCount),
	_device(device),
    _descriptorPool(descriptorPool),
	_descriptorSetLayout(descriptorSetLayout),
	_descriptorSet(descriptorSet)
    
{
}

DescriptorSetVK::~DescriptorSetVK()
{
}

}