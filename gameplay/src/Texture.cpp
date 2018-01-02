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
	_hostVisible(true)
{
}

Texture::Texture(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
				 Format pixelFormat, 
				 Usage usage,
				 SampleCount sampleCount,
				 bool hostVisible,
				 bool hostOwned) :
	_type(type),
	_width(width),
	_height(height),
	_depth(depth),
	_mipLevels(mipLevels),
	_pixelFormat(pixelFormat),
	_usage(usage),
	_sampleCount(sampleCount),
	_hostVisible(hostVisible),
	_hostOwned(hostOwned)
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

bool Texture::isHostOwned() const
{
	return _hostOwned;
}

}