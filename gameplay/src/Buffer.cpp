#include "Base.h"
#include "Buffer.h"

namespace gameplay
{

Buffer::Buffer()
{
}

Buffer::Buffer(Usage usage, size_t size, size_t stride, bool hostVisible) :
	_usage(usage), 
	_size(size), 
	_stride(stride), 
	_hostVisible(hostVisible)
{
}

Buffer::~Buffer()
{
}

Buffer::Usage Buffer::getUsage() const
{
	return _usage;
}

size_t Buffer::getSize() const
{
	return size_t();
}

size_t Buffer::getStride() const
{
	return _stride;
}

bool Buffer::isHostVisible() const
{
	return _hostVisible;
}

}