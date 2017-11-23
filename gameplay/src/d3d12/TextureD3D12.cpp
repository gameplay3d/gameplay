#include "Base.h"
#include "TextureD3D12.h"

namespace gameplay
{

TextureD3D12::TextureD3D12() :
    Texture(),
    _device(nullptr),
    _resource(nullptr),
    _hostMemory(nullptr)
{
}

TextureD3D12::TextureD3D12(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
						   Format pixelFormat,
					       Usage usage, 
						   SampleCount sampleCount,
						   const ClearValue& clearValue,
						   bool hostVisible,
						   bool hostOwned,
						   ID3D12Device* device,
						   ID3D12Resource* resource) : 
    Texture(type, width, height, depth, mipLevels, pixelFormat, usage, sampleCount, clearValue, hostVisible, hostOwned), 
    _device(device),
    _resource(resource),
	_hostMemory(nullptr)
{
}

TextureD3D12::~TextureD3D12()
{
}

void* TextureD3D12::getHostMemory() const
{
	return _hostMemory;
}

}