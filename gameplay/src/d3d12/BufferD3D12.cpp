#include "Base.h"
#include "BufferD3D12.h"

namespace gameplay
{


BufferD3D12::BufferD3D12() :
	Buffer(),
	_device(nullptr),
	_buffer(nullptr)
{
}

BufferD3D12::BufferD3D12(Usage usage, size_t size, size_t stride, bool hostVisible, void* hostMemory,
					     ID3D12Device* device, 
					     ID3D12Resource* buffer) : 
	Buffer(usage, size, stride, hostVisible, hostMemory), 
	_device(_device),
	_buffer(buffer)
{
}

BufferD3D12::~BufferD3D12()
{
}

}