#include "Base.h"
#include "BufferMTL.h"

namespace gameplay
{


BufferMTL::BufferMTL() :
	Buffer(),
	_hostMemory(nullptr)
{
}

BufferMTL::BufferMTL(Usage usage, size_t size, size_t stride, bool hostVisible) : 
	Buffer(usage, size, stride, hostVisible), 
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