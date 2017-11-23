#include "Base.h"
#include "Format.h"
#include "GraphicsVK.h"
#include "BufferVK.h"
#include "TextureVK.h"
#include "RenderPassVK.h"
#include "SamplerVK.h"
#include "ShaderVK.h"
#include "DescriptorSetVK.h"
#include "RenderPipelineVK.h"
#include "CommandPoolVK.h"
#include "CommandBufferVK.h"
#include "SemaphoreVK.h"
#include "FenceVK.h"
#include "Game.h"
#include "FileSystem.h"

namespace gameplay
{

const std::vector<const char*> __validationLayers = 
{
	"VK_LAYER_LUNARG_standard_validation"
};

GraphicsVK::GraphicsVK() :
    _initialized(false),
    _resized(false),
	_width(0),
	_height(0),
	_fullscreen(false),
	_vsync(false),
	_multisampling(0),
	_validation(false),
	_instance(VK_NULL_HANDLE),
	_surface(VK_NULL_HANDLE),
	_physicalDevice(VK_NULL_HANDLE),
	_queueFamilyIndexGraphics(0),
	_queueIndex(0),
	_queue(nullptr),
	_device(VK_NULL_HANDLE),
	_swapchain(VK_NULL_HANDLE),
	_swapchainImageIndex(0),
	_swapchainImageCount(0),
	_colorSpace(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR),
	_colorFormat(VK_FORMAT_UNDEFINED),
	_depthStencilFormat(VK_FORMAT_UNDEFINED),
	_renderPass(nullptr),
	_debugMessageCallback(VK_NULL_HANDLE)
{
}

GraphicsVK::~GraphicsVK()
{
	vkDeviceWaitIdle(_device);
	
	// Destroy synchronization primitives here...

	vkDestroySwapchainKHR(_device, _swapchain, nullptr);
	vkDestroyDevice(_device, nullptr);

	if (vkDebugReportMessageEXT != VK_NULL_HANDLE && _debugMessageCallback != VK_NULL_HANDLE)
		vkDestroyDebugReportCallbackEXT(_instance, _debugMessageCallback, nullptr);

	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	vkDestroyInstance(_instance, nullptr);
}

void GraphicsVK::onInitialize(unsigned long window, unsigned long connection)
{
    if (_initialized)
        return;

	std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
	_width = config->width;
	_height = config->height;
	_fullscreen = config->fullscreen;
	_vsync = config->vsync;
	_multisampling = config->multisampling;
	_validation = config->validation;

	createInstance();
	createDevice();
	createSurface(window, connection);
	createSwapchain();

    _initialized = true;
    _resized = true;
}

bool GraphicsVK::isInitialized()
{
    return _initialized;
}

void GraphicsVK::onResize(int width, int height)
{
    if (!_resized)
		return;
    _resized = false;
	
	VK_CHECK_RESULT(vkDeviceWaitIdle(_device));

	// destroy render passes here..
	
	createSwapchain();

	VK_CHECK_RESULT(vkDeviceWaitIdle(_device));
    _resized = true;
}

bool GraphicsVK::isResized()
{
    return _resized;
}

int GraphicsVK::getWidth()
{
    return _width;
}

int GraphicsVK::getHeight()
{
    return _height;
}

std::shared_ptr<RenderPass> GraphicsVK::acquireNextSwapchainImage(std::shared_ptr<Fence> waitFence,
																  std::shared_ptr<Semaphore> signalSemaphore)
{
	VkFence fence;
	if (waitFence != nullptr)
	{
		fence = std::static_pointer_cast<FenceVK>(waitFence)->_fence;
	}
	VkSemaphore semaphore;
	if (signalSemaphore != nullptr)
	{
		semaphore = std::static_pointer_cast<SemaphoreVK>(signalSemaphore)->_semaphore;
	}
	VK_CHECK_RESULT(vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, semaphore, fence, &_swapchainImageIndex));
	VK_CHECK_RESULT(vkWaitForFences(_device, 1, &fence, VK_TRUE, UINT64_MAX));
	VK_CHECK_RESULT(vkResetFences(_device, 1, &fence));

	return _renderPasses[_swapchainImageIndex];
}

void GraphicsVK::present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores)
{
	VkSemaphore semaphores[GP_GRAPHICS_PRESENT_WAIT_SEMAPHORES_MAX];
	for (size_t i = 0; i < waitSemaphores.size(); i++)
	{
		semaphores[i] = std::static_pointer_cast<SemaphoreVK>(waitSemaphores[i])->_semaphore;
	}
	VkPresentInfoKHR presentInfo {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = waitSemaphores.size();
    presentInfo.pWaitSemaphores = semaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &_swapchain;
    presentInfo.pImageIndices = &_swapchainImageIndex;
    presentInfo.pResults = nullptr;

	VK_CHECK_RESULT(vkQueuePresentKHR(_queue, &presentInfo));
}

void GraphicsVK::waitIdle(std::shared_ptr<Fence> waitFence)
{
	VK_CHECK_RESULT(vkQueueWaitIdle(_queue));
}

std::shared_ptr<CommandPool> GraphicsVK::createCommandPool()
{
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = _queueFamilyIndexGraphics;

    VkCommandPool commandPool;
    VK_CHECK_RESULT(vkCreateCommandPool(_device, &info, nullptr, &commandPool));

	return std::make_shared<CommandPoolVK>(_device, commandPool);
}

void GraphicsVK::destroyCommandPool(std::shared_ptr<CommandPool> commandPool)
{
	GP_ASSERT(commandPool);

    vkDestroyCommandPool(_device, std::static_pointer_cast<CommandPoolVK>(commandPool)->_commandPool, nullptr);
	commandPool.reset();
}

void GraphicsVK::submit(std::vector<std::shared_ptr<CommandBuffer>> commandBuffersVK,
						std::vector<std::shared_ptr<Semaphore>> waitSemaphoresVK,
						std::vector<std::shared_ptr<Semaphore>> signalSemaphoresVK)
{
	VkCommandBuffer commandBuffers[GP_GRAPHICS_SUBMIT_COMMAND_BUFFERS_MAX];
	for (size_t i = 0; i < commandBuffersVK.size(); i++)
	{
		commandBuffers[i] = std::static_pointer_cast<CommandBufferVK>(commandBuffersVK[i])->_commandBuffer;
	}
	VkSemaphore waitSemaphores[GP_GRAPHICS_SUBMIT_WAIT_SEMAPHORES_MAX];
	VkPipelineStageFlags waitMasks[GP_GRAPHICS_SUBMIT_WAIT_SEMAPHORES_MAX];
	for (size_t i = 0; i < waitSemaphoresVK.size(); i++)
	{
		waitSemaphores[i] = std::static_pointer_cast<SemaphoreVK>(waitSemaphoresVK[i])->_semaphore;
		waitMasks[i] = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	}
	VkSemaphore signalSemaphores[GP_GRAPHICS_SUBMIT_SIGNAL_SEMAPHORES_MAX];
	for (size_t i = 0; i < signalSemaphoresVK.size(); i++)
	{
		signalSemaphores[i] = std::static_pointer_cast<SemaphoreVK>(signalSemaphoresVK[i])->_semaphore;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = waitSemaphoresVK.size();
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitMasks;
    submitInfo.commandBufferCount = commandBuffersVK.size();
    submitInfo.pCommandBuffers = commandBuffers;
    submitInfo.signalSemaphoreCount = signalSemaphoresVK.size();
    submitInfo.pSignalSemaphores = signalSemaphores;

	VK_CHECK_RESULT(vkQueueSubmit(_queue, 1, &submitInfo, nullptr));
}

void GraphicsVK::cmdBegin(std::shared_ptr<CommandBuffer> commandBuffer)
{
	GP_ASSERT(commandBuffer);
	VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;;
    beginInfo.pNext = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

	VK_CHECK_RESULT(vkBeginCommandBuffer(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, &beginInfo));
}

void GraphicsVK::cmdEnd(std::shared_ptr<CommandBuffer> commandBuffer)
{
	GP_ASSERT(commandBuffer);

	VK_CHECK_RESULT(vkEndCommandBuffer(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer));
}

void GraphicsVK::cmdBeginRender(std::shared_ptr<CommandBuffer> commandBuffer,
								std::shared_ptr<RenderPass> renderPass)
{
	GP_ASSERT(commandBuffer);
	GP_ASSERT(renderPass);

	VkRect2D renderArea = {};
    renderArea.offset.x = 0;
    renderArea.offset.y = 0;
	renderArea.extent.width = renderPass->getWidth();
	renderArea.extent.height = renderPass->getHeight();

	VkClearValue clearValues[(GP_GRAPHICS_COLOR_ATTACHMENTS_MAX * 2) + 1];
	size_t colorCount = renderPass->getColorAttachmentCount();
    for (size_t i = 0; i < colorCount; ++i) 
	{
		clearValues[i].color.float32[0] = renderPass->getColorAttachment(i)->getClearValue().color.red;
        clearValues[i].color.float32[1] = renderPass->getColorAttachment(i)->getClearValue().color.green;
        clearValues[i].color.float32[2] = renderPass->getColorAttachment(i)->getClearValue().color.blue;
        clearValues[i].color.float32[3] = renderPass->getColorAttachment(i)->getClearValue().color.alpha;
        if (renderPass->getSampleCount() > Texture::SAMPLE_COUNT_1X) 
		{
            clearValues[i].color.float32[0] = renderPass->getColorMultisampleAttachment(i)->getClearValue().color.red;
			clearValues[i].color.float32[1] = renderPass->getColorMultisampleAttachment(i)->getClearValue().color.green;
			clearValues[i].color.float32[2] = renderPass->getColorMultisampleAttachment(i)->getClearValue().color.blue;
			clearValues[i].color.float32[3] = renderPass->getColorMultisampleAttachment(i)->getClearValue().color.alpha;
        }
    }
	size_t depthStencilCount = (renderPass->getDepthStencilFormat() == Format::FORMAT_UNDEFINED) ? 1 : 0;
    if (depthStencilCount > 0) 
	{
        clearValues[colorCount].depthStencil.depth = renderPass->getDepthStencilAttachment()->getClearValue().depthStencil.depth;
        clearValues[colorCount].depthStencil.stencil = renderPass->getDepthStencilAttachment()->getClearValue().depthStencil.stencil;
    }
    uint32_t clearValueCount = colorCount;
    if (renderPass->getSampleCount() > Texture::SAMPLE_COUNT_1X) 
	{
        clearValueCount *= 2;
    }
    clearValueCount += depthStencilCount;

	std::shared_ptr<RenderPassVK> renderPassVK = std::static_pointer_cast<RenderPassVK>(renderPass);
	VkRenderPassBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = renderPassVK->_renderPass;
    beginInfo.framebuffer = renderPassVK->_framebuffer;
    beginInfo.renderArea = renderArea;
    beginInfo.clearValueCount = clearValueCount;
    beginInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void GraphicsVK::cmdEndRender(std::shared_ptr<CommandBuffer> commandBuffer)
{
	GP_ASSERT(commandBuffer);

	vkCmdEndRenderPass(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer);
}

void GraphicsVK::cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
								float x, float y, float width, float height, 
								float depthMin, float depthMax)
{
	GP_ASSERT(commandBuffer);

	VkViewport viewport = {};
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = depthMin;
    viewport.maxDepth = depthMax;

    vkCmdSetViewport(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 0, 1, &viewport);
}

void GraphicsVK::cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
							   size_t x, size_t y, 
							   size_t width, size_t height)
{
	GP_ASSERT(commandBuffer);

	VkRect2D rect = {};
    rect.offset.x = x;
    rect.offset.x = y;
    rect.extent.width = width;
    rect.extent.height = height;
    vkCmdSetScissor(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 0, 1, &rect);
}

void GraphicsVK::cmdClearColorAttachment(std::shared_ptr<CommandBuffer> commandBuffer,
										 size_t attachmentTndex,
										 const ClearValue& clearValue)
{
	GP_ASSERT(commandBuffer);
	VkClearAttachment attachment = {};
    attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    attachment.colorAttachment = attachmentTndex;
    attachment.clearValue.color.float32[0] = clearValue.color.red;
    attachment.clearValue.color.float32[1] = clearValue.color.green;
    attachment.clearValue.color.float32[2] = clearValue.color.blue;
    attachment.clearValue.color.float32[3] = clearValue.color.alpha;

    VkClearRect rect = {};
    rect.baseArrayLayer = 0;
    rect.layerCount = 1;
    rect.rect.offset.x = 0;
    rect.rect.offset.y = 0;
    rect.rect.extent.width = _renderPass->getWidth();
    rect.rect.extent.height = _renderPass->getHeight();

    vkCmdClearAttachments(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 1, 
						  &attachment, 1, 
						  &rect);
}

void GraphicsVK::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
									   std::shared_ptr<RenderPipeline> renderPipeline)
{
	GP_ASSERT(commandBuffer);

	vkCmdBindPipeline(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
					 VK_PIPELINE_BIND_POINT_GRAPHICS, 
					 std::static_pointer_cast<RenderPipelineVK>(renderPipeline)->_pipeline);
}

void GraphicsVK::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::shared_ptr<RenderPipeline> renderPipeline, 
									  std::shared_ptr<DescriptorSet> descriptorSet)
{
	GP_ASSERT(commandBuffer);

	vkCmdBindDescriptorSets(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							std::static_pointer_cast<RenderPipelineVK>(renderPipeline)->_pipelineLayout, 0, 1, 
							&(std::static_pointer_cast<DescriptorSetVK>(descriptorSet)->_descriptorSet), 0, nullptr);
}

void GraphicsVK::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									 std::shared_ptr<Buffer> vertexBuffer)
{
	GP_ASSERT(commandBuffer);
}

