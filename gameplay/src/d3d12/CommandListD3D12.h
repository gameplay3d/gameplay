#pragma once

#include "CommandList.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Defines a Direct3D 12 impl of CommandList.
 */
class CommandListD3D12 : public CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandListD3D12();

	/**
	 * Constructor.
	 */
	CommandListD3D12(ID3D12Device* device, 
					 ID3D12CommandAllocator* commandAllocator, 
					 ID3D12GraphicsCommandList* commandList);
	/**
	 * Destructor.
	 */
	~CommandListD3D12();

	/**
	 * @see CommandList::beginCommands
	 */
	void beginCommands();

	/**
	 * @see CommandList::endCommands
	 */
	void endCommands();

	ID3D12Device* _device;
	ID3D12CommandAllocator* _commandAllocator;
	ID3D12GraphicsCommandList* _commandList;
};

}