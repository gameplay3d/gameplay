#include "Base.h"
#include "CommandBufferD3D12.h"

namespace gameplay
{

CommandBufferD3D12::CommandBufferD3D12() :
	_device(nullptr), 
	_commandAllocator(nullptr), 
	_commandList(nullptr)
{
}

CommandBufferD3D12::CommandBufferD3D12(ID3D12Device* device, 
									   ID3D12CommandAllocator* commandAllocator, 
									   ID3D12GraphicsCommandList* commandList) :
	_device(device), 
	_commandAllocator(commandAllocator), 
	_commandList(commandList)
{
}

CommandBufferD3D12::~CommandBufferD3D12()
{
}

}
