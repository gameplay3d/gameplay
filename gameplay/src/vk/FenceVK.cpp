#include "Base.h"
#include "FenceVK.h"

namespace gameplay
{

FenceVK::FenceVK() :
	Fence(),
	_device(nullptr),
    _fence(nullptr)
{
}

FenceVK::FenceVK(VkDevice device, VkFence fence) :
	Fence(),
	_device(device),
    _fence(fence)
{
}

FenceVK::~FenceVK()
{
}

}