void GraphicsVK::cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::vector<std::shared_ptr<Buffer>> vertexBuffers)
{
	GP_ASSERT(commandBuffer);

    const uint32_t buffersMax = _physicalDeviceProperties.limits.maxVertexInputBindings;
    uint32_t buffersMaxCapped = vertexBuffers.size() > buffersMax ? buffersMax : vertexBuffers.size();
    GP_ASSERT(buffersMaxCapped < 64);

    VkBuffer buffers[64];
    VkDeviceSize offsets[64];
    for (uint32_t i = 0; i < buffersMaxCapped; ++i) 
	{
		buffers[i] = std::static_pointer_cast<BufferVK>(vertexBuffers[i])->_buffer;
    }

    vkCmdBindVertexBuffers(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 0, 
							buffersMaxCapped, buffers, offsets);
}

void GraphicsVK::cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									std::shared_ptr<Buffer> indexBuffer)
{
	GP_ASSERT(commandBuffer);

	VkIndexType indexType = (indexBuffer->getStride() == sizeof(unsigned short)) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    
	vkCmdBindIndexBuffer(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
					     std::static_pointer_cast<BufferVK>(indexBuffer)->_buffer, 0, indexType);
}

void GraphicsVK::cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
						size_t vertexCount, size_t vertexStart)
{
	GP_ASSERT(commandBuffer);

	 vkCmdDraw(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
			   vertexCount, 1, vertexStart, 0);
}

void GraphicsVK::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
								size_t indexCount, size_t indexStart)
{
	GP_ASSERT(commandBuffer);

	vkCmdDrawIndexed(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
					 indexCount, 1, indexStart, 0, 0);
}

void GraphicsVK::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
									std::shared_ptr<Texture> texture, 
									Texture::Usage usageOld, 
									Texture::Usage usageNew)
{
	GP_ASSERT(commandBuffer);

	VkPipelineStageFlags stageMskSrc = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkPipelineStageFlags stageMaskDst = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkDependencyFlags dependencyFlags = 0;
    
	std::shared_ptr<TextureVK> textureVK = std::static_pointer_cast<TextureVK>(texture);

	VkImageMemoryBarrier barrier = {};
    barrier.oldLayout = toVkImageLayout(usageOld);
    barrier.newLayout = toVkImageLayout(usageNew);
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = NULL;
    barrier.oldLayout = toVkImageLayout(usageOld);
    barrier.newLayout = toVkImageLayout(usageNew);
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = textureVK->_image;
    barrier.subresourceRange.aspectMask = textureVK->_imageAspectFlags;
    barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = textureVK->getMipLevels();
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    // oldLayout
    switch (barrier.oldLayout)
    {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		 break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT;
		break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		break;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		break;
    }

    // newImageLayout
    switch (barrier.newLayout)
    {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT;
		break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
	    break;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		break;
    }

    vkCmdPipelineBarrier(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer,
						 stageMskSrc, stageMaskDst, dependencyFlags, 0, nullptr, 0, nullptr, 1, &barrier);
}

std::shared_ptr<Semaphore> GraphicsVK::createSemaphore()
{
	VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;

	VkSemaphore semaphore;
    VK_CHECK_RESULT(vkCreateSemaphore(_device, &createInfo, nullptr, &semaphore));

	return std::make_shared<SemaphoreVK>(_device, semaphore);
}

void GraphicsVK::destroySemaphore(std::shared_ptr<Semaphore> semaphore)
{
	GP_ASSERT(semaphore);

	vkDestroySemaphore(_device, std::static_pointer_cast<SemaphoreVK>(semaphore)->_semaphore, nullptr);
	semaphore.reset();
}

std::shared_ptr<Fence> GraphicsVK::createFence()
{
	VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;

	VkFence fence;
    VK_CHECK_RESULT(vkCreateFence(_device, &createInfo, nullptr, &fence));

	return std::make_shared<FenceVK>(_device, fence);
}

void GraphicsVK::destroyFence(std::shared_ptr<Fence> fence)
{
	GP_ASSERT(fence);

	vkDestroyFence(_device, std::static_pointer_cast<FenceVK>(fence)->_fence, nullptr);
	fence.reset();
}

std::shared_ptr<Buffer> GraphicsVK::createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible)
{
	if (usage == Buffer::USAGE_UNIFORM) 
		size = GP_MATH_ROUNDUP(size, 256);
	
	VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = size;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;
	bufferCreateInfo.usage = 0;
	switch (usage) 
	{
	case Buffer::USAGE_VERTEX: 
		bufferCreateInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		break;
    case Buffer::USAGE_INDEX: 
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		break;
	case Buffer::USAGE_UNIFORM: 
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		break;
    }

	VkBuffer buffer;
	VK_CHECK_RESULT(vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &buffer));

	VkMemoryRequirements memReqs = {};
	vkGetBufferMemoryRequirements(_device, buffer, &memReqs);
	VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    if (hostVisible)
        memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uint32_t memoryTypeIndex = UINT32_MAX;
	if (!getMemoryTypeFromProperties(memReqs.memoryTypeBits, memFlags, &memoryTypeIndex))
		GP_ERROR("Failed to find compatible memory for buffer.");
	
	VkMemoryAllocateInfo  allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;
	VkDeviceMemory deviceMemoryVK;
    VK_CHECK_RESULT(vkAllocateMemory(_device, &allocInfo, nullptr, &deviceMemoryVK));
	VK_CHECK_RESULT(vkBindBufferMemory(_device, buffer, deviceMemoryVK, 0));

	std::shared_ptr<BufferVK> bufferVK = std::make_shared<BufferVK>(Buffer::USAGE_VERTEX, size, stride, hostVisible, _device, buffer, deviceMemoryVK);
	
	 if (hostVisible) 
		VK_CHECK_RESULT(vkMapMemory(_device, bufferVK->_deviceMemory, 0, VK_WHOLE_SIZE, 0, &bufferVK->_hostMemory));

	 bufferVK->_bufferView.buffer = buffer;
	 bufferVK->_bufferView.offset = 0;
	 bufferVK->_bufferView.range = VK_WHOLE_SIZE;

	return std::static_pointer_cast<Buffer>(bufferVK);
}

std::shared_ptr<Buffer> GraphicsVK::createVertexBuffer(size_t size, size_t vertexStride, bool hostVisible)
{
	return createBuffer(Buffer::USAGE_VERTEX, size, vertexStride, hostVisible);
}

std::shared_ptr<Buffer> GraphicsVK::createIndexBuffer(size_t size, IndexFormat indexFormat, bool hostVisible)
{
	size_t stride = (indexFormat == INDEX_FORMAT_UINT) ? sizeof(unsigned int) : sizeof(unsigned short);
	return createBuffer(Buffer::USAGE_INDEX, size, stride, hostVisible);
}

std::shared_ptr<Buffer> GraphicsVK::createUniformBuffer(size_t size, bool hostVisible)
{
	return createBuffer(Buffer::USAGE_UNIFORM, size, size, hostVisible);
}

void GraphicsVK::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
	std::shared_ptr<BufferVK> bufferVK = std::static_pointer_cast<BufferVK>(buffer);
	vkDestroyBuffer(_device, bufferVK->_buffer, nullptr);
	bufferVK.reset();
}

