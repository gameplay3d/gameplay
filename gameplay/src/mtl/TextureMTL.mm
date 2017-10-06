#include "Base.h"
#include "TextureMTL.h"

namespace gameplay
{


TextureMTL::TextureMTL() :
    Texture(),
    _device(nullptr),
    _texture(nullptr),
    _hostMemory(nullptr)
{
}

TextureMTL::TextureMTL(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
                       Format pixelFormat,
                       Usage usage, 
                       SampleCount sampleCount, 
                       bool hostVisible,
                       id<MTLDevice> device,
                       id<MTLTexture> texture) : 
    Texture(type, width, height, depth, mipLevels, usage, pixelFormat, sampleCount, hostVisible), 
    _device(device),
    _texture(texture),
    _hostMemory(nullptr)
{
}

TextureMTL::~TextureMTL()
{
}

void* TextureMTL::getHostMemory() const
{
    return _hostMemory;
}

}