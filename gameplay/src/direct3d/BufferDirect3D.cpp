#include "Base.h"
#include "BufferDirect3D.h"

namespace gameplay
{

BufferDirect3D::BufferDirect3D(Type type, 
							   size_t size,
							   bool dynamic,
							   ID3D12Device* _device, 
							   ID3D12Resource* buffer) : 
	Buffer(type, size, dynamic), 
	_device(_device),
	_buffer(buffer)
{
}

BufferDirect3D::~BufferDirect3D()
{
}

}