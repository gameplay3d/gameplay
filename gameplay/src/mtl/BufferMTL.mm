#include "Base.h"
#include "BufferMTL.h"

namespace gameplay
{


BufferMTL::BufferMTL() :
	Buffer(),
    _device(nullptr),
    _buffer(nullptr),
	_hostMemory(nullptr)
{
}

BufferMTL::BufferMTL(Usage usage, size_t size, size_t stride, bool hostVisible,
                     id<MTLDevice> device,
                     id<MTLBuffer> buffer) : 
	Buffer(usage, size, stride, hostVisible),
    _device(device),
    _buffer(buffer),
	_hostMemory(nullptr)
{
}

BufferMTL::~BufferMTL()
{
}

void* BufferMTL::getHostMemory() const
{
	return _hostMemory;
}

}