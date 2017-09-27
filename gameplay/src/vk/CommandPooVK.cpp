#include "Base.h"
#include "CommandPoolVK.h"

namespace gameplay
{

CommandPoolVK::CommandPoolVK()
{
}

CommandPoolVK::~CommandPoolVK()
{
}

std::shared_ptr<CommandList> CommandPoolVK::createCommandList()
{
	return nullptr;
}

}
