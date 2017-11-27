#include "Base.h"
#include "FenceD3D12.h"

namespace gameplay
{

FenceD3D12::FenceD3D12() :
	Fence(),
	_device(nullptr),
	_fence(nullptr),
	_fenceEvent(0),
	_fenceValue(0)
{
}

FenceD3D12::FenceD3D12(ID3D12Device* device, ID3D12Fence* fence, HANDLE fenceEvent) :
	Fence(),
	_device(device),
	_fence(fence),
	_fenceEvent(0),
	_fenceValue(0)
{
}

FenceD3D12::~FenceD3D12()
{
}

}