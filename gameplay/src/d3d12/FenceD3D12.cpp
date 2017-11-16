#include "Base.h"
#include "FenceD3D12.h"

namespace gameplay
{

FenceD3D12::FenceD3D12() :
	Fence(),
	_device(nullptr),
	_fence(nullptr)
{
}

FenceD3D12::FenceD3D12(ID3D12Device* device, ID3D12Fence* fence) :
	Fence(),
	_device(device),
	_fence(fence)
{
}

FenceD3D12::~FenceD3D12()
{
}

}