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
			  ClearValue clearValue,
			  bool hostVisible,
			  bool hostOwned,
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
	VkImageView  _imageView;
	VkDeviceMemory _deviceMemory;
	void* _hostMemory;
	VkImageAspectFlags  _imageAspectFlags;
    VkDescriptorImageInfo _imageViewInfo;
};

}