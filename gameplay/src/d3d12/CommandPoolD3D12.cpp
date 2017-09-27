#include "Base.h"
#include "CommandPoolD3D12.h"

namespace gameplay
{

CommandPoolD3D12::CommandPoolD3D12()
{
}

CommandPoolD3D12::~CommandPoolD3D12()
{
}

std::shared_ptr<CommandList> CommandPoolD3D12::createCommandList()
{
	return nullptr;
}

}
