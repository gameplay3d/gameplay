#include "Base.h"
#include "CommandPoolVK.h"

namespace gameplay
{

CommandPoolVK::CommandPoolVK() :
	_device(nullptr),
	_queue(nullptr),
	_commandPool(nullptr)
{
}

CommandPoolVK::CommandPoolVK(VkDevice device, VkQueue queue, VkCommandPool pool) :
	_device(device),
	_queue(queue),
	_commandPool(pool)
{
}

CommandPoolVK::~CommandPoolVK()
{
}

std::shared_ptr<CommandList> CommandPoolVK::createCommandList(bool secondary)
{
	return nullptr;
}

void CommandPoolVK::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
}

}