std::shared_ptr<Texture> GraphicsVK::createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels, 
												   Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount, 
												   const ClearValue& clearValue, bool hostVisible, VkImage existingImage)
{
	VkImageType imageType = VK_IMAGE_TYPE_2D;
	VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
    switch (type) 
	{
	case Texture::TYPE_1D:
		imageType = VK_IMAGE_TYPE_1D;
		imageViewType = VK_IMAGE_VIEW_TYPE_1D;
		break;
    case Texture::TYPE_2D: 
		imageType = VK_IMAGE_TYPE_2D;
		imageViewType = VK_IMAGE_VIEW_TYPE_2D;
		break;
    case Texture::TYPE_3D: 
		imageType = VK_IMAGE_TYPE_3D; 
		imageViewType = VK_IMAGE_VIEW_TYPE_3D;
		break;    
    }

	VkFormat format = toVkFormat(pixelFormat);
	VkImage image;
	VkDeviceMemory deviceMemory;
	bool hostOwned;
	if (existingImage == nullptr)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = imageType;
		imageCreateInfo.format = format;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = depth;
		imageCreateInfo.mipLevels = mipLevels;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = toVkSamples(sampleCount);
		imageCreateInfo.tiling = (hostVisible) ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = toVkImageUsageFlags(usage);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = 0;
		imageCreateInfo.pQueueFamilyIndices = nullptr;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		if (VK_IMAGE_USAGE_SAMPLED_BIT & imageCreateInfo.usage)
		{
			imageCreateInfo.usage |= (VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
		}

		VkFormatProperties formatProps = {};
		vkGetPhysicalDeviceFormatProperties(_physicalDevice, imageCreateInfo.format, &formatProps);
		VkFormatFeatureFlags formatFeatureFlags = toVkFormatFeatureFlags(imageCreateInfo.usage);

		if (hostVisible)
		{
			VkFormatFeatureFlags flags = formatProps.linearTilingFeatures & formatFeatureFlags;
			assert((0 != flags) && "Format is not supported for host visible images");
		}
		else
		{
			VkFormatFeatureFlags flags = formatProps.optimalTilingFeatures & formatFeatureFlags;
			assert((0 != flags) && "Format is not supported for GPU local images (i.e. not host visible images)");
		}

		// Apply some bounds to the image
		VkImageFormatProperties imageFormatProps = {};
		VkResult vkResult = vkGetPhysicalDeviceImageFormatProperties(_physicalDevice, imageCreateInfo.format,
			imageCreateInfo.imageType, imageCreateInfo.tiling,
			imageCreateInfo.usage, imageCreateInfo.flags, &imageFormatProps);
		GP_ASSERT(VK_SUCCESS == vkResult);
		if (imageCreateInfo.mipLevels > 1)
		{
			mipLevels = GP_MATH_MIN(mipLevels, imageFormatProps.maxMipLevels);
			imageCreateInfo.mipLevels = mipLevels;
		}

		// Create image
		VkImage image;
		VK_CHECK_RESULT(vkCreateImage(_device, &imageCreateInfo, nullptr, &image));

		// Find memory requirements and allocate memory for image
		VkMemoryRequirements memReqs{};
		vkGetImageMemoryRequirements(_device, image, &memReqs);

		VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		if (hostVisible)
		{
			memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		uint32_t memoryTypeIndex = UINT32_MAX;
		bool found = getMemoryTypeFromProperties(memReqs.memoryTypeBits, memFlags, &memoryTypeIndex);
		if (!found)
			GP_ERROR("Failed to find compatible memory for texture.");

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = memReqs.size;
		allocInfo.memoryTypeIndex = memoryTypeIndex;

		VK_CHECK_RESULT(vkAllocateMemory(_device, &allocInfo, nullptr, &deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(_device, image, deviceMemory, 0));
		hostOwned = true;
	}
	else
	{
		image = existingImage;
		hostOwned = false;
	}

	std::shared_ptr<TextureVK> textureVK = std::make_shared<TextureVK>(type, width, height, depth, mipLevels,
																	pixelFormat, usage, sampleCount, clearValue, hostVisible, hostOwned,
																	_device, image, deviceMemory);
	if (hostVisible)
	{
		VK_CHECK_RESULT(vkMapMemory(_device, textureVK->_deviceMemory, 0, VK_WHOLE_SIZE, 0, &textureVK->_hostMemory));
	}

	// Create image view
	VkImageViewCreateInfo imageViewCreateinfo = {};
    imageViewCreateinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateinfo.pNext = nullptr;
    imageViewCreateinfo.flags = 0;
    imageViewCreateinfo.image = image;
    imageViewCreateinfo.viewType = imageViewType;
    imageViewCreateinfo.format = format;
    imageViewCreateinfo.components.r = VK_COMPONENT_SWIZZLE_R;
    imageViewCreateinfo.components.g = VK_COMPONENT_SWIZZLE_G;
    imageViewCreateinfo.components.b = VK_COMPONENT_SWIZZLE_B;
    imageViewCreateinfo.components.a = VK_COMPONENT_SWIZZLE_A;
    imageViewCreateinfo.subresourceRange.aspectMask = toVkImageAspectFlags(format);
    imageViewCreateinfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateinfo.subresourceRange.levelCount = mipLevels;
    imageViewCreateinfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateinfo.subresourceRange.layerCount = 1;

    VK_CHECK_RESULT(vkCreateImageView(_device, &imageViewCreateinfo, nullptr, &(textureVK->_imageView)));

	textureVK->_imageAspectFlags = imageViewCreateinfo.subresourceRange.aspectMask;
	textureVK->_imageViewInfo.imageView = textureVK->_imageView;
    textureVK->_imageViewInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	return std::static_pointer_cast<Texture>(textureVK);
}

std::shared_ptr<Texture> GraphicsVK::createTexture1d(size_t width, 
													Format pixelFormat, 
													Texture::Usage usage, 
													Texture::SampleCount sampleCount,
													const ClearValue& clearValue,
													bool hostVisible)
{
	return createTexture(Texture::TYPE_1D, width, 1, 1, 1, 
						 pixelFormat, usage, sampleCount, clearValue, hostVisible, 
						 nullptr);
}

std::shared_ptr<Texture> GraphicsVK::createTexture2d(size_t width, size_t height, size_t mipLevels,
													 Format pixelFormat,
													 Texture::Usage usage,
													 Texture::SampleCount sampleCount,
												     const ClearValue& clearValue,
													 bool hostVisible)
{
	return createTexture(Texture::TYPE_2D, width, height, 1, mipLevels, 
						 pixelFormat, usage, sampleCount, clearValue, hostVisible, 
						 nullptr);
}

std::shared_ptr<Texture> GraphicsVK::createTexture3d(size_t width, size_t height, size_t depth,
													 Format pixelFormat,
													 Texture::Usage usage,
													 Texture::SampleCount sampleCount,
													 const ClearValue& clearValue,
													 bool hostVisible)
{
	return createTexture(Texture::TYPE_3D, width, height, depth, 1, 
						 pixelFormat, usage, sampleCount, clearValue, hostVisible, 
						 nullptr);
}

void GraphicsVK::destroyTexture(std::shared_ptr<Texture> texture)
{
	std::shared_ptr<TextureVK> textureVK = std::static_pointer_cast<TextureVK>(texture);
	if (texture->isHostOwned())
	{
		if (textureVK->_deviceMemory)
			vkFreeMemory(_device, textureVK->_deviceMemory, nullptr);
		if (textureVK->_image)
			vkDestroyImage(_device, textureVK->_image, nullptr);
	}
	if (textureVK->_imageView)
		vkDestroyImageView(_device, textureVK->_imageView, nullptr);
	
	textureVK.reset();
}

std::shared_ptr<RenderPass> GraphicsVK::createRenderPass(size_t width, size_t height,
														 size_t colorAttachmentCount,
														 VkFormat colorFormatVK,
														 VkFormat depthStencilFormatVK,
														 VkSampleCountFlagBits sampleCountVK,
														 std::vector<std::shared_ptr<Texture>> colorAttachments,
														 std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
														 std::shared_ptr<Texture> depthStencilAttachment)
{
	Format colorFormat = toFormat(colorFormatVK);
	Format depthStencilFormat = toFormat(depthStencilFormatVK);
	Texture::SampleCount sampleCount = toSamples(sampleCountVK);

	// Create the render pass
	VkAttachmentDescription* attachmentDescs = nullptr;
	VkAttachmentReference* colorAttachmentRefs = nullptr;
    VkAttachmentReference* resolveAttachmentRefs = nullptr;
    VkAttachmentReference* depthStencilAttachmentRef = nullptr;
	size_t depthStencilAttachmentCount = depthStencilFormat == Format::FORMAT_UNDEFINED ? 0 : 1;

	if (sampleCount > Texture::SAMPLE_COUNT_1X)
	{
		attachmentDescs = (VkAttachmentDescription*)calloc((2 * colorAttachmentCount) + depthStencilAttachmentCount, sizeof(*attachmentDescs));
		GP_ASSERT(attachmentDescs != nullptr);
		if (colorAttachmentCount > 0) 
		{
            colorAttachmentRefs = (VkAttachmentReference*)calloc(colorAttachmentCount, sizeof(*colorAttachmentRefs));
            GP_ASSERT(colorAttachmentRefs != nullptr);
            resolveAttachmentRefs = (VkAttachmentReference*)calloc(colorAttachmentCount, sizeof(*resolveAttachmentRefs));
            GP_ASSERT(resolveAttachmentRefs != nullptr);
        }
		if (depthStencilAttachmentCount > 0)
		{
            depthStencilAttachmentRef = (VkAttachmentReference*)calloc(1, sizeof(*depthStencilAttachmentRef));
            GP_ASSERT(depthStencilAttachmentRef);
        }
        for (size_t i = 0; i < colorAttachmentCount; ++i) 
		{
            const size_t colorIndex = 2 * i;
            const size_t multiSampleIndex = colorIndex + 1;
            attachmentDescs[colorIndex].flags = 0;
            attachmentDescs[colorIndex].format = colorFormatVK;
            attachmentDescs[colorIndex].samples = sampleCountVK;
            attachmentDescs[colorIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[colorIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[colorIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[colorIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[colorIndex].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachmentDescs[colorIndex].flags = 0;

			attachmentDescs[multiSampleIndex].format = colorFormatVK;
            attachmentDescs[multiSampleIndex].samples = sampleCountVK;
            attachmentDescs[multiSampleIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[multiSampleIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[multiSampleIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[multiSampleIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[multiSampleIndex].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachmentDescs[multiSampleIndex].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            colorAttachmentRefs[i].attachment = multiSampleIndex;
            colorAttachmentRefs[i].layout = attachmentDescs[multiSampleIndex].initialLayout;
            resolveAttachmentRefs[i].attachment = colorIndex;
            resolveAttachmentRefs[i].layout = attachmentDescs[colorIndex].initialLayout;
        }
	
        if (depthStencilAttachmentCount > 0)
		{
            const uint32_t index = (2 * colorAttachmentCount);
            attachmentDescs[index].flags = 0;
            attachmentDescs[index].format = depthStencilFormatVK;
            attachmentDescs[index].samples = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescs[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[index].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachmentDescs[index].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthStencilAttachmentRef[0].attachment = index;
            depthStencilAttachmentRef[0].layout = attachmentDescs[index].initialLayout;
        }
	}
	else
	{
		attachmentDescs = (VkAttachmentDescription*)calloc(colorAttachmentCount + depthStencilAttachmentCount, sizeof(*attachmentDescs));
        GP_ASSERT(attachmentDescs != nullptr);

        if (colorAttachmentCount > 0) 
		{
            colorAttachmentRefs = (VkAttachmentReference*)calloc(colorAttachmentCount, sizeof(*colorAttachmentRefs));
            GP_ASSERT(colorAttachmentRefs != nullptr);
        }
        if (depthStencilAttachmentCount > 0) 
		{
            depthStencilAttachmentRef = (VkAttachmentReference*)calloc(1, sizeof(*depthStencilAttachmentRef));
            GP_ASSERT(depthStencilAttachmentRef != nullptr);
        }
        for (uint32_t i = 0; i < colorAttachmentCount; ++i)
		{
            const uint32_t colorIndex = i;
            attachmentDescs[colorIndex].flags = 0;
            attachmentDescs[colorIndex].format = colorFormatVK;
            attachmentDescs[colorIndex].samples = sampleCountVK;
            attachmentDescs[colorIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[colorIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[colorIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[colorIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[colorIndex].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachmentDescs[colorIndex].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            colorAttachmentRefs[i].attachment = colorIndex;
            colorAttachmentRefs[i].layout = attachmentDescs[colorIndex].initialLayout;
        }

        if (depthStencilAttachmentCount > 0) 
		{
            uint32_t index = colorAttachmentCount;
            attachmentDescs[index].flags = 0;
            attachmentDescs[index].format = depthStencilFormatVK;
            attachmentDescs[index].samples = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescs[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[index].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachmentDescs[index].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            depthStencilAttachmentRef[0].attachment = index;
            depthStencilAttachmentRef[0].layout = attachmentDescs[index].initialLayout;
        }
	}

	VkSubpassDescription subpass = {};
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = colorAttachmentCount;
    subpass.pColorAttachments = colorAttachmentRefs;
    subpass.pResolveAttachments = resolveAttachmentRefs;
    subpass.pDepthStencilAttachment = depthStencilAttachmentRef;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

	// Create self-dependency in case image or memory barrier is issued within subpass
	VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = 0;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	uint32_t attachmentCount = (sampleCount > Texture::SAMPLE_COUNT_1X) ? (2 * colorAttachmentCount) : colorAttachmentCount;
    attachmentCount += depthStencilAttachmentCount;

	// Create render pass
	VkRenderPass renderPass;
	{
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.attachmentCount = attachmentCount;
		createInfo.pAttachments = attachmentDescs;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &subpassDependency;

		VK_CHECK_RESULT(vkCreateRenderPass(_device, &createInfo, nullptr, &renderPass));
	}

	GP_SAFE_FREE(attachmentDescs);
	GP_SAFE_FREE(colorAttachmentRefs);
    GP_SAFE_FREE(resolveAttachmentRefs);
    GP_SAFE_FREE(depthStencilAttachmentRef);

	// Create the framebuffer
	VkFramebuffer framebuffer;
	{
		size_t imageCount = colorAttachmentCount;
		if (sampleCount > Texture::SAMPLE_COUNT_1X)
		{
			imageCount *= 2;
		}
		if (depthStencilFormat != Format::FORMAT_UNDEFINED)
		{
			imageCount += 1;
		}

		// Create the framebuffer textures
		VkImageView* imageViews = (VkImageView*)calloc(imageCount, sizeof(*imageViews));
		VkImageView* imageView = imageViews;
		for (size_t i = 0; i < colorAttachmentCount; ++i)
		{
		    std::shared_ptr<TextureVK> colorAttachmentVK = std::static_pointer_cast<TextureVK>(colorAttachments[i]);
			*imageView = colorAttachmentVK->_imageView;
			++imageView;
			if (sampleCount > Texture::SAMPLE_COUNT_1X)
			{
				std::shared_ptr<TextureVK> colorMultisampleAttachmentVK = std::static_pointer_cast<TextureVK>(colorMultisampleAttachments[i]);
				*imageView = colorMultisampleAttachmentVK->_imageView;
				++imageView;
			}
		}
		if (depthStencilFormat != Format::FORMAT_UNDEFINED)
		{
			std::shared_ptr<TextureVK> depthStencilAttachmentVK = std::static_pointer_cast<TextureVK>(depthStencilAttachment);
			*imageView = depthStencilAttachmentVK->_imageView;
			++imageView;
		}

		// Create the framebuffer
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = attachmentCount;
		createInfo.pAttachments = imageViews;
		createInfo.width = width;
		createInfo.height = height;
		createInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(_device, &createInfo, nullptr, &framebuffer));
		GP_SAFE_FREE(imageViews);
	}

	std::shared_ptr<RenderPassVK> renderPassVK = std::make_shared<RenderPassVK>(width, height,
																			  colorAttachmentCount, colorFormat,
																			  depthStencilFormat, 
																			  sampleCount,
																			  colorAttachments,
																			  colorMultisampleAttachments,
																			  depthStencilAttachment,
																			  _device, renderPass, framebuffer);
	return std::static_pointer_cast<RenderPass>(renderPassVK);
}

std::shared_ptr<RenderPass> GraphicsVK::createRenderPass(size_t width, size_t height,
														 size_t colorAttachmentCount,
														 Format colorFormat,
														 Format depthStencilFormat,
														 Texture::SampleCount sampleCount)
{
	VkFormat colorFormatVK = toVkFormat(colorFormat);
	VkFormat depthStencilFormatVK = toVkFormat(depthStencilFormat);
	VkSampleCountFlagBits sampleCountVK = toVkSamples(sampleCount);

	// Create the textures for the framebuffer
	std::vector<std::shared_ptr<Texture>> colorAttachments;
	std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;
	std::shared_ptr<Texture> depthStencilAttachment = nullptr;
	size_t imageCount = colorAttachmentCount;
	if (sampleCount > Texture::SAMPLE_COUNT_1X)
	{
		imageCount *= 2;
	}
	if (depthStencilFormat != Format::FORMAT_UNDEFINED)
	{
		imageCount += 1;
	}
	for (size_t i = 0; i < colorAttachmentCount; ++i)
	{
		ClearValue clearColor;
		clearColor.color.red = 0;
		clearColor.color.green = 0;
		clearColor.color.blue = 0;
		clearColor.color.red = 0;
		std::shared_ptr<Texture> colorAttachment = createTexture2d(width, height, 1, colorFormat, 
																   Texture::USAGE_COLOR_ATTACHMENT, 
																   Texture::SAMPLE_COUNT_1X, clearColor, false);
		std::shared_ptr<TextureVK> colorAttachmentVK = std::static_pointer_cast<TextureVK>(colorAttachment);
		colorAttachments.push_back(colorAttachment);
		if (sampleCount > Texture::SAMPLE_COUNT_1X)
		{
			std::shared_ptr<Texture> colorMultisampleAttachment = createTexture2d(width, height, 1, colorFormat, 
																				  Texture::USAGE_COLOR_ATTACHMENT, 
																				  sampleCount, clearColor, false);
			std::shared_ptr<TextureVK> colorMultisampleAttachmentVK = std::static_pointer_cast<TextureVK>(colorMultisampleAttachment);
			colorMultisampleAttachments.push_back(colorMultisampleAttachment);
		}
	}
	if (depthStencilFormat != Format::FORMAT_UNDEFINED)
	{
		ClearValue clearDepthStencil;
		clearDepthStencil.depthStencil.depth = 0.0f;
		clearDepthStencil.depthStencil.stencil = 0;
		depthStencilAttachment = createTexture2d(width, height, 1, depthStencilFormat, 
													Texture::USAGE_DEPTH_STENCIL_ATTACHMENT, 
													Texture::SAMPLE_COUNT_1X, clearDepthStencil, false);
		std::shared_ptr<TextureVK> depthStencilAttachmentVK = std::static_pointer_cast<TextureVK>(depthStencilAttachment);
	}
	return createRenderPass(width, height,
							colorAttachmentCount,
							colorFormatVK,
							depthStencilFormatVK,
							sampleCountVK,
							colorAttachments,
							colorMultisampleAttachments,
							depthStencilAttachment);
}

void GraphicsVK::destroyRenderPass(std::shared_ptr<RenderPass> renderPass)
{
	GP_ASSERT(renderPass);

	for (size_t i = 0; i < renderPass->getColorAttachmentCount(); i++)
	{
		destroyTexture(renderPass->getColorAttachment(i));
		if (renderPass->getSampleCount() > Texture::SAMPLE_COUNT_1X)
		{
			destroyTexture(renderPass->getColorMultisampleAttachment(i));
		}
	}
	if (renderPass->getDepthStencilFormat() != Format::FORMAT_UNDEFINED)
	{
		destroyTexture(renderPass->getDepthStencilAttachment());
	}

	vkDestroyFramebuffer(_device, std::static_pointer_cast<RenderPassVK>(renderPass)->_framebuffer, nullptr);
	vkDestroyRenderPass(_device, std::static_pointer_cast<RenderPassVK>(renderPass)->_renderPass, nullptr);
	renderPass.reset();
}

std::shared_ptr<Sampler> GraphicsVK::createSampler(Sampler::Filter filterMag,
												   Sampler::Filter filterMin,
												   Sampler::Filter filterMip,
												   Sampler::AddressMode addressModeU,
												   Sampler::AddressMode addressModeV,
												   Sampler::AddressMode addressModeW,
												   Sampler::BorderColor borderColor,
												   bool compareEnabled,
												   Sampler::CompareFunc compareFunc,
												   bool anisotropyEnabled,
												   float anisotropyMax,
												   float lodMin,
												   float lodMax,
												   float lodMipBias)
{
	VkSamplerCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
	createInfo.magFilter = toVkFilter(filterMag);
    createInfo.minFilter = toVkFilter(filterMin);
    createInfo.mipmapMode = toVkSamplerMipmapMode(filterMip);
    createInfo.addressModeU = toVkSamplerAddressMode(addressModeU);
    createInfo.addressModeV = toVkSamplerAddressMode(addressModeV);
    createInfo.addressModeW = toVkSamplerAddressMode(addressModeW);;
	createInfo.borderColor = toVkBorderColor(borderColor);
	createInfo.compareEnable = compareEnabled ? VK_TRUE : VK_FALSE;
	createInfo.compareOp = toVkCompareOp(compareFunc);
    createInfo.anisotropyEnable = anisotropyEnabled ? VK_TRUE : VK_FALSE;
    createInfo.maxAnisotropy = anisotropyMax;
    createInfo.minLod = lodMin;
    createInfo.maxLod = lodMax;
	createInfo.mipLodBias = lodMipBias;
    createInfo.unnormalizedCoordinates = VK_FALSE;

	VkSampler sampler;
    VK_CHECK_RESULT(vkCreateSampler(_device, &createInfo, nullptr, &sampler));
 
	std::shared_ptr<SamplerVK> samplerVK = std::make_shared<SamplerVK>(filterMag, filterMin, filterMip,
																	   addressModeU, addressModeV, addressModeW,
																	   borderColor, compareEnabled, compareFunc,
																	   anisotropyEnabled, anisotropyMax,
																	   lodMin, lodMax, lodMipBias, _device, sampler);
	return std::static_pointer_cast<SamplerVK>(samplerVK);
}

void GraphicsVK::destroySampler(std::shared_ptr<Sampler> sampler)
{
	std::shared_ptr<SamplerVK> samplerVK = std::static_pointer_cast<SamplerVK>(sampler);
	vkDestroySampler(_device, samplerVK->_sampler, nullptr);
	sampler.reset();
}

std::shared_ptr<Shader> GraphicsVK::createShader(const std::string& url)
{
	std::string shaderUrl = FileSystem::getHomePath();
	shaderUrl.append(GP_GRAPHICS_VK_SHADER_PATH);
	shaderUrl.append(url);
	shaderUrl.append(GP_GRAPHICS_VK_SHADER_EXT);
	std::string shaderData = FileSystem::readAll(shaderUrl);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.codeSize = shaderData.size();
	createInfo.pCode = (uint32_t*)shaderData.c_str();
	createInfo.flags = 0;

	VkShaderModule shaderModule;
	VK_CHECK_RESULT(vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule));

	std::shared_ptr<ShaderVK> shader = std::make_shared<ShaderVK>(_device, shaderModule);	
	return std::static_pointer_cast<Shader>(shader);
}

void GraphicsVK::destroyShader(std::shared_ptr<Shader> shader)
{
	std::shared_ptr<ShaderVK> shaderVK = std::static_pointer_cast<ShaderVK>(shader);
	vkDestroyShaderModule(_device, shaderVK->_shader, nullptr);
	shaderVK.reset();
}

std::shared_ptr<DescriptorSet> GraphicsVK::createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
															   size_t descriptorCount)
{
	enum { maxTypes = 12 };
	VkDescriptorPoolSize poolSizesByType[maxTypes];
    poolSizesByType[VK_DESCRIPTOR_TYPE_SAMPLER].type = VK_DESCRIPTOR_TYPE_SAMPLER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    poolSizesByType[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizesByType[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizesByType[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizesByType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    poolSizesByType[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	VkDescriptorSetLayoutBinding* bindings = (VkDescriptorSetLayoutBinding*)calloc(descriptorCount, sizeof(*bindings));
	 for (size_t i = 0; i < descriptorCount; i++) 
	 {
        const DescriptorSet::Descriptor* descriptor = &descriptors[i];
        VkDescriptorSetLayoutBinding* binding = &(bindings[i]);
        uint32_t typeIndex = UINT32_MAX;
        switch (descriptor->type) 
		{
		case DescriptorSet::Descriptor::TYPE_SAMPLER: 
			typeIndex = VK_DESCRIPTOR_TYPE_SAMPLER; 
			break;
        case DescriptorSet::Descriptor::TYPE_TEXTURE: 
			typeIndex = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE; 
			break;
        case DescriptorSet::Descriptor::TYPE_UNIFORM: 
			typeIndex = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
			break;
        }
        if (typeIndex != UINT32_MAX) 
		{
            binding->binding = descriptor->binding;
            binding->descriptorType = (VkDescriptorType)typeIndex;
            binding->descriptorCount = descriptor->count;
            binding->stageFlags = toVkShaderStageFlags(descriptor->shaderStage);
            binding->pImmutableSamplers = nullptr;

            poolSizesByType[typeIndex].descriptorCount += descriptor->count;
        }
    }

	size_t poolSizeCount = 0;
    VkDescriptorPoolSize poolSizes[maxTypes];
    for (uint32_t i = 0; i < maxTypes; ++i) 
	{
        if (poolSizesByType[i].descriptorCount > 0) 
		{
            poolSizes[poolSizeCount].type = poolSizesByType[i].type;
            poolSizes[poolSizeCount].descriptorCount = poolSizesByType[i].descriptorCount;
            ++poolSizeCount;
        }
    }

	// Descriptor pool
	VkDescriptorPool descriptorPool;
    {
		VkDescriptorPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = 1;
        createInfo.poolSizeCount = poolSizeCount;
        createInfo.pPoolSizes = poolSizes;

        VK_CHECK_RESULT(vkCreateDescriptorPool(_device, &createInfo, nullptr, &descriptorPool));
    }

    // DescriptorSet layout
	VkDescriptorSetLayout descriptorSetLayout;
	
    {
		VkDescriptorSetLayoutCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
		createInfo.bindingCount = descriptorCount;
        createInfo.pBindings = bindings;

        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(_device, &createInfo, nullptr, &descriptorSetLayout));
    }

    // Allocate DescriptorSet
	VkDescriptorSet descriptorSet;
    {
		VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(_device, &allocInfo, &descriptorSet));
    }

	std::shared_ptr<DescriptorSetVK> descriptorSetVK = std::make_shared<DescriptorSetVK>(descriptors, descriptorCount,
																					     _device,
																						 descriptorPool,
																					     descriptorSetLayout,
																						 descriptorSet);
    GP_SAFE_FREE(bindings);
	return std::static_pointer_cast<DescriptorSet>(descriptorSetVK);
}

void GraphicsVK::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet)
{
	std::shared_ptr<DescriptorSetVK> descriptorSetVK = std::static_pointer_cast<DescriptorSetVK>(descriptorSet);

	VK_CHECK_RESULT(vkFreeDescriptorSets(_device, descriptorSetVK->_descriptorPool, 1, &descriptorSetVK->_descriptorSet));
	vkDestroyDescriptorSetLayout(_device, descriptorSetVK->_descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(_device, descriptorSetVK->_descriptorPool, nullptr);
	descriptorSet.reset();
}

std::shared_ptr<RenderPipeline> GraphicsVK::createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
																 VertexLayout vertexLayout,
																 RasterizerState rasterizerState,
																 ColorBlendState colorBlendState,
																 DepthStencilState depthStencilState,
																 std::shared_ptr<RenderPass> renderPass,
																 std::shared_ptr<DescriptorSet> descriptorSet,
																 std::shared_ptr<Shader> vertShader,
																 std::shared_ptr<Shader> tescShader,
																 std::shared_ptr<Shader> teseShader,
																 std::shared_ptr<Shader> geomShader,
																 std::shared_ptr<Shader> fragShader)
{
	GP_ASSERT(vertexLayout.getAttributeCount() > 0);
	GP_ASSERT(renderPass != nullptr);
	GP_ASSERT(descriptorSet != nullptr);

    // ShaderStages
	std::vector<VkPipelineShaderStageCreateInfo> stagesCreateInfo;
	{
		if (vertShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			createInfo.module = std::static_pointer_cast<ShaderVK>(vertShader)->_shader;
			stagesCreateInfo.push_back(createInfo);
		}
		if (tescShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			createInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			createInfo.module = std::static_pointer_cast<ShaderVK>(tescShader)->_shader;
			stagesCreateInfo.push_back(createInfo);
		}
		if (tescShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			createInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			createInfo.module = std::static_pointer_cast<ShaderVK>(teseShader)->_shader;
			stagesCreateInfo.push_back(createInfo);
		}
		if (geomShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			createInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
			createInfo.module = std::static_pointer_cast<ShaderVK>(geomShader)->_shader;
			stagesCreateInfo.push_back(createInfo);
		}
		if (fragShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			createInfo.module = std::static_pointer_cast<ShaderVK>(fragShader)->_shader;
			stagesCreateInfo.push_back(createInfo);
		}
	}

	// VertexInputState
    uint32_t inputBindingCount = 0;
    VkVertexInputBindingDescription inputBindings[GP_GRAPHICS_VERTEX_BINDINGS_MAX] = { 0 };

    VkVertexInputAttributeDescription inputAttributes[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX] = { 0 };
    uint32_t inputAttributeCount = vertexLayout.getAttributeCount() > GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX ? GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX : vertexLayout.getAttributeCount();
    uint32_t bindingValue = UINT32_MAX;

    for (size_t i = 0; i < inputAttributeCount; ++i) 
	{
		VertexLayout::VertexAttribute attribute = vertexLayout.getAttribute(i);
        if (attribute.binding != bindingValue) 
		{
            bindingValue = attribute.binding;
            ++inputBindingCount;
        }

        inputBindings[inputBindingCount - 1].binding = bindingValue;
        inputBindings[inputBindingCount - 1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		inputBindings[inputBindingCount - 1].stride += VertexLayout::toStride(attribute.format);

        inputAttributes[inputAttributeCount].location = attribute.location;
        inputAttributes[inputAttributeCount].binding = attribute.binding;
        inputAttributes[inputAttributeCount].format = toVkFormat(attribute.format);
        inputAttributes[inputAttributeCount].offset = attribute.offset;
        ++inputAttributeCount;
    }

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.pNext = nullptr;
    vertexInputStateCreateInfo.flags = 0;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = inputBindingCount;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = inputBindings;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = inputAttributeCount;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = inputAttributes;

	// Topology
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    switch (primitiveTopology) 
	{
	case RenderPipeline::PRIMITIVE_TOPOLOGY_POINT_LIST: 
		topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_LINE_LIST: 
		topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_LINE_STRIP: 
		topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: 
		topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: 
		topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; 
		break;
    }

	// InputAssemblyState
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
    inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.pNext = nullptr;
    inputAssemblyStateCreateInfo.flags = 0;
    inputAssemblyStateCreateInfo.topology = topology;
    inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	// TesselationState
	VkPipelineTessellationStateCreateInfo tessStateCreateInfo = {};
    tessStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessStateCreateInfo.pNext = NULL;
    tessStateCreateInfo.flags = 0;
    tessStateCreateInfo.patchControlPoints = 0;

	// ViewportState
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pNext = nullptr;
    viewportStateCreateInfo.flags = 0;
    viewportStateCreateInfo.viewportCount = 0;
    viewportStateCreateInfo.pViewports = nullptr;
    viewportStateCreateInfo.scissorCount = 0;
    viewportStateCreateInfo.pScissors = nullptr;
        
	// RasterizerState
	VkPipelineRasterizationStateCreateInfo rasterizerStateCreateInfo = {};
    rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerStateCreateInfo.pNext = nullptr;
    rasterizerStateCreateInfo.flags = 0;
    rasterizerStateCreateInfo.depthClampEnable = rasterizerState.depthClipEnabled;
    rasterizerStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerStateCreateInfo.polygonMode = toVkPolygonMode(rasterizerState.polygonMode);
    rasterizerStateCreateInfo.cullMode = toVkCullModeFlags(rasterizerState.cullMode);
    rasterizerStateCreateInfo.frontFace = toVkFrontFace(rasterizerState.frontFace);
    rasterizerStateCreateInfo.depthBiasEnable = rasterizerState.depthBias == 0 ? VK_FALSE : VK_TRUE;
    rasterizerStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizerStateCreateInfo.depthBiasClamp = rasterizerState.depthBiasClamp;
    rasterizerStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    rasterizerStateCreateInfo.lineWidth = rasterizerState.lineWidth;

	// MultisampleState
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.pNext = nullptr;
    multisampleStateCreateInfo.flags = 0;
    multisampleStateCreateInfo.rasterizationSamples = toVkSamples(renderPass->getSampleCount());
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.minSampleShading = 0.0f;
    multisampleStateCreateInfo.pSampleMask = 0;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	// DepthStencilState
	VkPipelineDepthStencilStateCreateInfo  depthStencilStateCreateInfo = {};
    depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateCreateInfo.pNext = nullptr;
    depthStencilStateCreateInfo.flags = 0;
	depthStencilStateCreateInfo.depthTestEnable = depthStencilState.depthEnable ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.depthWriteEnable = depthStencilState.depthWrite ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.depthCompareOp = toVkCompareOp(depthStencilState.depthFunc);
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable = depthStencilState.stencilEnable ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.front.failOp = toVkStencilOp(depthStencilState.stencilOpStateFront.failOp);
    depthStencilStateCreateInfo.front.passOp = toVkStencilOp(depthStencilState.stencilOpStateFront.passOp);
    depthStencilStateCreateInfo.front.depthFailOp = toVkStencilOp(depthStencilState.stencilOpStateFront.depthFailOp);
    depthStencilStateCreateInfo.front.compareOp = toVkCompareOp(depthStencilState.stencilOpStateFront.compareFunc);
    depthStencilStateCreateInfo.front.compareMask = depthStencilState.stencilOpStateFront.compareMask;
	depthStencilStateCreateInfo.front.writeMask = depthStencilState.stencilOpStateFront.writeMask;
    depthStencilStateCreateInfo.front.reference = 0;
    depthStencilStateCreateInfo.back.failOp = toVkStencilOp(depthStencilState.stencilOpStateBack.failOp);
    depthStencilStateCreateInfo.back.passOp = toVkStencilOp(depthStencilState.stencilOpStateBack.passOp);
    depthStencilStateCreateInfo.back.depthFailOp = toVkStencilOp(depthStencilState.stencilOpStateBack.depthFailOp);
    depthStencilStateCreateInfo.back.compareOp = toVkCompareOp(depthStencilState.stencilOpStateBack.compareFunc);
    depthStencilStateCreateInfo.back.compareMask = depthStencilState.stencilOpStateBack.compareMask;
	depthStencilStateCreateInfo.back.writeMask = depthStencilState.stencilOpStateBack.writeMask;
    depthStencilStateCreateInfo.back.reference = 0;
    depthStencilStateCreateInfo.minDepthBounds = 0.0f;
    depthStencilStateCreateInfo.maxDepthBounds = 0.0f;

	// ColorBlendState
	VkPipelineColorBlendAttachmentState colorBlendAttachmentStateCreateInfo = {};
	colorBlendAttachmentStateCreateInfo.blendEnable = colorBlendState.blendEnabled ? VK_TRUE : VK_FALSE;
    colorBlendAttachmentStateCreateInfo.colorWriteMask = colorBlendState.colorWriteMask;
    colorBlendAttachmentStateCreateInfo.alphaBlendOp = toVkBlendOp(colorBlendState.alphaBlendOp);
	colorBlendAttachmentStateCreateInfo.colorBlendOp = toVkBlendOp(colorBlendState.colorBlendOp);
	colorBlendAttachmentStateCreateInfo.srcColorBlendFactor = toVkBlendFactor(colorBlendState.colorBlendSrc);
    colorBlendAttachmentStateCreateInfo.dstColorBlendFactor = toVkBlendFactor(colorBlendState.colorBlendDst);
    colorBlendAttachmentStateCreateInfo.srcAlphaBlendFactor = toVkBlendFactor(colorBlendState.alphaBlendSrc);
    colorBlendAttachmentStateCreateInfo.dstAlphaBlendFactor = toVkBlendFactor(colorBlendState.alphaBlendSrc);

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pNext = nullptr;
    colorBlendStateCreateInfo.flags = 0;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendStateCreateInfo.attachmentCount = renderPass->getColorAttachmentCount();
    colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentStateCreateInfo;
    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	// DynamicState
    VkDynamicState dynamicStates[9];
    dynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;
    dynamicStates[2] = VK_DYNAMIC_STATE_LINE_WIDTH;
    dynamicStates[3] = VK_DYNAMIC_STATE_DEPTH_BIAS;
    dynamicStates[4] = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
    dynamicStates[5] = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
    dynamicStates[6] = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
    dynamicStates[7] = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
    dynamicStates[8] = VK_DYNAMIC_STATE_STENCIL_REFERENCE;

	VkPipelineDynamicStateCreateInfo dynamicStatesCreateInfo = {};
    dynamicStatesCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStatesCreateInfo.pNext = nullptr;
    dynamicStatesCreateInfo.flags = 0;
    dynamicStatesCreateInfo.dynamicStateCount = 9;
    dynamicStatesCreateInfo.pDynamicStates = dynamicStates;

	// Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = (descriptorSet != nullptr) ? 1 : 0;
    pipelineLayoutCreateInfo.pSetLayouts = (descriptorSet != nullptr) ? &(std::static_pointer_cast<DescriptorSetVK>(descriptorSet)->_descriptorSetLayout) : nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
    VK_CHECK_RESULT(vkCreatePipelineLayout(_device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
 

	// Pipeline
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stageCount = stagesCreateInfo.size();
    pipelineCreateInfo.pStages = stagesCreateInfo.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    pipelineCreateInfo.pTessellationState = &tessStateCreateInfo;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizerStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState = &dynamicStatesCreateInfo;
    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.renderPass = std::static_pointer_cast<RenderPassVK>(renderPass)->_renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

	VkPipeline pipeline;
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline));

	std::shared_ptr<RenderPipelineVK> renderPipeline = std::make_shared<RenderPipelineVK>(primitiveTopology,
																						  vertexLayout,
																						  rasterizerState,
																						  colorBlendState,
																						  depthStencilState,
																						  renderPass,
																						  descriptorSet,
																						  vertShader,
																						  tescShader,
																						  teseShader,
																						  geomShader,
																						  fragShader,
																						  _device,
																						  pipelineLayout,
																						  pipeline);	
	return std::static_pointer_cast<RenderPipeline>(renderPipeline);

}

void GraphicsVK::destroyRenderPipeline(std::shared_ptr<RenderPipeline> pipeline)
{
}

Format GraphicsVK::toFormat(VkFormat pixelFormat)
{
	Format result = Format::FORMAT_UNDEFINED;	
	switch (pixelFormat) 
	{
    case VK_FORMAT_R8_UNORM:
		result = Format::FORMAT_R8_UNORM; 
		break;
    case VK_FORMAT_R16_UNORM:
		result = Format::FORMAT_R16_UNORM;
		break;
    case VK_FORMAT_R16_SFLOAT:
		result = Format::FORMAT_R16_FLOAT;
		break;
    case VK_FORMAT_R32_UINT:
		result = Format::FORMAT_R32_UINT;
		break;
    case VK_FORMAT_R32_SFLOAT:
		result = Format::FORMAT_R32_FLOAT;
		break;        
    case VK_FORMAT_R8G8_UNORM:
		result = Format::FORMAT_R8G8_UNORM;
		break;
    case VK_FORMAT_R16G16_UNORM:
		result = Format::FORMAT_R16G16_UNORM;
		break;
    case VK_FORMAT_R16G16_SFLOAT:
		result = Format::FORMAT_R16G16_FLOAT;
		break;
    case VK_FORMAT_R32G32_UINT:
		result = Format::FORMAT_R32G32_UINT;
		break;
    case VK_FORMAT_R32G32_SFLOAT:
		result = Format::FORMAT_R32G32_FLOAT;
		break;
    case VK_FORMAT_R8G8B8_UNORM:
		result = Format::FORMAT_R8G8B8_UNORM;
		break;
    case VK_FORMAT_R16G16B16_UNORM:
		result = Format::FORMAT_R16G16B16_UNORM;
		break;
    case VK_FORMAT_R16G16B16_SFLOAT:
		result = Format::FORMAT_R16G16B16_FLOAT;
		break;
    case VK_FORMAT_R32G32B32_UINT:
		result = Format::FORMAT_R32G32B32_UINT;
		break;
    case VK_FORMAT_R32G32B32_SFLOAT:
		result = Format::FORMAT_R32G32B32_FLOAT;
		break;
    case VK_FORMAT_B8G8R8A8_UNORM:
		result = Format::FORMAT_B8G8R8A8_UNORM;
		break;
    case VK_FORMAT_R8G8B8A8_UNORM:
		result = Format::FORMAT_R8G8B8A8_UNORM;
		break;
    case VK_FORMAT_R16G16B16A16_UNORM:
		result = Format::FORMAT_R16G16B16A16_UNORM;
		break;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
		result = Format::FORMAT_R16G16B16A16_FLOAT;
		break;
    case VK_FORMAT_R32G32B32A32_UINT:
		result = Format::FORMAT_R32G32B32A32_UINT;
		break;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
		result = Format::FORMAT_R32G32B32A32_FLOAT;
		break;    
    case VK_FORMAT_D16_UNORM:
		result = Format::FORMAT_D16_UNORM;
		break;
    case VK_FORMAT_X8_D24_UNORM_PACK32:
		result = Format::FORMAT_X8_D24_UNORM_PACK32;
		break;
    case VK_FORMAT_D32_SFLOAT:
		result = Format::FORMAT_D32_FLOAT;
		break;
    case VK_FORMAT_S8_UINT:
		result = Format::FORMAT_S8_UINT;
		break;
    case VK_FORMAT_D16_UNORM_S8_UINT:
		result = Format::FORMAT_D16_UNORM_S8_UINT;
		break;
    case VK_FORMAT_D24_UNORM_S8_UINT:
		result = Format::FORMAT_D24_UNORM_S8_UINT;
		break;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
		result = Format::FORMAT_D32_FLOAT_S8_UINT;
		break;
    }
    return result;
}

Texture::SampleCount GraphicsVK::toSamples(VkSampleCountFlagBits sampleCount)
{
	Texture::SampleCount result = Texture:: SAMPLE_COUNT_1X;
    switch (sampleCount) 
	{
	case VK_SAMPLE_COUNT_1_BIT:
		result = Texture:: SAMPLE_COUNT_1X;  
			break;
    case VK_SAMPLE_COUNT_2_BIT:
		result = Texture:: SAMPLE_COUNT_2X;  
		break;
    case VK_SAMPLE_COUNT_4_BIT:
		result = Texture:: SAMPLE_COUNT_4X;  
		break;
    case VK_SAMPLE_COUNT_8_BIT:
		result = Texture:: SAMPLE_COUNT_8X;  
		break;
    case VK_SAMPLE_COUNT_16_BIT:
		result = Texture:: SAMPLE_COUNT_16X; 
		break;
    }
    return result;
}

VkFormat GraphicsVK::toVkFormat(Format pixelFormat)
{
	VkFormat result = VK_FORMAT_UNDEFINED;	
	switch (pixelFormat) 
	{
    case Format::FORMAT_R8_UNORM: 
		result = VK_FORMAT_R8_UNORM; 
		break;
    case Format::FORMAT_R16_UNORM:
		result = VK_FORMAT_R16_UNORM;
		break;
    case Format::FORMAT_R16_FLOAT:
		result = VK_FORMAT_R16_SFLOAT;
		break;
    case Format::FORMAT_R32_UINT: 
		result = VK_FORMAT_R32_UINT;
		break;
    case Format::FORMAT_R32_FLOAT:
		result = VK_FORMAT_R32_SFLOAT;
		break;        
    case Format::FORMAT_R8G8_UNORM:
		result = VK_FORMAT_R8G8_UNORM;
		break;
    case Format::FORMAT_R16G16_UNORM:
		result = VK_FORMAT_R16G16_UNORM;
		break;
    case Format::FORMAT_R16G16_FLOAT:
		result = VK_FORMAT_R16G16_SFLOAT;
		break;
    case Format::FORMAT_R32G32_UINT:
		result = VK_FORMAT_R32G32_UINT;
		break;
    case Format::FORMAT_R32G32_FLOAT:
		result = VK_FORMAT_R32G32_SFLOAT;
		break;
    case Format::FORMAT_R8G8B8_UNORM:
		result = VK_FORMAT_R8G8B8_UNORM;
		break;
    case Format::FORMAT_R16G16B16_UNORM:
		result = VK_FORMAT_R16G16B16_UNORM;
		break;
    case Format::FORMAT_R16G16B16_FLOAT:
		result = VK_FORMAT_R16G16B16_SFLOAT;
		break;
    case Format::FORMAT_R32G32B32_UINT:
		result = VK_FORMAT_R32G32B32_UINT;
		break;
    case Format::FORMAT_R32G32B32_FLOAT:
		result = VK_FORMAT_R32G32B32_SFLOAT;
		break;
    case Format::FORMAT_B8G8R8A8_UNORM:
		result = VK_FORMAT_B8G8R8A8_UNORM;
		break;
    case Format::FORMAT_R8G8B8A8_UNORM:
		result = VK_FORMAT_R8G8B8A8_UNORM;
		break;
    case Format::FORMAT_R16G16B16A16_UNORM:
		result = VK_FORMAT_R16G16B16A16_UNORM;
		break;
    case Format::FORMAT_R16G16B16A16_FLOAT:
		result = VK_FORMAT_R16G16B16A16_SFLOAT;
		break;
    case Format::FORMAT_R32G32B32A32_UINT:
		result = VK_FORMAT_R32G32B32A32_UINT;
		break;
    case Format::FORMAT_R32G32B32A32_FLOAT:
		result = VK_FORMAT_R32G32B32A32_SFLOAT;
		break;    
    case Format::FORMAT_D16_UNORM:
		result = VK_FORMAT_D16_UNORM;
		break;
    case Format::FORMAT_X8_D24_UNORM_PACK32:
		result = VK_FORMAT_X8_D24_UNORM_PACK32;
		break;
    case Format::FORMAT_D32_FLOAT:
		result = VK_FORMAT_D32_SFLOAT;
		break;
    case Format::FORMAT_S8_UINT:
		result = VK_FORMAT_S8_UINT;
		break;
    case Format::FORMAT_D16_UNORM_S8_UINT:
		result = VK_FORMAT_D16_UNORM_S8_UINT;
		break;
    case Format::FORMAT_D24_UNORM_S8_UINT:
		result = VK_FORMAT_D24_UNORM_S8_UINT;
		break;
    case Format::FORMAT_D32_FLOAT_S8_UINT:
		result = VK_FORMAT_D32_SFLOAT_S8_UINT;
		break;
    }
    return result;
}

VkSampleCountFlagBits GraphicsVK::toVkSamples(Texture::SampleCount sampleCount)
{
	VkSampleCountFlagBits result = VK_SAMPLE_COUNT_1_BIT;
    switch (sampleCount) 
	{
	case Texture:: SAMPLE_COUNT_1X:
		result = VK_SAMPLE_COUNT_1_BIT;  
			break;
    case Texture:: SAMPLE_COUNT_2X:
		result = VK_SAMPLE_COUNT_2_BIT;  
		break;
    case Texture:: SAMPLE_COUNT_4X:
		result = VK_SAMPLE_COUNT_4_BIT;  
		break;
    case Texture:: SAMPLE_COUNT_8X:
		result = VK_SAMPLE_COUNT_8_BIT;  
		break;
    case Texture:: SAMPLE_COUNT_16X:
		result = VK_SAMPLE_COUNT_16_BIT; 
		break;
    }
    return result;
}

VkImageUsageFlags GraphicsVK::toVkImageUsageFlags(Texture::Usage usage)
{
	 VkImageUsageFlags result = 0;
    if (Texture::USAGE_TRANSFER_SRC == (usage & Texture::USAGE_TRANSFER_SRC))
	{
        result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (Texture::USAGE_TRANSFER_DST == (usage & Texture::USAGE_TRANSFER_DST))
	{
        result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if (Texture::USAGE_SAMPLED_IMAGE == (usage & Texture::USAGE_SAMPLED_IMAGE))
	{
        result |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (Texture::USAGE_STORAGE == (usage & Texture::USAGE_STORAGE))
	{
        result |= VK_IMAGE_USAGE_STORAGE_BIT;
    }
    if (Texture::USAGE_COLOR_ATTACHMENT == (usage & Texture::USAGE_COLOR_ATTACHMENT))
	{
        result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (Texture::USAGE_DEPTH_STENCIL_ATTACHMENT == (usage & Texture::USAGE_DEPTH_STENCIL_ATTACHMENT))
	{
        result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    return result;
}

VkFormatFeatureFlags GraphicsVK::toVkFormatFeatureFlags(VkImageUsageFlags usage)
{
	VkFormatFeatureFlags result = (VkFormatFeatureFlags)0;
    if (VK_IMAGE_USAGE_SAMPLED_BIT == (usage & VK_IMAGE_USAGE_SAMPLED_BIT)) 
	{
        result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    }
    if (VK_IMAGE_USAGE_STORAGE_BIT == (usage & VK_IMAGE_USAGE_STORAGE_BIT)) {
        result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    }
    if (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT == (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)) {
        result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    }
    if (VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT == (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)) {
        result |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    return result;
}

VkImageAspectFlags GraphicsVK::toVkImageAspectFlags(VkFormat format)
{
    VkImageAspectFlags result = 0;
    switch (format) 
	{        
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D32_SFLOAT:
        result = VK_IMAGE_ASPECT_DEPTH_BIT;
        break;        
    case VK_FORMAT_S8_UINT:
        result = VK_IMAGE_ASPECT_STENCIL_BIT;
        break;        
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        result = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        break;
    default:
        result = VK_IMAGE_ASPECT_COLOR_BIT;
        break;
    }
    return result;
}

VkImageLayout GraphicsVK::toVkImageLayout(Texture::Usage usage)
{
    VkImageLayout result = VK_IMAGE_LAYOUT_UNDEFINED;
    switch (usage) 
	{
	case Texture::USAGE_TRANSFER_SRC: 
		result = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL; 
		break;
    case Texture::USAGE_TRANSFER_DST: 
		result = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; 
		break;
	case Texture::USAGE_SAMPLED_IMAGE: 
		result = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; 
		break;
	case Texture::USAGE_STORAGE: 
		result = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; 
		break;
	case Texture::USAGE_COLOR_ATTACHMENT: 
		result = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
		break;
	case Texture::USAGE_DEPTH_STENCIL_ATTACHMENT: 
		result = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 
		break;
	case Texture::USAGE_PRESENT: 
		result = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
		break;
    }
    return result;
}

VkFilter GraphicsVK::toVkFilter(Sampler::Filter filter)
{
	VkFilter result = VK_FILTER_LINEAR;
	switch (filter)
	{
	case Sampler::FILTER_LINEAR:
		result = VK_FILTER_LINEAR;
		break;
	case Sampler::FILTER_NEAREST:
		result = VK_FILTER_NEAREST;
		break;
	}
	return result;
}

VkSamplerMipmapMode GraphicsVK::toVkSamplerMipmapMode(Sampler::Filter filter)
{
	VkSamplerMipmapMode result = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	switch (filter)
	{
	case Sampler::FILTER_LINEAR:
		result = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		break;
	case Sampler::FILTER_NEAREST:
		result = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		break;
	}
	return result;
}

VkSamplerAddressMode GraphicsVK::toVkSamplerAddressMode(Sampler::AddressMode addressMode)
{
	VkSamplerAddressMode result = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	switch (addressMode)
	{
	case Sampler::ADDRESS_MODE_WRAP:
		result = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		break;
	case Sampler::ADDRESS_MODE_MIRROR:
		result = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		break;
	case Sampler::ADDRESS_MODE_CLAMP_EDGE:
		result = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
		break;
	case Sampler::ADDRESS_MODE_CLAMP_BORDER:
		result = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		break;
	case Sampler::ADDRESS_MODE_MIRROR_ONCE:
		result = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
		break;
	}
	return result;
}

VkBorderColor GraphicsVK::toVkBorderColor(Sampler::BorderColor borderColor)
{
	VkBorderColor result = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	switch (borderColor)
	{
	case Sampler::BORDER_COLOR_BLACK_TRANSPARENT:
		result = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		break;
	case Sampler::BORDER_COLOR_BLACK_OPAQUE:
		result = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		break;
	case Sampler::BORDER_COLOR_WHITE_OPAQUE:
		result = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			break;
	}
	return result;
}

VkCompareOp GraphicsVK::toVkCompareOp(Sampler::CompareFunc compareFunc)
{
	VkCompareOp result = VK_COMPARE_OP_NEVER;
	switch (compareFunc)
	{
	case Sampler::COMPARE_FUNC_NEVER:
		result = VK_COMPARE_OP_NEVER;
		break;
	case Sampler::COMPARE_FUNC_LESS:
		result = VK_COMPARE_OP_LESS;
		break;
	case Sampler::COMPARE_FUNC_EQUAL:
		result = VK_COMPARE_OP_EQUAL;
			break;
	case Sampler::COMPARE_FUNC_LESS_OR_EQUAL:
		result = VK_COMPARE_OP_LESS_OR_EQUAL;
		break;
	case Sampler::COMPARE_FUNC_GREATER:
		result = VK_COMPARE_OP_GREATER;
		break;
	case Sampler::COMPARE_FUNC_NOT_EQUAL:
		result = VK_COMPARE_OP_NOT_EQUAL;
			break;
	case Sampler::COMPARE_FUNC_GREATER_OR_EQUAL:
		result = VK_COMPARE_OP_GREATER_OR_EQUAL;
		break;
	case Sampler::COMPARE_FUNC_ALWAYS:
		result = VK_COMPARE_OP_ALWAYS;
			break;
	}
	return result;
}

VkShaderStageFlags GraphicsVK::toVkShaderStageFlags(DescriptorSet::Descriptor::ShaderStage shaderStage)
{
	 VkShaderStageFlags result = VK_IMAGE_LAYOUT_UNDEFINED;
	 switch (shaderStage)
	 {
	 case DescriptorSet::Descriptor::SHADER_STAGE_VERT:
		 result = VK_SHADER_STAGE_VERTEX_BIT;
		 break;
	 case DescriptorSet::Descriptor::SHADER_STAGE_TESE:
		 result = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		 break;
	 case DescriptorSet::Descriptor::SHADER_STAGE_TESC:
		 result = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		 break;
	 case DescriptorSet::Descriptor::SHADER_STAGE_GEOM:
		 result = VK_SHADER_STAGE_GEOMETRY_BIT;
		 break;
	 case DescriptorSet::Descriptor::SHADER_STAGE_FRAG:
		 result = VK_SHADER_STAGE_FRAGMENT_BIT;
		 break;
	 }
	 return result;
}

VkPolygonMode GraphicsVK::toVkPolygonMode(RasterizerState::PolygonMode polygonMode)
{
	VkPolygonMode result = VK_POLYGON_MODE_FILL;
	switch (polygonMode)
	{
	case  RasterizerState::POLYGON_MODE_FILL:
		result = VK_POLYGON_MODE_FILL;
		break;
	case RasterizerState::POLYGON_MODE_WIREFRAME:
		result = VK_POLYGON_MODE_LINE;
		break;
	}
	return result;
}

VkCullModeFlags GraphicsVK::toVkCullModeFlags(RasterizerState::CullMode cullMode)
{
	VkCullModeFlags result = VK_CULL_MODE_NONE;
	switch (cullMode)
	{
	case RasterizerState::CULL_MODE_NONE:
		result = VK_CULL_MODE_NONE;
		break;
	case RasterizerState::CULL_MODE_BACK:
		result = VK_CULL_MODE_BACK_BIT;
		break;
	case RasterizerState::CULL_MODE_FRONT:
		result = VK_CULL_MODE_BACK_BIT;
		break;
	case RasterizerState::CULL_MODE_FRONT_AND_BACK:
		result = VK_CULL_MODE_FRONT_AND_BACK;
		break;
	}
	return result;
}


VkFrontFace GraphicsVK::toVkFrontFace(RasterizerState::FrontFace frontFace)

{
	VkFrontFace result = VK_FRONT_FACE_CLOCKWISE;
	switch (frontFace)
	{
	case RasterizerState::FRONT_FACE_CW:
		result = VK_FRONT_FACE_CLOCKWISE;
		break;
	case RasterizerState::FRONT_FACE_CCW:
		result = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	}
	return result;
}

VkCompareOp GraphicsVK::toVkCompareOp(DepthStencilState::CompareFunc compareFunc)
{
	VkCompareOp result = VK_COMPARE_OP_NEVER;
	switch (compareFunc)
	{
	case DepthStencilState::COMPARE_FUNC_NEVER:
		result = VK_COMPARE_OP_NEVER;
		break;
	case DepthStencilState::COMPARE_FUNC_LESS:
		result = VK_COMPARE_OP_LESS;
		break;
	case DepthStencilState::COMPARE_FUNC_EQUAL:
		result = VK_COMPARE_OP_EQUAL;
		break;
	case DepthStencilState::COMPARE_FUNC_LESS_OR_EQUAL:
		result = VK_COMPARE_OP_LESS_OR_EQUAL;
		break;
	case DepthStencilState::COMPARE_FUNC_GREATER:
		result = VK_COMPARE_OP_GREATER;
		break;
	case DepthStencilState::COMPARE_FUNC_GREATER_OR_EQUAL:
		result = VK_COMPARE_OP_GREATER_OR_EQUAL;
		break;
	case DepthStencilState::COMPARE_FUNC_ALWAYS:
		result = VK_COMPARE_OP_ALWAYS;
		break;
	}
	return result;
}

VkStencilOp GraphicsVK::toVkStencilOp(DepthStencilState::StencilOp stencilOp)
{
	VkStencilOp result = VK_STENCIL_OP_ZERO;
	switch (stencilOp)
	{
	case DepthStencilState::STENCIL_OP_ZERO:
		result = VK_STENCIL_OP_ZERO;
		break;
	case DepthStencilState::STENCIL_OP_REPLACE:
		result = VK_STENCIL_OP_REPLACE;
		break;
	case DepthStencilState::STENCIL_OP_INCREMENT_AND_CLAMP:
		result = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		break;
	case DepthStencilState::STENCIL_OP_DECREMENT_AND_CLAMP:
		result = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		break;
	case DepthStencilState::STENCIL_OP_INVERT:
		result = VK_STENCIL_OP_INVERT;
		break;
	case DepthStencilState::STENCIL_OP_INCREMENT_AND_WRAP:
		result = VK_STENCIL_OP_INCREMENT_AND_WRAP;
		break;
	case DepthStencilState::STENCIL_OP_DECREMENT_AND_WRAP:
		result = VK_STENCIL_OP_DECREMENT_AND_WRAP;
		break;
	}
	return result;
}

VkBlendOp GraphicsVK::toVkBlendOp(ColorBlendState::BlendOp blendOp)
{
	VkBlendOp result = VK_BLEND_OP_ADD;
	switch (blendOp)
	{
	case ColorBlendState::BLEND_OP_ADD:
		result = VK_BLEND_OP_ADD;
		break;
	case ColorBlendState::BLEND_OP_SUBSTRACT:
		result = VK_BLEND_OP_SUBTRACT;
		break;
	case ColorBlendState::BLEND_OP_REVERSE_SUBTRACT:
		result = VK_BLEND_OP_REVERSE_SUBTRACT;
		break;
	case ColorBlendState::BLEND_OP_MIN:
		result = VK_BLEND_OP_MIN;
		break;
	case ColorBlendState::BLEND_OP_MAX:
		result = VK_BLEND_OP_MAX;
		break;
	}

	return result;
}

VkBlendFactor GraphicsVK::toVkBlendFactor(ColorBlendState::BlendFactor blendFactor)
{
	VkBlendFactor result = VK_BLEND_FACTOR_ZERO;
	switch (blendFactor)
	{
	case ColorBlendState::BLEND_FACTOR_ZERO:
		result = VK_BLEND_FACTOR_ZERO;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE:
		result = VK_BLEND_FACTOR_ONE;
		break;
	case ColorBlendState::BLEND_FACTOR_SRC_COLOR:
		result = VK_BLEND_FACTOR_SRC_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
		result = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_DST_COLOR:
		result = VK_BLEND_FACTOR_DST_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_DST_COLOR:
		result = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_SRC_ALPHA:
		result = VK_BLEND_FACTOR_SRC_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_DST_ALPHA:
		result = VK_BLEND_FACTOR_DST_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
		result = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_CONSTANT_COLOR:
		result = VK_BLEND_FACTOR_CONSTANT_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR:
		result = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_CONSTANT_ALPHA:
		result = VK_BLEND_FACTOR_CONSTANT_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA:
		result = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_SRC_ALPHA_SATURATE:
		result = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		break;
	case ColorBlendState::BLEND_FACTOR_SRC1_COLOR:
		result = VK_BLEND_FACTOR_SRC1_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
		result = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		break;
	case ColorBlendState::BLEND_FACTOR_SRC1_ALPHA:
		result = VK_BLEND_FACTOR_SRC1_ALPHA;
		break;
	case ColorBlendState::BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
		result = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
		break;
	}
	return result;
}

void GraphicsVK::createInstance()
{
	loadLibrary();

	// Create app info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = GP_ENGINE_NAME;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = GP_ENGINE_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION(GP_ENGINE_VERSION_MAJOR, GP_ENGINE_VERSION_MINOR, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Add extensions
	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WINDOWS)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__ANDROID__)
	extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
	extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

	// Create instance
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	if (extensions.size() > 0)
	{
		if (_validation)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}
		instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
	}
	if (_validation)
	{
        instanceCreateInfo.enabledLayerCount = (uint32_t)__validationLayers.size();
		instanceCreateInfo.ppEnabledLayerNames = __validationLayers.data();
	}
	VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &_instance));

	loadFunctions();

	// Register validation debug report
	if (_validation)
	{
		// Get the validation debug report extensions
		vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(_instance, "vkCreateDebugReportCallbackEXT"));
		vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(_instance, "vkDestroyDebugReportCallbackEXT"));
		vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(_instance, "vkDebugReportMessageEXT"));

		// Register the debug report callback
		VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
		dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)GraphicsVK::validationDebugReport;
		dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

		VK_CHECK_RESULT(vkCreateDebugReportCallbackEXT(_instance, &dbgCreateInfo, nullptr, &_debugMessageCallback));
	}
}

void GraphicsVK::createDevice()
{
	// Get the available physical devices
	uint32_t gpuCount = 0;
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(_instance, &gpuCount, nullptr));
	GP_ASSERT(gpuCount > 0);

	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(_instance, &gpuCount, physicalDevices.data()));

	// Rate the physical devices based on  important properties and features
	std::map<int, VkPhysicalDevice> physicalDevicesRated;
	for (const auto& physicalDevice : physicalDevices)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);
		int score = 0;
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
			score += 1000;
		score += properties.limits.maxImageDimension2D;
		if (!features.geometryShader)
			score = 0;
		physicalDevicesRated[score] = physicalDevice;
	}

	// Take the first device from rated devices that support our queue requirements
	for (const auto& physicalDeviceRated : physicalDevicesRated)
	{
		VkPhysicalDevice physicalDevice = physicalDeviceRated.second;
		_physicalDevice = physicalDevice;
		break;
	}
	// Get properties various properties of the physical device
	vkGetPhysicalDeviceProperties(_physicalDevice, &_physicalDeviceProperties);
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_physicalDeviceMemoryProperties);
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
	GP_ASSERT(queueFamilyCount > 0);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, _queueFamilyProperties.data());

	// Get queue create infos for queues
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	// Graphics queue create info
	const float defaultQueuePriority(0.0f);
	_queueFamilyIndexGraphics = getQueueFamiliyIndex(VK_QUEUE_GRAPHICS_BIT);
	VkDeviceQueueCreateInfo queueInfo{};
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.queueFamilyIndex = _queueFamilyIndexGraphics;
	queueInfo.queueCount = 1;
	queueInfo.pQueuePriorities = &defaultQueuePriority;
	queueCreateInfos.push_back(queueInfo);

	// Compute queue create info (if different) and requested
	_queueFamilyIndexCompute = getQueueFamiliyIndex(VK_QUEUE_COMPUTE_BIT);
	if (_queueFamilyIndexCompute != _queueFamilyIndexGraphics)
	{
		float queuePriority(0.0f);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = _queueFamilyIndexCompute;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	// Add swapchain extensions
	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	// Create the logical device
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	
	// Add validation layers/extensions
	if (_validation && isDeviceExtensionPresent(_physicalDevice, VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
	if (extensions.size() > 0)
	{
		deviceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	}
	else
	{
		deviceCreateInfo.enabledExtensionCount = 0;
	}
	if (_validation)
	{
		deviceCreateInfo.enabledLayerCount = __validationLayers.size();
		deviceCreateInfo.ppEnabledLayerNames = __validationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	// Create device
	VK_CHECK_RESULT(vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_device));

	// Get the queue
	vkGetDeviceQueue(_device, _queueFamilyIndexGraphics, 0, &_queue);
}

void GraphicsVK::createSurface(unsigned long window, unsigned long connection)
{
	// Lookup device surface extensions
	vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
	vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
	vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));

	// Create the platform surface connection
    VkResult result;
#ifdef _WINDOWS
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)connection;
	surfaceCreateInfo.hwnd = (HWND)window;
    result = vkCreateWin32SurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
#else
#ifdef __ANDROID__
	VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.window = (ANativeWindow*)window;
    result = vkCreateAndroidSurfaceKHR(_instance, &surfaceCreateInfo, NULL, &_surface);
#elif defined(__linux__)
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = (xcb_connection_t*)connection;
	surfaceCreateInfo.window = (xcb_window_t)window;
    result = vkCreateXcbSurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
#endif
#endif
	if (result != VK_SUCCESS)
		GP_ERROR("Failed to create surface.");

	// Find a device with properties to support presenting swapchain images
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueCount, NULL);
	GP_ASSERT(queueCount > 0);
	std::vector<VkQueueFamilyProperties> queueProps(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueCount, queueProps.data());
	std::vector<VkBool32> supportsPresent(queueCount);
	for (uint32_t i = 0; i < queueCount; i++) 
		vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _surface, &supportsPresent[i]);

	// Find a queue that supports both graphics and presenting
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < queueCount; i++) 
	{
		if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) 
		{
			if (graphicsQueueNodeIndex == UINT32_MAX) 
				graphicsQueueNodeIndex = i;
			if (supportsPresent[i] == VK_TRUE) 
			{
				graphicsQueueNodeIndex = i;
				presentQueueNodeIndex = i;
				break;
			}
		}
	}
	if (presentQueueNodeIndex == UINT32_MAX) 
	{	
		for (uint32_t i = 0; i < queueCount; ++i) 
		{
			if (supportsPresent[i] == VK_TRUE) 
			{
				presentQueueNodeIndex = i;
				break;
			}
		}
	}
	if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX) 
		GP_ERROR("Failed to find a graphics and/or presenting queue.");
	if (graphicsQueueNodeIndex != presentQueueNodeIndex) 
		GP_ERROR("Separate graphics and presenting queues are not supported yet!");
	_queueIndex = graphicsQueueNodeIndex;

	// Get list of supported surface formats
	uint32_t surfaceFormatCount;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, NULL) != VK_SUCCESS || surfaceFormatCount <= 0)
		GP_ERROR("Failed to find device surface formats.");
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, surfaceFormats.data()) != VK_SUCCESS)
		GP_ERROR("Failed to find device surface formats.");

	// Get color format
	if ((surfaceFormatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		_colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
	else
		_colorFormat = surfaceFormats[0].format;

	// Get color space
	_colorSpace = surfaceFormats[0].colorSpace;
}

void GraphicsVK::createSwapchain()
{
	vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(_device, "vkCreateSwapchainKHR"));
	vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetDeviceProcAddr(_device, "vkDestroySwapchainKHR"));
	vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetDeviceProcAddr(_device, "vkGetSwapchainImagesKHR"));
	vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetDeviceProcAddr(_device, "vkAcquireNextImageKHR"));
	vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetDeviceProcAddr(_device, "vkQueuePresentKHR"));

	// Query the swapchain surface info and choose surface format and present mode
	VkSwapchainKHR oldSwapchain = _swapchain;
	SwapchainSurfaceInfo surfaceInfo = querySwapchainSurfaceInfo(_physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(surfaceInfo.formats);
	VkPresentModeKHR presentMode = choosePresentMode(surfaceInfo.presentModes);

	// Request the swapchain backbuffer image count 
	_swapchainImageCount = surfaceInfo.capabilities.minImageCount;
	if (surfaceInfo.capabilities.maxImageCount > 0 && _swapchainImageCount > surfaceInfo.capabilities.maxImageCount) 
		_swapchainImageCount = surfaceInfo.capabilities.maxImageCount;

	// Find the transformation of the surface (prefer non-rotated)
	VkSurfaceTransformFlagsKHR preTransfer;
	if (surfaceInfo.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		preTransfer = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	else 
		preTransfer = surfaceInfo.capabilities.currentTransform;

	// Create the swapchain
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.surface = _surface;
	createInfo.minImageCount = _swapchainImageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = { _width, _height };
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.preTransform =  (VkSurfaceTransformFlagBitsKHR)preTransfer;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.oldSwapchain = oldSwapchain;

	// Get the color attachment format and properties
	VkFormatProperties formatProps;
	vkGetPhysicalDeviceFormatProperties(_physicalDevice, _colorFormat, &formatProps);
	if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)
		createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	// Create the swapchain
	VK_CHECK_RESULT(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain));

	// Get the backbuffer images for color attachments
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_swapchainImageCount, NULL));
	_swapchainImages.resize(_swapchainImageCount);
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_swapchainImageCount, _swapchainImages.data()));

	// Get the depth stencil format
	VkBool32 validDepthFormat = getDepthStencilFormat(_physicalDevice, &_depthStencilFormat);
	if (!validDepthFormat)
		GP_ERROR("Failed to find valid depth format.");

	// Create the render passes
	for (size_t i = 0; i < _swapchainImageCount; ++i)
	{

		std::vector<std::shared_ptr<Texture>> colorAttachments;
		std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;
		ClearValue clearColor;
		clearColor.color.red = 0;
		clearColor.color.green = 0;
		clearColor.color.blue = 0;
		clearColor.color.alpha = 0;
		std::shared_ptr<Texture> colorAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
																 toFormat(_colorFormat),
																 Texture::USAGE_COLOR_ATTACHMENT, 
																 Texture::SAMPLE_COUNT_1X, clearColor, false,
																 _swapchainImages[i]);
		colorAttachments.push_back(colorAttachment);
		ClearValue clearDepthStencil;
		clearDepthStencil.depthStencil.depth = 0;
		clearDepthStencil.depthStencil.stencil = 0;
		std::shared_ptr<Texture> depthStencilAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
																		toFormat(_depthStencilFormat),
																		Texture::USAGE_DEPTH_STENCIL_ATTACHMENT, 
																		Texture::SAMPLE_COUNT_1X, clearDepthStencil, false,
																		_swapchainImages[i]);

		std::shared_ptr<RenderPass> renderPass = createRenderPass(_width, _height, 1, 
																  _colorFormat, 
																  _depthStencilFormat,
																  VK_SAMPLE_COUNT_1_BIT,
																  colorAttachments, 
																  colorMultisampleAttachments,
																  depthStencilAttachment);
		_renderPasses.push_back(renderPass);
	}														
}

