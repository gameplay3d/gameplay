#include "Base.h"
#include "Format.h"
#include "GraphicsVK.h"
#include "UtilsVK.h"
#include "BufferVK.h"
#include "TextureVK.h"
#include "RenderPassVK.h"
#include "SamplerVK.h"
#include "ShaderVK.h"
#include "DescriptorSetVK.h"
#include "RenderPipelineVK.h"
#include "CommandBufferVK.h"
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
	_commandPool(nullptr),
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

int GraphicsVK::getWidth()
{
    return _width;
}

int GraphicsVK::getHeight()
{
    return _height;
}

std::shared_ptr<RenderPass> GraphicsVK::acquireNextSwapchainImage()
{
	uint32_t frameIndex = Game::getInstance()->getFrameCount() % GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT;

	VkFence fence = _swapchainAcquiredFences[frameIndex];
	VkSemaphore semaphore = _swapchainAcquiredSemaphores[frameIndex];

	VK_CHECK_RESULT(vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, semaphore, fence, &_swapchainImageIndex));
	VK_CHECK_RESULT(vkWaitForFences(_device, 1, &fence, VK_TRUE, UINT64_MAX));
	VK_CHECK_RESULT(vkResetFences(_device, 1, &fence));

	return _renderPasses[_swapchainImageIndex];
}

void GraphicsVK::present()
{
	VkPresentInfoKHR presentInfo {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = _renderCompleteSemaphores.size();
	presentInfo.pWaitSemaphores = _renderCompleteSemaphores.data();
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &_swapchain;
    presentInfo.pImageIndices = &_swapchainImageIndex;
    presentInfo.pResults = nullptr;

	VK_CHECK_RESULT(vkQueuePresentKHR(_queue, &presentInfo));
}

void GraphicsVK::waitIdle()
{
	VK_CHECK_RESULT(vkQueueWaitIdle(_queue));
}

std::shared_ptr<CommandBuffer> GraphicsVK::beginCommands()
{
	GP_ASSERT(_commandBuffer == nullptr);
	_commandBuffer = _commandBuffers[_swapchainImageIndex];
	VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;;
    beginInfo.pNext = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

	VK_CHECK_RESULT(vkBeginCommandBuffer(std::static_pointer_cast<CommandBufferVK>(_commandBuffer)->_commandBuffer, &beginInfo));

	return _commandBuffer;
}

void GraphicsVK::endCommands()
{
	GP_ASSERT(_commandBuffer != nullptr);
	VK_CHECK_RESULT(vkEndCommandBuffer(std::static_pointer_cast<CommandBufferVK>(_commandBuffer)->_commandBuffer));
}

void GraphicsVK::submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
	VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = &waitMask;
	submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;

	VK_CHECK_RESULT(vkQueueSubmit(_queue, 1, &submitInfo, nullptr));
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
							   size_t x, size_t y, size_t width, size_t height)
{
	GP_ASSERT(commandBuffer);

	VkRect2D rect = {};
    rect.offset.x = x;
    rect.offset.x = y;
    rect.extent.width = width;
    rect.extent.height = height;
    vkCmdSetScissor(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 0, 1, &rect);
}

void GraphicsVK::cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
								    std::shared_ptr<RenderPass> renderPass)
{
	GP_ASSERT(commandBuffer);
	GP_ASSERT(renderPass);

	VkRect2D renderArea = {};
    renderArea.offset.x = 0;
    renderArea.offset.y = 0;
	renderArea.extent.width = renderPass->getWidth();
	renderArea.extent.height = renderPass->getHeight();

	std::shared_ptr<RenderPassVK> renderPassVK = std::static_pointer_cast<RenderPassVK>(renderPass);
	VkRenderPassBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = renderPassVK->_renderPass;
    beginInfo.framebuffer = renderPassVK->_framebuffer;
    beginInfo.renderArea = renderArea;
	beginInfo.clearValueCount = 0;
	beginInfo.pClearValues = nullptr;

	vkCmdBeginRenderPass(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void GraphicsVK::cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer)
{
	GP_ASSERT(commandBuffer);

	vkCmdEndRenderPass(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer);
}

void GraphicsVK::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
									   std::shared_ptr<RenderPipeline> renderPipeline)
{
	GP_ASSERT(commandBuffer);

	vkCmdBindPipeline(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
					 std::static_pointer_cast<RenderPipelineVK>(renderPipeline)->_pipeline);
}

