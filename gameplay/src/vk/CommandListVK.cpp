#include "Base.h"
#include "CommandListVK.h"

namespace gameplay
{

CommandListVK::CommandListVK() :
	_device(nullptr), _commandPool(nullptr), _commandBuffer(nullptr)
{
}

CommandListVK::CommandListVK(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer) :
	_device(device), _commandPool(commandPool), _commandBuffer(commandBuffer)
{
}

CommandListVK::~CommandListVK()
{
}

void CommandListVK::beginCommands()
{
	VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;;
    beginInfo.pNext = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;
	if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		GP_ERROR("Failed to begin commands.");
	}
}

void CommandListVK::endCommands()
{
	if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
	{
		GP_ERROR("Failed to end commands.");
	}
}

}
