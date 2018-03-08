#include "Base.h"
#include "Texture.h"
#include "Graphics.h"

namespace gameplay
{

Texture::Texture() :
    _type(TYPE_2D),
    _width(0),
    _height(0),
    _depth(0),
    _mipLevels(1),
    _usage(USAGE_UNDEFINED),
    _sampleCount(Texture::SAMPLE_COUNT_1X),
    _hostVisible(true),
    _hostMemory(nullptr),
    _hostOwned(true),
    _deviceMemory(VK_NULL_HANDLE),
    _image(VK_NULL_HANDLE),
    _imageView(VK_NULL_HANDLE)
{
}

Texture::~Texture()
{
}

Texture::Type Texture::getType() const
{
    return _type;
}

size_t Texture::getWidth() const
{
    return _width;
}

size_t Texture::getHeight() const
{
    return _height;
}

size_t Texture::getDepth() const
{
    return _depth;
}

size_t Texture::getMipLevels() const
{
    return _mipLevels;
}

Texture::Usage Texture::getUsage() const
{
    return _usage;
}

Format Texture::getPixelFormat() const
{
    return _pixelFormat;
}

Texture::SampleCount Texture::getSampleCount() const
{
    return _sampleCount;
}

bool Texture::isHostVisible() const
{
    return _hostVisible;
}

void* Texture::getHostMemory() const
{
    return _hostMemory;
}

bool Texture::isHostOwned() const
{
    return _hostOwned;
}

}