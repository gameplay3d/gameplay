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
	 * Destructor.
	 */
	~CommandListD3D12();

	void beginCommands();

	void endCommands();

};

}