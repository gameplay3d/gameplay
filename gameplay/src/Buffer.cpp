#include "Base.h"
#include "Buffer.h"

namespace gameplay
{

Buffer::Buffer() :
    _usage(USAGE_VERTEX), 
    _size(0), 
    _stride(0), 
    _hostVisible(true),
	_hostMemory(nullptr),
	_buffer(VK_NULL_HANDLE),
	_deviceMemory(VK_NULL_HANDLE)
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

void* Buffer::getHostMemory() const
{
	return _hostMemory;
}

}