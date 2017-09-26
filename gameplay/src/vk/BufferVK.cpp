#include "Base.h"
#include "BufferVK.h"

namespace gameplay
{

BufferVK::BufferVK() :
	Buffer(),
	_device(nullptr),
	_buffer(nullptr)
{
}

BufferVK::BufferVK(Usage usage, size_t size, size_t stride, bool hostVisible, void* hostMemory,
				   VkDevice device,
				   VkBuffer buffer) :
	Buffer(usage, size, stride, hostVisible, hostMemory),
	_device(device),
	_buffer(buffer)
{
}

BufferVK::~BufferVK()
{
}

}