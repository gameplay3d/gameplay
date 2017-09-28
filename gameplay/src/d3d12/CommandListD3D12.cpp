#include "Base.h"
#include "CommandListD3D12.h"

namespace gameplay
{

CommandListD3D12::CommandListD3D12() :
	_device(nullptr), _commandList(nullptr)
{
}

CommandListD3D12::CommandListD3D12(ID3D12Device* device, 
								   ID3D12CommandAllocator* commandAllocator, 
								   ID3D12GraphicsCommandList* commandList) :
	_device(device), 
	_commandAllocator(commandAllocator), 
	_commandList(commandList)
{
}

CommandListD3D12::~CommandListD3D12()
{
}

void CommandListD3D12::beginCommands()
{
	_commandAllocator->Reset();
	_commandList->Reset(_commandAllocator, nullptr);
}

void CommandListD3D12::endCommands()
{
	_commandList->Close();
}

}
