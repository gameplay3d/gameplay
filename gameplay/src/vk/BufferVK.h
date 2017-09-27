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
	BufferVK(Usage usage, size_t size, size_t stride, bool hostVisible,
		     VkDevice device, 
		     VkBuffer buffer,
			 VkDeviceMemory deviceMemory);
	/**
	 * Destructor.
	 */
	~BufferVK();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;

	VkDevice _device;
	VkBuffer _buffer;
	VkDeviceMemory _deviceMemory;
	void* _hostMemory;
    VkDescriptorBufferInfo _bufferView;
};

}