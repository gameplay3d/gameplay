#pragma once

#include "CommandPool.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Defines a Direct3D 12 impl of CommandPool.
 */
class CommandPoolD3D12 : public CommandPool
{
public:

	/**
	 * Constructor.
	 */
	CommandPoolD3D12();

	/**
	 * Constructor.
	 */
	CommandPoolD3D12(ID3D12CommandAllocator* commandAllocator);

	/**
	 * Destructor.
	 */
	~CommandPoolD3D12();

	ID3D12CommandAllocator* _commandAllocator;
};

}