void GraphicsVK::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::shared_ptr<RenderPipeline> renderPipeline, 
									  std::shared_ptr<DescriptorSet> descriptorSet)
{
	GP_ASSERT(commandBuffer);

	vkCmdBindDescriptorSets(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
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

	VkIndexType indexType = (indexBuffer->getStride() == sizeof(uint16_t)) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    
	vkCmdBindIndexBuffer(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 
					     std::static_pointer_cast<BufferVK>(indexBuffer)->_buffer, 0, indexType);
}

void GraphicsVK::cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
							   float red, float green, float blue, float alpha,
							   size_t attachmentTndex)
{
	GP_ASSERT(commandBuffer);
	VkClearAttachment attachment = {};
    attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    attachment.colorAttachment = attachmentTndex;
    attachment.clearValue.color.float32[0] = red;
    attachment.clearValue.color.float32[1] = green;
    attachment.clearValue.color.float32[2] = blue;
    attachment.clearValue.color.float32[3] = alpha;

    VkClearRect rect = {};
    rect.baseArrayLayer = 0;
    rect.layerCount = 1;
    rect.rect.offset.x = 0;
    rect.rect.offset.y = 0;
    rect.rect.extent.width = _renderPass->getWidth();
    rect.rect.extent.height = _renderPass->getHeight();

    vkCmdClearAttachments(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 1, &attachment, 1, &rect);
}

void GraphicsVK::cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
							          float depth, uint32_t stenci)
{
	GP_ASSERT(commandBuffer);
	VkClearAttachment attachment = {};
	attachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	attachment.clearValue.depthStencil.depth = depth;
	attachment.clearValue.depthStencil.stencil = stenci;

    VkClearRect rect = {};
    rect.baseArrayLayer = 0;
    rect.layerCount = 1;
    rect.rect.offset.x = 0;
    rect.rect.offset.y = 0;
    rect.rect.extent.width = _renderPass->getWidth();
    rect.rect.extent.height = _renderPass->getHeight();

    vkCmdClearAttachments(std::static_pointer_cast<CommandBufferVK>(commandBuffer)->_commandBuffer, 1, &attachment, 1, &rect);
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

std::shared_ptr<Buffer> GraphicsVK::createBuffer(Buffer::Usage usage, 
												 size_t size, 
												 size_t stride, 
												 bool hostVisible,
											     const void* data)
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
	
	VkMemoryAllocateInfo  allocInfo = {};
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

std::shared_ptr<Buffer> GraphicsVK::createVertexBuffer(size_t size, 
													   size_t vertexStride, 
													   bool hostVisible,
													   const void* data)
{
	return createBuffer(Buffer::USAGE_VERTEX, size, vertexStride, hostVisible, data);
}

std::shared_ptr<Buffer> GraphicsVK::createIndexBuffer(size_t size, 
													  size_t indexStride,
													  bool hostVisible,
													  const void* data)
{
	return createBuffer(Buffer::USAGE_INDEX, size, indexStride, hostVisible, data);
}

std::shared_ptr<Buffer> GraphicsVK::createUniformBuffer(size_t size, 
														bool hostVisible,
											            const void* data)
{
	return createBuffer(Buffer::USAGE_UNIFORM, size, size, hostVisible, data);
}

void GraphicsVK::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
	std::shared_ptr<BufferVK> bufferVK = std::static_pointer_cast<BufferVK>(buffer);
	vkDestroyBuffer(_device, bufferVK->_buffer, nullptr);
	bufferVK.reset();
}

std::shared_ptr<Texture> GraphicsVK::createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels, 
												   Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount, 
												   bool hostVisible, const void* data,
												   VkImage existingImage)
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

	VkFormat format = lookupVkFormat[pixelFormat];
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
		imageCreateInfo.samples = (VkSampleCountFlagBits)sampleCount;
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
		VK_CHECK_RESULT(vkCreateImage(_device, &imageCreateInfo, nullptr, &image));

		// Find memory requirements and allocate memory for image
		VkMemoryRequirements memReqs = {};
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
																	pixelFormat, usage, sampleCount, hostVisible, hostOwned,
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
													bool hostVisible,
													const void* data)
{
	return createTexture(Texture::TYPE_1D, width, 1, 1, 1, 
						 pixelFormat, usage, sampleCount, hostVisible, data, nullptr);
}

