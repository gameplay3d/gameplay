#include "Base.h"
#include "TextureD3D12.h"

namespace gameplay
{

TextureD3D12::TextureD3D12() :
    Texture(),
    _device(nullptr),
    _texture(nullptr),
    _hostMemory(nullptr)
{
}

TextureD3D12::TextureD3D12(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
						   Format pixelFormat,
					       Usage usage, 
						   SampleCount sampleCount, 
						   bool hostVisible,
						   ID3D12Device* device,
						   ID3D12Resource* texture,
						   D3D12_SHADER_RESOURCE_VIEW_DESC textureView) : 
    Texture(type, width, height, depth, mipLevels, pixelFormat, usage, sampleCount, hostVisible), 
    _device(device),
    _texture(texture),
    _textureView(textureView),
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