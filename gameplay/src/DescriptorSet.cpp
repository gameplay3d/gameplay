#include "Base.h"
#include "DescriptorSet.h"

namespace gameplay
{

DescriptorSet::Descriptor::Descriptor()
{
}

DescriptorSet::Descriptor::Descriptor(const Descriptor& copy)
{
}

DescriptorSet::Descriptor::~Descriptor()
{
}

DescriptorSet::DescriptorSet()
{
}

DescriptorSet::DescriptorSet(const Descriptor* descriptors, size_t descriptorCount)
{
	GP_ASSERT(descriptors);
	GP_ASSERT(descriptorCount < GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);

    for (size_t i = 0; i < descriptorCount; ++i)
    {
        Descriptor descriptor;
        memcpy(&descriptor, &descriptors[i], sizeof(Descriptor));
        _descriptors.push_back(descriptor);
    }
}

DescriptorSet::~DescriptorSet()
{
}

size_t DescriptorSet::getDescriptorCount() const
{
	return _descriptors.size();
}

const DescriptorSet::Descriptor& DescriptorSet::getDescriptor(size_t index) const
{
	GP_ASSERT(index < _descriptors.size());
    return _descriptors[index];
}

}