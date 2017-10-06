#pragma once

#include "Texture.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Texture.
 */
class TextureVK : public Texture
{
public:

	/**
	 * Constructor.
	 */
	TextureVK();

	/**
	 * Constructor.
	 */
	TextureVK(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
			  Format pixelFormat, 
			  Usage usage, 
			  SampleCount sampleCount, 
			  bool hostVisible,
		      VkDevice device, 
		      VkImage image,
			  VkDeviceMemory deviceMemory);
	/**
	 * Destructor.
	 */
	~TextureVK();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;

	VkDevice _device;
	VkImage _image;
	VkDeviceMemory _deviceMemory;
    VkImageView  _imageView;
	VkImageAspectFlags  _imageAspectFlags;
    VkDescriptorImageInfo _textureView;
	void* _hostMemory;
};

}