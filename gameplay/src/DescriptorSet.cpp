#include "Base.h"
#include "DescriptorSet.h"

namespace gameplay
{

DescriptorSet::Descriptor::Descriptor()
{
}

DescriptorSet::Descriptor::~Descriptor()
{
}

DescriptorSet::DescriptorSet() :
	_descriptorPool(VK_NULL_HANDLE),
	_descriptorSetLayout(VK_NULL_HANDLE),
	_descriptorSet(VK_NULL_HANDLE)
{
}

DescriptorSet::DescriptorSet(const Descriptor* descriptors, size_t descriptorCount)
{
	GP_ASSERT(descriptors);
	GP_ASSERT(descriptorCount < GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);

	_descriptors.resize(descriptorCount);
    for (size_t i = 0; i < descriptorCount; ++i)
    {
        memcpy(&_descriptors[i], &descriptors[i], sizeof(Descriptor));
    }
}

DescriptorSet::~DescriptorSet()
{
}

size_t DescriptorSet::getDescriptorCount() const
{
	return _descriptors.size();
}

DescriptorSet::Descriptor DescriptorSet::getDescriptor(size_t index) const
{
	GP_ASSERT(index < _descriptors.size());
    return _descriptors[index];
}

}