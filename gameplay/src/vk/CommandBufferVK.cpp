#include "Base.h"
#include "CommandBufferVK.h"

namespace gameplay
{

CommandBufferVK::CommandBufferVK() :
	_device(nullptr), 
	_commandPool(nullptr), 
	_commandBuffer(nullptr)
{
}

CommandBufferVK::CommandBufferVK(VkDevice device, 
								 VkCommandPool commandPool, 
								 VkCommandBuffer commandBuffer) :
	_device(device), 
	_commandPool(commandPool), 
	_commandBuffer(commandBuffer)
{
}

CommandBufferVK::~CommandBufferVK()
{
}

}