std::shared_ptr<Texture> GraphicsVK::createTexture2d(size_t width, size_t height, size_t mipLevels,
													 Format pixelFormat,
													 Texture::Usage usage,
													 Texture::SampleCount sampleCount,
													 bool hostVisible, const void* data)
{
	return createTexture(Texture::TYPE_2D, width, height, 1, mipLevels, 
						 pixelFormat, usage, sampleCount, hostVisible, data,
						 nullptr);
}

std::shared_ptr<Texture> GraphicsVK::createTexture3d(size_t width, size_t height, size_t depth,
													 Format pixelFormat,
													 Texture::Usage usage,
													 Texture::SampleCount sampleCount,
													 bool hostVisible,
													 const void* data)
{
	return createTexture(Texture::TYPE_3D, width, height, depth, 1, 
						 pixelFormat, usage, sampleCount, hostVisible, data,
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

	// Create the render pass
	VkAttachmentDescription* attachmentDescs = nullptr;
	VkAttachmentReference* colorAttachmentRefs = nullptr;
    VkAttachmentReference* resolveAttachmentRefs = nullptr;
    VkAttachmentReference* depthStencilAttachmentRef = nullptr;
	size_t depthStencilAttachmentCount = depthStencilFormat == Format::FORMAT_UNDEFINED ? 0 : 1;

	if (sampleCountVK > VK_SAMPLE_COUNT_1_BIT)
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

	uint32_t attachmentCount = (sampleCountVK > VK_SAMPLE_COUNT_1_BIT) ? (2 * colorAttachmentCount) : colorAttachmentCount;
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
		createInfo.dependencyCount = 0;
		createInfo.pDependencies = nullptr;

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
		if (sampleCountVK > VK_SAMPLE_COUNT_1_BIT)
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
			if (sampleCountVK > VK_SAMPLE_COUNT_1_BIT)
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
																			  (Texture::SampleCount)sampleCountVK,
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
	VkFormat colorFormatVK = lookupVkFormat[colorFormat];
	VkFormat depthStencilFormatVK = lookupVkFormat[depthStencilFormat];
	VkSampleCountFlagBits sampleCountVK = lookupVkSampleCountFlagBits[sampleCount];

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
		std::shared_ptr<Texture> colorAttachment = createTexture2d(width, height, 1, colorFormat, 
																   Texture::USAGE_COLOR_ATTACHMENT, 
																   Texture::SAMPLE_COUNT_1X, false, nullptr);
		std::shared_ptr<TextureVK> colorAttachmentVK = std::static_pointer_cast<TextureVK>(colorAttachment);
		colorAttachments.push_back(colorAttachment);
		if (sampleCount > Texture::SAMPLE_COUNT_1X)
		{
			std::shared_ptr<Texture> colorMultisampleAttachment = createTexture2d(width, height, 1, colorFormat, 
																				  Texture::USAGE_COLOR_ATTACHMENT, 
																				  sampleCount, false, nullptr);
			colorMultisampleAttachments.push_back(colorMultisampleAttachment);
		}
	}
	if (depthStencilFormat != Format::FORMAT_UNDEFINED)
	{
		depthStencilAttachment = createTexture2d(width, height, 1, depthStencilFormat, 
												 Texture::USAGE_DEPTH_STENCIL_ATTACHMENT, 
												 Texture::SAMPLE_COUNT_1X, false, nullptr);
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

std::shared_ptr<Sampler> GraphicsVK::createSampler(Sampler::Filter filterMin,
												   Sampler::Filter filterMag,
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
	createInfo.minFilter = lookupVkFilter[filterMin];
	createInfo.magFilter = lookupVkFilter[filterMag];
    createInfo.mipmapMode = lookupVkSamplerMipmapMode[filterMip];
    createInfo.addressModeU = lookupVkSamplerAddressMode[addressModeU];
    createInfo.addressModeV = lookupVkSamplerAddressMode[addressModeV];
    createInfo.addressModeW = lookupVkSamplerAddressMode[addressModeW];
	createInfo.borderColor = lookupVkBorderColor[borderColor];
	createInfo.compareEnable = compareEnabled ? VK_TRUE : VK_FALSE;
	createInfo.compareOp = lookupVkCompareOp[compareFunc];
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
            binding->stageFlags = lookupVkShaderStageFlags[descriptor->shaderStages];
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
    VkVertexInputBindingDescription inputBindings[GP_GRAPHICS_VERTEX_BINDINGS_MAX] = {};
    VkVertexInputAttributeDescription inputAttributes[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX] = {};
    uint32_t attributeCount = vertexLayout.getAttributeCount() > GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX ? GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX : vertexLayout.getAttributeCount();
	uint32_t inputAttributeCount = 0;

	uint32_t bindingValue = UINT32_MAX;
    for (size_t i = 0; i < attributeCount; ++i) 
	{
		VertexLayout::Attribute attribute = vertexLayout.getAttribute(i);
        if (bindingValue != attribute.binding) 
		{
            bindingValue = attribute.binding;
            ++inputBindingCount;
        }

        inputBindings[inputBindingCount - 1].binding = bindingValue;
        inputBindings[inputBindingCount - 1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		inputBindings[inputBindingCount - 1].stride += VertexLayout::toStride(attribute.format);

        inputAttributes[inputAttributeCount].location = attribute.location;
        inputAttributes[inputAttributeCount].binding = attribute.binding;
        inputAttributes[inputAttributeCount].format = lookupVkFormat[attribute.format];
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
    rasterizerStateCreateInfo.polygonMode = lookupVkPolygonMode[rasterizerState.fillMode];
    rasterizerStateCreateInfo.cullMode = lookupVkCullModeFlags[rasterizerState.cullMode];
    rasterizerStateCreateInfo.frontFace = lookupVkFrontFace[rasterizerState.frontFace];
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
    multisampleStateCreateInfo.rasterizationSamples = lookupVkSampleCountFlagBits[renderPass->getSampleCount()];
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
	depthStencilStateCreateInfo.depthTestEnable = depthStencilState.depthEnabled ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.depthWriteEnable = depthStencilState.depthWrite ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.depthCompareOp = lookupVkCompareOp[depthStencilState.depthFunc];
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable = depthStencilState.stencilEnabled ? VK_TRUE : VK_FALSE;
	depthStencilStateCreateInfo.front.failOp = lookupVkStencilOp[depthStencilState.stencilOpStateFront.failOp];
    depthStencilStateCreateInfo.front.passOp = lookupVkStencilOp[depthStencilState.stencilOpStateFront.passOp];
    depthStencilStateCreateInfo.front.depthFailOp = lookupVkStencilOp[depthStencilState.stencilOpStateFront.depthFailOp];
    depthStencilStateCreateInfo.front.compareOp = lookupVkCompareOp[depthStencilState.stencilOpStateFront.compareFunc];
    depthStencilStateCreateInfo.front.compareMask = depthStencilState.stencilOpStateFront.compareMask;
	depthStencilStateCreateInfo.front.writeMask = depthStencilState.stencilOpStateFront.writeMask;
    depthStencilStateCreateInfo.front.reference = 0;
    depthStencilStateCreateInfo.back.failOp = lookupVkStencilOp[depthStencilState.stencilOpStateBack.failOp];
    depthStencilStateCreateInfo.back.passOp = lookupVkStencilOp[depthStencilState.stencilOpStateBack.passOp];
    depthStencilStateCreateInfo.back.depthFailOp = lookupVkStencilOp[depthStencilState.stencilOpStateBack.depthFailOp];
    depthStencilStateCreateInfo.back.compareOp = lookupVkCompareOp[depthStencilState.stencilOpStateBack.compareFunc];
    depthStencilStateCreateInfo.back.compareMask = depthStencilState.stencilOpStateBack.compareMask;
	depthStencilStateCreateInfo.back.writeMask = depthStencilState.stencilOpStateBack.writeMask;
    depthStencilStateCreateInfo.back.reference = 0;
    depthStencilStateCreateInfo.minDepthBounds = 0.0f;
    depthStencilStateCreateInfo.maxDepthBounds = 0.0f;

	// ColorBlendState
	VkPipelineColorBlendAttachmentState colorBlendAttachmentStateCreateInfo = {};
	colorBlendAttachmentStateCreateInfo.blendEnable = colorBlendState.blendEnabled ? VK_TRUE : VK_FALSE;
    colorBlendAttachmentStateCreateInfo.colorWriteMask = colorBlendState.colorWriteMask;
    colorBlendAttachmentStateCreateInfo.alphaBlendOp = lookupVkBlendOp[colorBlendState.alphaBlendOp];
	colorBlendAttachmentStateCreateInfo.colorBlendOp = lookupVkBlendOp[colorBlendState.colorBlendOp];
	colorBlendAttachmentStateCreateInfo.srcColorBlendFactor = lookupVkBlendFactor[colorBlendState.colorBlendSrc];
    colorBlendAttachmentStateCreateInfo.dstColorBlendFactor = lookupVkBlendFactor[colorBlendState.colorBlendDst];
    colorBlendAttachmentStateCreateInfo.srcAlphaBlendFactor = lookupVkBlendFactor[colorBlendState.alphaBlendSrc];
    colorBlendAttachmentStateCreateInfo.dstAlphaBlendFactor = lookupVkBlendFactor[colorBlendState.alphaBlendSrc];

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

	std::shared_ptr<RenderPipelineVK> renderPipeline = std::make_shared<RenderPipelineVK>(primitiveTopology, vertexLayout,
																						  rasterizerState, colorBlendState, depthStencilState,
																						  renderPass, descriptorSet,
																						  vertShader, tescShader, teseShader, geomShader, fragShader,
																						  _device, pipelineLayout, pipeline);	
	return std::static_pointer_cast<RenderPipeline>(renderPipeline);

}

void GraphicsVK::destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline)
{
	GP_ASSERT(renderPipeline);

	std::shared_ptr<RenderPipelineVK> renderPipelineVK = std::static_pointer_cast<RenderPipelineVK>(renderPipeline);

	vkDestroyPipeline(_device, renderPipelineVK->_pipeline, nullptr);
    vkDestroyPipelineLayout(_device, renderPipelineVK->_pipelineLayout, nullptr);
	renderPipeline.reset();
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
	createSynchronizationObjects();

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
		dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)validationDebugReport;
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
	SwapchainInfo swapchainInfo = querySwapchainInfo(_physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapchainInfo.formats);
	VkPresentModeKHR presentMode = choosePresentMode(swapchainInfo.presentModes);

	// Request the swapchain backbuffer image count 
	_swapchainImageCount = swapchainInfo.capabilities.minImageCount;
	if (swapchainInfo.capabilities.maxImageCount > 0 && _swapchainImageCount > swapchainInfo.capabilities.maxImageCount) 
		_swapchainImageCount = swapchainInfo.capabilities.maxImageCount;

	// Find the transformation of the surface (prefer non-rotated)
	VkSurfaceTransformFlagsKHR preTransfer;
	if (swapchainInfo.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		preTransfer = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	else 
		preTransfer = swapchainInfo.capabilities.currentTransform;

	// Create the swapchain
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.surface = _surface;
	createInfo.minImageCount = GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT;
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
		std::shared_ptr<Texture> colorAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
																 toFormat(_colorFormat),
																 Texture::USAGE_COLOR_ATTACHMENT,
																 Texture::SAMPLE_COUNT_1X, false, nullptr,
																 _swapchainImages[i]);
		colorAttachments.push_back(colorAttachment);
		std::shared_ptr<Texture> depthStencilAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
																	    toFormat(_depthStencilFormat),
																		Texture::USAGE_DEPTH_STENCIL_ATTACHMENT,
																		Texture::SAMPLE_COUNT_1X, false, nullptr,
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

void GraphicsVK::createSynchronizationObjects()
{
	_swapchainAcquiredFences.resize(GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT);
	_swapchainAcquiredSemaphores.resize(GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT);

	VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = _queueFamilyIndexGraphics;
    VK_CHECK_RESULT(vkCreateCommandPool(_device, &info, nullptr, &_commandPool));

	for (size_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	{
		// Create the swapchain fence
		{
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			VK_CHECK_RESULT(vkCreateFence(_device, &createInfo, nullptr, &_swapchainAcquiredFences[i]));
		}

		// Create the swapchain semaphore
		{
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			VK_CHECK_RESULT(vkCreateSemaphore(_device, &createInfo, nullptr, &_swapchainAcquiredSemaphores[i]));
		}

		// Create command buffers
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = _commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VK_CHECK_RESULT(vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer));
		_commandBuffers.push_back(std::make_shared<CommandBufferVK>(_device, _commandPool, commandBuffer));
	}
}

GraphicsVK::SwapchainInfo GraphicsVK::querySwapchainInfo(VkPhysicalDevice physicalDevice)
{
	// Get the capabilities of the swapchain
	SwapchainInfo swapchainInfo;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &swapchainInfo.capabilities);

	// Get surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		swapchainInfo.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, swapchainInfo.formats.data());
	}

	// Get the presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) 
	{
		swapchainInfo.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, swapchainInfo.presentModes.data());
	}
	return swapchainInfo;
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

std::string toErrorString(VkResult result)
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
