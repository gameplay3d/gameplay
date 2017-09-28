#include "Base.h"
#include "CommandPoolD3D12.h"
#include "CommandListD3D12.h"

namespace gameplay
{

CommandPoolD3D12::CommandPoolD3D12() :
	_commandAllocator(nullptr)
{
}

CommandPoolD3D12::CommandPoolD3D12(ID3D12CommandAllocator* commandAllocator) :
	_commandAllocator(commandAllocator)
{
}

CommandPoolD3D12::~CommandPoolD3D12()
{
}

}
