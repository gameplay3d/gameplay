#include "Base.h"
#include "CommandPoolD3D12.h"

namespace gameplay
{

CommandPoolD3D12::CommandPoolD3D12() :
	_device(nullptr),
	_commandAllocator(nullptr)
{
}

CommandPoolD3D12::CommandPoolD3D12(ID3D12Device* device, ID3D12CommandAllocator* commandAllocator) :
	_device(device),
	_commandAllocator(commandAllocator)
{
}

CommandPoolD3D12::~CommandPoolD3D12()
{
}

std::shared_ptr<CommandList> CommandPoolD3D12::createCommandList(bool secondary)
{
	return nullptr;
}

void CommandPoolD3D12::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
}

}