GraphicsVK::SwapchainSurfaceInfo GraphicsVK::querySwapchainSurfaceInfo(VkPhysicalDevice physicalDevice)
{
	// Get the capabilities of the swapchain
	SwapchainSurfaceInfo swapchainSurfaceInfo;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &swapchainSurfaceInfo.capabilities);

	// Get surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		swapchainSurfaceInfo.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, swapchainSurfaceInfo.formats.data());
	}

	// Get the presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) 
	{
		swapchainSurfaceInfo.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, swapchainSurfaceInfo.presentModes.data());
	}
	return swapchainSurfaceInfo;
}

VkSurfaceFormatKHR GraphicsVK::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
			return availableFormat;
	}
	return availableFormats[0];
}

VkPresentModeKHR GraphicsVK::choosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	if (!_vsync)
	{
		for (size_t i = 0; i < availablePresentModes.size(); i++)
		{
			if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (availablePresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
			{
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
	}
	return swapchainPresentMode;
}


VkBool32 GraphicsVK::getDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat* depthStencilFormat)
{
	std::vector<VkFormat> depthStencilFormats = 
	{
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};
	for (auto& format : depthStencilFormats)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			*depthStencilFormat = format;
			return true;
		}
	}
	return false;
}

VkBool32 GraphicsVK::isDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const char* extensionName)
{
	uint32_t extensionCount = 0;
	std::vector<VkExtensionProperties> extensions;
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
	extensions.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, extensions.data());
	for (auto& ext : extensions)
	{
		if (!strcmp(extensionName, ext.extensionName))
			return true;
	}
	return false;
}

