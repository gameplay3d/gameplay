#include "Base.h"
#include "TextureVK.h"

namespace gameplay
{

TextureVK::TextureVK() :
	Texture(),
	_device(nullptr),
	_image(nullptr),
	_deviceMemory(nullptr),
	_hostMemory(nullptr)
{
}

TextureVK::TextureVK(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
					 Format pixelFormat,
					 Usage usage,
					 SampleCount sampleCount,
					 bool hostVisible,
					 bool hostOwned,
					 VkDevice device, 
					 VkImage image,
					 VkDeviceMemory deviceMemory) :
	Texture(type, width, height, depth, mipLevels, pixelFormat, usage, sampleCount, hostOwned, hostVisible), 
	_device(device),
	_image(image),
	_imageView(nullptr),
	_deviceMemory(deviceMemory),
	_hostMemory(nullptr)
{
}

TextureVK::~TextureVK()
{
}

void* TextureVK::getHostMemory() const
{
	return _hostMemory;
}

}