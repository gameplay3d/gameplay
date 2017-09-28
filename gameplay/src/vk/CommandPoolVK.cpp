#include "Base.h"
#include "CommandPoolVK.h"
#include "CommandListVK.h"

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
	VkCommandBuffer commandBufferVK;
	VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
	if (vkAllocateCommandBuffers(_device, &allocInfo, &commandBufferVK) != VK_SUCCESS)
	{
		GP_ERROR("Failed to allocate command buffers.");
	}
	std::shared_ptr<CommandListVK> commandLst = std::make_shared<CommandListVK>(_device, _commandPool, commandBufferVK);
	return std::static_pointer_cast<CommandList>(commandLst);
}

void CommandPoolVK::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
	std::shared_ptr<CommandListVK> commandListVK = std::static_pointer_cast<CommandListVK>(commandList);
	vkFreeCommandBuffers(_device, _commandPool, 1, &(commandListVK->_commandBuffer));
	commandList.reset();
}

}
