#include "Base.h"
#include "CommandPoolD3D12.h"
#include "CommandListD3D12.h"

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
	ID3D12GraphicsCommandList* commandListD3D = nullptr;
	ID3D12PipelineState* initialPipelineState = nullptr;
	if (FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator, initialPipelineState,
			                              __uuidof(commandListD3D), reinterpret_cast<void**>(&commandListD3D))))
	{
		GP_ERROR("Failed to create graphics command list.");
	}
    commandListD3D->Close();

	std::shared_ptr<CommandListD3D12> commandList = std::make_shared<CommandListD3D12>(_device, _commandAllocator, commandListD3D);
	return std::static_pointer_cast<CommandList>(commandList);
}

void CommandPoolD3D12::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
	std::shared_ptr<CommandListD3D12> commandListD3D = std::static_pointer_cast<CommandListD3D12>(commandList);
	GP_SAFE_RELEASE(commandListD3D->_commandList);
	commandList.reset();
}

}
