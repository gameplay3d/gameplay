#pragma once

#include "Texture.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Texture.
 */
class TextureMTL : public Texture
{
public:

	/**
	 * Constructor.
	 */
    TextureMTL();

	/**
	 * Constructor.
	 */
	TextureMTL(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
               Format pixelFormat,
			   Usage usage, 
			   SampleCount sampleCount, 
               const ClearValue& clearValue,
               bool hostVisible,
               bool hostOwned,
			   id<MTLDevice> device,
			   id<MTLTexture> texture);

	/**
	 * Destructor.
	 */
	~TextureMTL();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;


	id<MTLDevice> _device;
	id<MTLTexture> _texture;
	void* _hostMemory;

};

}