VkBool32 GraphicsVK::getMemoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex)
{
    for (uint32_t i = 0; i < _physicalDeviceMemoryProperties.memoryTypeCount; i++) 
	{
        if ((typeBits & 1) == 1) 
		{
            if ((_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags &requirements_mask) == requirements_mask) 
			{
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

uint32_t GraphicsVK::getQueueFamiliyIndex(VkQueueFlagBits queueFlags)
{
	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++)
		{
			if ((_queueFamilyProperties[i].queueFlags & queueFlags) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;
				break;
			}
		}
	}
	for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++)
	{
		if (_queueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
			break;
		}
	}
	return 0;
}

VkBool32 GraphicsVK::validationDebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
										       uint64_t srcObject, size_t location, int32_t msgCode,
										       const char* layerPrefix, const char* msg, void* userData)
{
	std::string prefix("");
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		prefix += "ERROR:";
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		prefix += "WARNING:";
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		prefix += "PERFORMANCE:";
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
		prefix += "INFO:";
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
		prefix += "DEBUG:";
	GP_INFO("%s [%s] Code: %d:%s", prefix, layerPrefix, msgCode, msg);
	return VK_FALSE;
}

#if defined(__ANDROID__)
#include <android/log.h>
#include <dlfcn.h>

PFN_vkCreateInstance vkCreateInstance;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
PFN_vkCreateDevice vkCreateDevice;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
PFN_vkMapMemory vkMapMemory;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
PFN_vkBindBufferMemory vkBindBufferMemory;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkBindImageMemory vkBindImageMemory;
PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
PFN_vkCmdCopyImage vkCmdCopyImage;
PFN_vkCmdBlitImage vkCmdBlitImage;
PFN_vkCmdClearAttachments vkCmdClearAttachments;
PFN_vkCreateSampler vkCreateSampler;
PFN_vkDestroySampler vkDestroySampler;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkCreateRenderPass vkCreateRenderPass;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
PFN_vkCreateImage vkCreateImage;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkDestroyImageView vkDestroyImageView;
PFN_vkCreateSemaphore vkCreateSemaphore;
PFN_vkDestroySemaphore vkDestroySemaphore;
PFN_vkCreateFence vkCreateFence;
PFN_vkDestroyFence vkDestroyFence;
PFN_vkWaitForFences vkWaitForFences;
PFN_vkResetFences vkResetFences;
PFN_vkCreateCommandPool vkCreateCommandPool;
PFN_vkDestroyCommandPool vkDestroyCommandPool;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkGetDeviceQueue vkGetDeviceQueue;
PFN_vkQueueSubmit vkQueueSubmit;
PFN_vkQueueWaitIdle vkQueueWaitIdle;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
PFN_vkCreateFramebuffer vkCreateFramebuffer;
PFN_vkCreatePipelineCache vkCreatePipelineCache;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkCreateComputePipelines vkCreateComputePipelines;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
PFN_vkCmdBindPipeline vkCmdBindPipeline;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
PFN_vkCmdSetViewport vkCmdSetViewport;
PFN_vkCmdSetScissor vkCmdSetScissor;
PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
PFN_vkCmdPushConstants vkCmdPushConstants;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
PFN_vkCmdDraw vkCmdDraw;
PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
PFN_vkCmdDispatch vkCmdDispatch;
PFN_vkDestroyPipeline vkDestroyPipeline;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkDestroyRenderPass vkDestroyRenderPass;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
PFN_vkCreateQueryPool vkCreateQueryPool;
PFN_vkDestroyQueryPool vkDestroyQueryPool;
PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
PFN_vkCmdBeginQuery vkCmdBeginQuery;
PFN_vkCmdEndQuery vkCmdEndQuery;
PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
void* libVulkan;
#endif


