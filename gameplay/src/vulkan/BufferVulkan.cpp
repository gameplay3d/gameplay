#include "Base.h"
#include "BufferVulkan.h"

namespace gameplay
{

BufferVulkan::BufferVulkan(Type type,
						   size_t size,
						   bool dynamic,
						   VkDevice device, 
						   VkBuffer buffer) : 
	Buffer(type, size, dynamic),
	_device(device), 
	_buffer(buffer)
{
}

BufferVulkan::~BufferVulkan()
{
}

}