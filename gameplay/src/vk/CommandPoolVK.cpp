#include "Base.h"
#include "CommandPoolVK.h"
#include "CommandBufferVK.h"
#include "GraphicsVK.h"

namespace gameplay
{

CommandPoolVK::CommandPoolVK() :
	_device(nullptr), 
	_commandPool(nullptr)
{
}

CommandPoolVK::CommandPoolVK(VkDevice device, 
							 VkCommandPool commandPool) :
	_device(device), 
	_commandPool(commandPool)
{
}

CommandPoolVK::~CommandPoolVK()
{
}

std::shared_ptr<CommandBuffer> CommandPoolVK::createCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    VK_CHECK_RESULT(vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer));

    return std::make_shared<CommandBufferVK>(_device, _commandPool, commandBuffer);
}

void CommandPoolVK::destroyCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer)
{
    GP_ASSERT(commandBuffer);

    vkFreeCommandBuffers(_device, _commandPool, 1, &(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer));
    commandBuffer.reset();
}

}
