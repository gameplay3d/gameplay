#pragma once

#include "CommandList.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Defines a Direct3D impl of CommandList.
 */
class CommandListDirect3D : public CommandList
{
public:

	/**
	 * Constructor.
	 */
	CommandListDirect3D();

	/**
	 * Destructor.
	 */
	~CommandListDirect3D();

	/**
	 * @see CommandList::clear
	 */
	void clear();
};

}