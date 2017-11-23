#pragma once

#include "Texture.h"
#include <dxgi1_5.h>
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Texture.
 */
class TextureD3D12 : public Texture
{
public:

	/**
	 * Constructor.
	 */
	TextureD3D12();

	/**
	 * Constructor.
	 */
	TextureD3D12(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
				 Format pixelFormat, 
				 Usage usage, 
			     SampleCount sampleCount,
				 const ClearValue& clearValue,
			     bool hostVisible,
				 bool hostOwned,
		         ID3D12Device* device, 
		         ID3D12Resource* resource);
	/**
	 * Destructor.
	 */
	~TextureD3D12();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;

	ID3D12Device* _device;
	ID3D12Resource* _resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC  _resourceViewDesc;
	void* _hostMemory;
};

}