void GraphicsVK::loadLibrary()
{
#if defined(__ANDROID__)
	__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Loading libvulkan.so...\n");

	// Load the vulkan library
	libVulkan = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
	if (!libVulkan)
	{
		__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Could not load vulkan library : %s!\n", dlerror());
		return false;
	}

	// Load base function pointers
	vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(dlsym(libVulkan, "vkEnumerateInstanceExtensionProperties"));
	vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(dlsym(libVulkan, "vkEnumerateInstanceLayerProperties"));
	vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(dlsym(libVulkan, "vkCreateInstance"));
	vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(libVulkan, "vkGetInstanceProcAddr"));
	vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(dlsym(libVulkan, "vkGetDeviceProcAddr"));
#elif defined(__linux__)
	//initxcbConnection();
#endif
}

void GraphicsVK::freeLibrary()
{
#if defined(__ANDROID__)
	dlclose(libVulkan);
#endif
}

void GraphicsVK::loadFunctions()
{
#if defined(__ANDROID__)
	__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Loading instance based function pointers...\n");
	vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices"));
	vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties"));
	vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(vkGetInstanceProcAddr(instance, "vkEnumerateDeviceLayerProperties"));
	vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties"));
	vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties"));
	vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures"));
	vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(vkGetInstanceProcAddr(instance, "vkCreateDevice"));
	vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties"));
	vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties"));
	vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(vkGetInstanceProcAddr(instance, "vkCmdPipelineBarrier"));
	vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(vkGetInstanceProcAddr(instance, "vkCreateShaderModule"));
	vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(vkGetInstanceProcAddr(instance, "vkCreateBuffer"));
	vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(vkGetInstanceProcAddr(instance, "vkGetBufferMemoryRequirements"));
	vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(vkGetInstanceProcAddr(instance, "vkMapMemory"));
	vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(vkGetInstanceProcAddr(instance, "vkUnmapMemory"));
	vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(vkGetInstanceProcAddr(instance, "vkFlushMappedMemoryRanges"));
	vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(vkGetInstanceProcAddr(instance, "vkInvalidateMappedMemoryRanges"));
	vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(vkGetInstanceProcAddr(instance, "vkBindBufferMemory"));
	vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(vkGetInstanceProcAddr(instance, "vkDestroyBuffer"));
	vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(vkGetInstanceProcAddr(instance, "vkAllocateMemory"));
	vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(vkGetInstanceProcAddr(instance, "vkFreeMemory"));
	vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(vkGetInstanceProcAddr(instance, "vkCreateRenderPass"));
	vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(vkGetInstanceProcAddr(instance, "vkCmdBeginRenderPass"));
	vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(vkGetInstanceProcAddr(instance, "vkCmdEndRenderPass"));
	vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(vkGetInstanceProcAddr(instance, "vkCmdExecuteCommands"));
	vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(vkGetInstanceProcAddr(instance, "vkCreateImage"));
	vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements"));
	vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(vkGetInstanceProcAddr(instance, "vkCreateImageView"));
	vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(vkGetInstanceProcAddr(instance, "vkDestroyImageView"));
	vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(vkGetInstanceProcAddr(instance, "vkBindImageMemory"));
	vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(vkGetInstanceProcAddr(instance, "vkGetImageSubresourceLayout"));
	vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(vkGetInstanceProcAddr(instance, "vkCmdCopyImage"));
	vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(vkGetInstanceProcAddr(instance, "vkCmdBlitImage"));
	vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(vkGetInstanceProcAddr(instance, "vkDestroyImage"));
	vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(vkGetInstanceProcAddr(instance, "vkCmdClearAttachments"));
	vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(vkGetInstanceProcAddr(instance, "vkCmdCopyBuffer"));
	vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(vkGetInstanceProcAddr(instance, "vkCmdCopyBufferToImage"));
	vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(vkGetInstanceProcAddr(instance, "vkCreateSampler"));
	vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(vkGetInstanceProcAddr(instance, "vkDestroySampler"));;
	vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(vkGetInstanceProcAddr(instance, "vkCreateSemaphore"));
	vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(vkGetInstanceProcAddr(instance, "vkDestroySemaphore"));
	vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(vkGetInstanceProcAddr(instance, "vkCreateFence"));
	vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(vkGetInstanceProcAddr(instance, "vkDestroyFence"));
	vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(vkGetInstanceProcAddr(instance, "vkWaitForFences"));
	vkResetFences = reinterpret_cast<PFN_vkResetFences>(vkGetInstanceProcAddr(instance, "vkResetFences"));;
	vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(vkGetInstanceProcAddr(instance, "vkCreateCommandPool"));
	vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(vkGetInstanceProcAddr(instance, "vkDestroyCommandPool"));;
	vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(vkGetInstanceProcAddr(instance, "vkAllocateCommandBuffers"));
	vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(vkGetInstanceProcAddr(instance, "vkBeginCommandBuffer"));
	vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(vkGetInstanceProcAddr(instance, "vkEndCommandBuffer"));
	vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(vkGetInstanceProcAddr(instance, "vkGetDeviceQueue"));
	vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(vkGetInstanceProcAddr(instance, "vkQueueSubmit"));
	vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(vkGetInstanceProcAddr(instance, "vkQueueWaitIdle"));
	vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(vkGetInstanceProcAddr(instance, "vkDeviceWaitIdle"));
	vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(vkGetInstanceProcAddr(instance, "vkCreateFramebuffer"));
	vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(vkGetInstanceProcAddr(instance, "vkCreatePipelineCache"));
	vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(vkGetInstanceProcAddr(instance, "vkCreatePipelineLayout"));
	vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(vkGetInstanceProcAddr(instance, "vkCreateGraphicsPipelines"));
	vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(vkGetInstanceProcAddr(instance, "vkCreateComputePipelines"));
	vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(vkGetInstanceProcAddr(instance, "vkCreateDescriptorPool"));
	vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(vkGetInstanceProcAddr(instance, "vkCreateDescriptorSetLayout"));
	vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(vkGetInstanceProcAddr(instance, "vkAllocateDescriptorSets"));
	vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(vkGetInstanceProcAddr(instance, "vkUpdateDescriptorSets"));
	vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(vkGetInstanceProcAddr(instance, "vkCmdBindDescriptorSets"));
	vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(vkGetInstanceProcAddr(instance, "vkCmdBindPipeline"));
	vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(vkGetInstanceProcAddr(instance, "vkCmdBindVertexBuffers"));
	vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(vkGetInstanceProcAddr(instance, "vkCmdBindIndexBuffer"));
	vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(vkGetInstanceProcAddr(instance, "vkCmdSetViewport"));
	vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(vkGetInstanceProcAddr(instance, "vkCmdSetScissor"));
	vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(vkGetInstanceProcAddr(instance, "vkCmdSetLineWidth"));
	vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(vkGetInstanceProcAddr(instance, "vkCmdSetDepthBias"));
	vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(vkGetInstanceProcAddr(instance, "vkCmdPushConstants"));;
	vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndexed"));
	vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(vkGetInstanceProcAddr(instance, "vkCmdDraw"));
	vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndexedIndirect"));
	vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndirect"));
	vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(vkGetInstanceProcAddr(instance, "vkCmdDispatch"));
	vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(vkGetInstanceProcAddr(instance, "vkDestroyPipeline"));
	vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(vkGetInstanceProcAddr(instance, "vkDestroyPipelineLayout"));;
	vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(vkGetInstanceProcAddr(instance, "vkDestroyDescriptorSetLayout"));
	vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(vkGetInstanceProcAddr(instance, "vkDestroyDevice"));
	vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(vkGetInstanceProcAddr(instance, "vkDestroyInstance"));
	vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(vkGetInstanceProcAddr(instance, "vkDestroyDescriptorPool"));
	vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(vkGetInstanceProcAddr(instance, "vkFreeCommandBuffers"));
	vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(vkGetInstanceProcAddr(instance, "vkDestroyRenderPass"));
	vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(vkGetInstanceProcAddr(instance, "vkDestroyFramebuffer"));
	vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(vkGetInstanceProcAddr(instance, "vkDestroyShaderModule"));
	vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(vkGetInstanceProcAddr(instance, "vkDestroyPipelineCache"));
	vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(vkGetInstanceProcAddr(instance, "vkCreateQueryPool"));
	vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(vkGetInstanceProcAddr(instance, "vkDestroyQueryPool"));
	vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(vkGetInstanceProcAddr(instance, "vkGetQueryPoolResults"));
	vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(vkGetInstanceProcAddr(instance, "vkCmdBeginQuery"));
	vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(vkGetInstanceProcAddr(instance, "vkCmdEndQuery"));
	vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(vkGetInstanceProcAddr(instance, "vkCmdResetQueryPool"));
	vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(vkGetInstanceProcAddr(instance, "vkCmdCopyQueryPoolResults"));
	vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateAndroidSurfaceKHR"));
	vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR"));
#endif
}

