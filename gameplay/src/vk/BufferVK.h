#pragma once

#include "Buffer.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Buffer.
 */
class BufferVK : public Buffer
{
public:

	/**
	 * Constructor.
	 */
	BufferVK();

	/**
	 * Constructor.
	 */
	BufferVK(Usage usage, size_t size, size_t stride, bool hostVisible, void* hostMemory,
		     VkDevice device, 
		     VkBuffer buffer);
	/**
	 * Destructor.
	 */
	~BufferVK();

	VkDevice _device;
	VkBuffer _buffer;
};

}