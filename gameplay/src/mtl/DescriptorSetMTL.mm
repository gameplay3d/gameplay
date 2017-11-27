#include "Base.h"
#include "DescriptorSetMTL.h"

namespace gameplay
{

DescriptorSetMTL::DescriptorSetMTL() :
	DescriptorSet(),
	_device(nullptr)
{
}

DescriptorSetMTL::DescriptorSetMTL(id<MTLDevice> device) :
	DescriptorSet(),
	_device(device)
{
}

DescriptorSetMTL::~DescriptorSetMTL()
{
}

}
