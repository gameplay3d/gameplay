#pragma once

#include "Buffer.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Buffer.
 */
class BufferVulkan : public Buffer
{
public:

	/**
	 * Constructor.
	 */
	BufferVulkan(Type type,
			     size_t size,
				 bool dynamic,
				 VkDevice device, 
				 VkBuffer buffer);

	/**
	 * Destructor.
	 */
	~BufferVulkan();

private:

	VkDevice _device;
	VkBuffer _buffer;
};

}