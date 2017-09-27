#include "Base.h"
#include "BufferVK.h"

namespace gameplay
{

BufferVK::BufferVK() :
	Buffer(),
	_device(nullptr),
	_buffer(nullptr),
	_hostMemory(nullptr)
{
}

BufferVK::BufferVK(Usage usage, size_t size, size_t stride, bool hostVisible,
				   VkDevice device,
				   VkBuffer buffer) :
	Buffer(usage, size, stride, hostVisible),
	_device(device),
	_buffer(buffer),
	_hostMemory(nullptr)
{
}

BufferVK::~BufferVK()
{
}

void* BufferVK::getHostMemory() const
{
	return _hostMemory;
}

}