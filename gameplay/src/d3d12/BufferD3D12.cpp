#include "Base.h"
#include "BufferD3D12.h"

namespace gameplay
{


BufferD3D12::BufferD3D12() :
	Buffer(),
	_device(nullptr),
	_buffer(nullptr),
	_hostMemory(nullptr)
{
}

BufferD3D12::BufferD3D12(Usage usage, size_t size, size_t stride, bool hostVisible,
					     ID3D12Device* device, 
					     ID3D12Resource* buffer) : 
	Buffer(usage, size, stride, hostVisible), 
	_device(_device),
	_buffer(buffer),
	_hostMemory(nullptr)
{
}

BufferD3D12::~BufferD3D12()
{
}

void* BufferD3D12::getHostMemory() const
{
	return _hostMemory;
}

}