std::string getErrorString(VkResult result)
{
	switch (result)
	{
#define STR(r) case VK_ ##r: return #r
		STR(NOT_READY);
		STR(TIMEOUT);
		STR(EVENT_SET);
		STR(EVENT_RESET);
		STR(INCOMPLETE);
		STR(ERROR_OUT_OF_HOST_MEMORY);
		STR(ERROR_OUT_OF_DEVICE_MEMORY);
		STR(ERROR_INITIALIZATION_FAILED);
		STR(ERROR_DEVICE_LOST);
		STR(ERROR_MEMORY_MAP_FAILED);
		STR(ERROR_LAYER_NOT_PRESENT);
		STR(ERROR_EXTENSION_NOT_PRESENT);
		STR(ERROR_FEATURE_NOT_PRESENT);
		STR(ERROR_INCOMPATIBLE_DRIVER);
		STR(ERROR_TOO_MANY_OBJECTS);
		STR(ERROR_FORMAT_NOT_SUPPORTED);
		STR(ERROR_SURFACE_LOST_KHR);
		STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		STR(SUBOPTIMAL_KHR);
		STR(ERROR_OUT_OF_DATE_KHR);
		STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		STR(ERROR_VALIDATION_FAILED_EXT);
		STR(ERROR_INVALID_SHADER_NV);
#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}

}
