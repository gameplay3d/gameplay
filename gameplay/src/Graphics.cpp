#include "Base.h"
#include "Graphics.h"
#include "GraphicsUtil.h"
#include "Game.h"
#include "Platform.h"
#include "SceneObject.h"
#include "FileSystem.h"

namespace gameplay
{

#define SWAPCHAIN_IMAGE_COUNT       3
#define SHADER_PATH                 "assets/shaders/spirv/"
#define SHADER_EXT                  ".spv"

const std::vector<const char*> __validationLayers = 
{
    "VK_LAYER_LUNARG_standard_validation"
};

Graphics::Graphics() :
    _initialized(false),
    _resized(false),
    _width(0),
    _height(0),
    _fullscreen(false),
    _vsync(false),
    _multisampling(false),
    _validation(false),
    _instance(VK_NULL_HANDLE),
    _surface(VK_NULL_HANDLE),
    _physicalDevice(VK_NULL_HANDLE),
    _debugMessageCallback(VK_NULL_HANDLE),
    _queue(VK_NULL_HANDLE),
    _queuePresentIndex(UINT32_MAX),
    _device(VK_NULL_HANDLE),
    _swapchain(VK_NULL_HANDLE),
    _swapchainImageIndex(0),
    _swapchainImageCount(0),
    _colorSpace(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR),
    _colorFormat(VK_FORMAT_UNDEFINED),
    _depthStencilFormat(VK_FORMAT_UNDEFINED),
    _commandPool(VK_NULL_HANDLE)
{
    _queueFamilyIndices.graphics = VK_NULL_HANDLE;
    _queueFamilyIndices.compute = VK_NULL_HANDLE;
    _queueFamilyIndices.transfer = VK_NULL_HANDLE;
}

Graphics::~Graphics()
{
    vkDeviceWaitIdle(_device);
    destroySemaphore(_presentCompleteSemaphore);
    destroySemaphore(_renderCompleteSemaphore);
    destroyCommandBuffers();
    destroyRenderPasses();

    if (vkDebugReportMessageEXT != VK_NULL_HANDLE && _debugMessageCallback != VK_NULL_HANDLE)
    {
        vkDestroyDebugReportCallbackEXT(_instance, _debugMessageCallback, nullptr);
    }
    if (_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
    }
    vkDestroyDevice(_device, nullptr);
    vkDestroyInstance(_instance, nullptr);
}

std::shared_ptr<RenderPass> Graphics::acquireNextFrame()
{
    // Acquire the next image from the swap chain
    vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, _presentCompleteSemaphore->_semaphore, (VkFence)nullptr, &_swapchainImageIndex);

    VkFence fence = _waitFences[_swapchainImageIndex];
    VK_CHECK_RESULT(vkWaitForFences(_device, 1, &fence, VK_TRUE, UINT64_MAX));
    VK_CHECK_RESULT(vkResetFences(_device, 1, &fence));
    return _renderPasses[_swapchainImageIndex];
}

void Graphics::presentFrame(std::shared_ptr<Semaphore> waitSemaphore)
{
    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    if (waitSemaphore != nullptr)
    {
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &waitSemaphore->_semaphore;
    }
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &_swapchain;
    presentInfo.pImageIndices = &_swapchainImageIndex;
    presentInfo.pResults = nullptr;

    VK_CHECK_RESULT(vkQueuePresentKHR(_queue, &presentInfo));
    VK_CHECK_RESULT(vkQueueWaitIdle(_queue));
}

uint32_t Graphics::getWidth()
{
    return _width;
}

uint32_t Graphics::getHeight()
{
    return _height;
}

std::shared_ptr<RenderPass> Graphics::getRenderPass()
{
    return _renderPasses[_swapchainImageIndex]; 
}

std::shared_ptr<CommandBuffer> Graphics::beginCommands()
{
    _commandBuffer = _commandBuffers[_swapchainImageIndex];
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    VK_CHECK_RESULT(vkBeginCommandBuffer(_commandBuffer->_commandBuffer, &beginInfo));

    return _commandBuffer;
}

void Graphics::endCommands()
{
    GP_ASSERT(_commandBuffer != nullptr);
    VK_CHECK_RESULT(vkEndCommandBuffer(_commandBuffer->_commandBuffer));
}

std::shared_ptr<Semaphore> Graphics::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphoreVK;
    VK_CHECK_RESULT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &semaphoreVK));

    std::shared_ptr<Semaphore> semaphore = std::make_shared<Semaphore>();
    semaphore->_semaphore = semaphoreVK;

    return semaphore;
}

void Graphics::destroySemaphore(std::shared_ptr<Semaphore> semaphore)
{
    vkDestroySemaphore(_device, semaphore->_semaphore, nullptr);
    semaphore.reset();
}

std::shared_ptr<Semaphore> Graphics::getSemaphoreRenderComplete()
{
    return _renderCompleteSemaphore;
}

std::shared_ptr<Semaphore> Graphics::getSemaphorePresentComplete()
{
    return _presentCompleteSemaphore;
}

void Graphics::submit(std::shared_ptr<CommandBuffer> commandBuffer,
                      std::shared_ptr<Semaphore> waitSemaphore,
                      std::shared_ptr<Semaphore> signalSemaphore)
{   
    VkPipelineStageFlags waitMasks = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &waitSemaphore->_semaphore;
    submitInfo.pWaitDstStageMask = &waitMasks;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->_commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore->_semaphore;

    VK_CHECK_RESULT(vkQueueSubmit(_queue, 1, &submitInfo, _waitFences[_swapchainImageIndex]));
}


void Graphics::cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
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

    vkCmdSetViewport(commandBuffer->_commandBuffer, 0, 1, &viewport);
}

void Graphics::cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
                             size_t x, size_t y, size_t width, size_t height)
{
    GP_ASSERT(commandBuffer);

    VkRect2D rect = {};
    rect.offset.x = x;
    rect.offset.x = y;
    rect.extent.width = width;
    rect.extent.height = height;

    vkCmdSetScissor(commandBuffer->_commandBuffer, 0, 1, &rect);
}

void Graphics::cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
                                  std::shared_ptr<RenderPass> renderPass)
{
    GP_ASSERT(commandBuffer);
    GP_ASSERT(renderPass);

    VkRect2D renderArea = {};
    renderArea.offset.x = 0;
    renderArea.offset.y = 0;
    renderArea.extent.width = renderPass->getWidth();
    renderArea.extent.height = renderPass->getHeight();
    
    VkClearValue clearValues[(2 * GP_GRAPHICS_COLOR_ATTACHMENTS_MAX) + 1];
    uint32_t colorValueCount = renderPass->getColorAttachmentCount();
    for (uint32_t i = 0; i < colorValueCount; ++i)
    {
        clearValues[i].color.float32[0] = 0;
        clearValues[i].color.float32[1] = 0;
        clearValues[i].color.float32[2] = 0;
        clearValues[i].color.float32[3] = 1;
        if (renderPass->getSampleCount() > Texture::SampleCount::e1X)
        {
            clearValues[i].color.float32[0] = 0;
            clearValues[i].color.float32[1] = 0;
            clearValues[i].color.float32[2] = 0;
            clearValues[i].color.float32[3] = 1;
        }
    }
    // TODO: multismaple
    uint32_t depthStencilCount = renderPass->getDepthStencilFormat() == Format::eUndefined ? 0 : 1;
    if (depthStencilCount > 0) 
    {
        clearValues[colorValueCount].depthStencil.depth = 1.0f;
        clearValues[colorValueCount].depthStencil.stencil = 0;
    }

    VkRenderPassBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = renderPass->_renderPass;
    beginInfo.framebuffer = renderPass->_framebuffer;
    beginInfo.renderArea = renderArea;

    uint32_t clearValueCount = colorValueCount;
    if (renderPass->getSampleCount() > Texture::SampleCount::e1X)
    {
        clearValueCount *= 2;
    }
    clearValueCount += depthStencilCount;

    beginInfo.clearValueCount = clearValueCount;
    beginInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer->_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

    _renderPass = renderPass;
}

void Graphics::cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer)
{
    GP_ASSERT(commandBuffer);

    vkCmdEndRenderPass(commandBuffer->_commandBuffer);
}

void Graphics::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
                                     std::shared_ptr<RenderPipeline> renderPipeline)
{
    GP_ASSERT(commandBuffer);

    vkCmdBindPipeline(commandBuffer->_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
                      renderPipeline->_pipeline);
}

void Graphics::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                                    std::shared_ptr<RenderPipeline> renderPipeline, 
                                    std::shared_ptr<DescriptorSet> descriptorSet)
{
    GP_ASSERT(commandBuffer);

    vkCmdBindDescriptorSets(commandBuffer->_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            renderPipeline->_pipelineLayout, 0, 1, 
                            &descriptorSet->_descriptorSet, 0, nullptr);
}

void Graphics::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                                   std::shared_ptr<Buffer> vertexBuffer)
{
    GP_ASSERT(commandBuffer);

    VkDeviceSize offsets[1] = {};
    vkCmdBindVertexBuffers(commandBuffer->_commandBuffer, 0, 1, &vertexBuffer->_buffer, offsets);
}


void Graphics::cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
                                      std::vector<std::shared_ptr<Buffer>> vertexBuffers)
{
    GP_ASSERT(commandBuffer);

    const uint32_t buffersMax = _deviceProperties.limits.maxVertexInputBindings;
    uint32_t buffersMaxCapped = vertexBuffers.size() > buffersMax ? buffersMax : vertexBuffers.size();
    GP_ASSERT(buffersMaxCapped < 64);

    VkBuffer buffers[64] = {};
    VkDeviceSize offsets[64] = {};
    for (uint32_t i = 0; i < buffersMaxCapped; ++i) 
    {
        buffers[i] = vertexBuffers[i]->_buffer;
    }
    vkCmdBindVertexBuffers(commandBuffer->_commandBuffer, 0, buffersMaxCapped, buffers, offsets);
}

void Graphics::cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                                  std::shared_ptr<Buffer> indexBuffer)
{
    GP_ASSERT(commandBuffer);

    VkIndexType indexType = (indexBuffer->getStride() == sizeof(uint16_t)) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;

    vkCmdBindIndexBuffer(commandBuffer->_commandBuffer, indexBuffer->_buffer, 0, indexType);
}

void Graphics::cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
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

    vkCmdClearAttachments(commandBuffer->_commandBuffer, 1, &attachment, 1, &rect);
}

void Graphics::cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
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

    vkCmdClearAttachments(commandBuffer->_commandBuffer, 1, &attachment, 1, &rect);
}


void Graphics::cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
                       size_t vertexCount, size_t vertexStart)
{
    GP_ASSERT(commandBuffer);

     vkCmdDraw(commandBuffer->_commandBuffer, vertexCount, 1, vertexStart, 0);
}

void Graphics::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                              size_t indexCount, size_t indexStart)
{
    GP_ASSERT(commandBuffer);

    vkCmdDrawIndexed(commandBuffer->_commandBuffer, indexCount, 1, indexStart, 0, 0);
}

void Graphics::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                                  std::shared_ptr<Texture> texture, 
                                  Texture::Usage usageOld,
                                  Texture::Usage usageNew)
{
    GP_ASSERT(commandBuffer);

    VkPipelineStageFlags stageMskSrc = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkPipelineStageFlags stageMaskDst = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkDependencyFlags dependencyFlags = 0;
    
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = NULL;
    barrier.oldLayout = toVkImageLayout(usageOld);
    barrier.newLayout = toVkImageLayout(usageNew);
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = texture->_image;
    barrier.subresourceRange.aspectMask = texture->_imageAspectFlags;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = texture->getMipLevels();
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
    vkCmdPipelineBarrier(commandBuffer->_commandBuffer, stageMskSrc, stageMaskDst, dependencyFlags, 0, nullptr, 0, nullptr, 1, &barrier);
}

std::shared_ptr<Buffer> Graphics::createBuffer(Buffer::Usage usage,
                                               size_t size, 
                                               size_t stride, 
                                               bool hostVisible,
                                               const void* data)
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.size = size;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;
    bufferCreateInfo.usage = 0;
    switch (usage)
    {
    case Buffer::Usage::eVertex:
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        break;
    case Buffer::Usage::eIndex:
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        break;
    case Buffer::Usage::eUniform:
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        break;
    }

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = nullptr;
    VkMemoryRequirements memReqs;
    if (hostVisible)
    {
        // Create the host visible buffer
        VkBuffer bufferVK;
        VK_CHECK_RESULT(vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &bufferVK));
        vkGetBufferMemoryRequirements(_device, bufferVK, &memReqs);
        memAlloc.allocationSize = memReqs.size;
        uint32_t memTypeIndex = UINT32_MAX;
        VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        if (!getMemoryType(memReqs.memoryTypeBits, memFlags, &memTypeIndex))
        {
            GP_ERROR("Failed to find compatible memory for buffer.");
        }
        VkDeviceMemory deviceMemory;
        memAlloc.memoryTypeIndex = memTypeIndex;
        VK_CHECK_RESULT(vkAllocateMemory(_device, &memAlloc, nullptr, &deviceMemory));
        VK_CHECK_RESULT(vkBindBufferMemory(_device, bufferVK, deviceMemory, 0));

        std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
        if (data)
        {
            VK_CHECK_RESULT(vkMapMemory(_device, deviceMemory, 0, VK_WHOLE_SIZE, 0, &buffer->_hostMemory));
            memcpy(buffer->_hostMemory, data, size);
            vkUnmapMemory(_device, deviceMemory);
        }
        buffer->_usage = usage;
        buffer->_size = size;
        buffer->_stride = stride;
        buffer->_hostVisible = hostVisible;
        buffer->_deviceMemory = deviceMemory;
        buffer->_buffer = bufferVK;
        buffer->_bufferView.buffer = bufferVK;
        buffer->_bufferView.offset = 0;
        buffer->_bufferView.range = VK_WHOLE_SIZE;

        return buffer;
    }
    else
    {
        // Create a host-visible staging buffer to temp copy the vertex data to
        VkBuffer stagingBuffer;
        void* stagingData;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VK_CHECK_RESULT(vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &stagingBuffer));
        vkGetBufferMemoryRequirements(_device, stagingBuffer, &memReqs);
        memAlloc.allocationSize = memReqs.size;
        VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        uint32_t memTypeIndex = UINT32_MAX;
        if (!getMemoryType(memReqs.memoryTypeBits, memFlags, &memTypeIndex))
        {
            GP_ERROR("Failed to find compatible memory for buffer.");
        }
        VkDeviceMemory stagingMemory;
        VK_CHECK_RESULT(vkAllocateMemory(_device, &memAlloc, nullptr, &stagingMemory));
        VK_CHECK_RESULT(vkMapMemory(_device, stagingMemory, 0, memAlloc.allocationSize, 0, (void**)&stagingData));
        memcpy(stagingData, data, size);
        vkUnmapMemory(_device, stagingMemory);
        VK_CHECK_RESULT(vkBindBufferMemory(_device, stagingBuffer, stagingMemory, 0));

        // Create a device local buffer to copied into
        VkBuffer bufferVK;
        bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VK_CHECK_RESULT(vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &bufferVK));
        vkGetBufferMemoryRequirements(_device, bufferVK, &memReqs);
        memAlloc.allocationSize = memReqs.size;
        memFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        memTypeIndex = UINT32_MAX;
        if (!getMemoryType(memReqs.memoryTypeBits, memFlags, &memTypeIndex))
        {
            GP_ERROR("Failed to find compatible memory for buffer.");
        }
        VkDeviceMemory deviceMemory;
        memAlloc.memoryTypeIndex = memTypeIndex;
        VK_CHECK_RESULT(vkAllocateMemory(_device, &memAlloc, nullptr, &deviceMemory));
        VK_CHECK_RESULT(vkBindBufferMemory(_device, bufferVK, deviceMemory, 0));

        // Copy from the staging buffer to local
        VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBufferBeginInfo.pNext = nullptr;
        VkCommandBuffer copyCmd = getCommandBuffer(true);
        VkBufferCopy copyRegion = {};
        copyRegion.size = size;
        vkCmdCopyBuffer(copyCmd, stagingBuffer, bufferVK, 1, &copyRegion);
        flushCommandBuffer(copyCmd);

        // Destroy the staging buffer and device memory
        vkDestroyBuffer(_device, stagingBuffer, nullptr);
        vkFreeMemory(_device, stagingMemory, nullptr);

        std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();        
        buffer->_usage = usage;
        buffer->_size = size;
        buffer->_stride = stride;
        buffer->_hostVisible = hostVisible;
        buffer->_deviceMemory = deviceMemory;
        buffer->_buffer = bufferVK;
        buffer->_bufferView.buffer = bufferVK;
        buffer->_bufferView.offset = 0;
        buffer->_bufferView.range = VK_WHOLE_SIZE;

        return buffer;
    }
}


std::shared_ptr<Buffer> Graphics::createVertexBuffer(size_t size, 
                                                     size_t vertexStride, 
                                                     bool hostVisible,
                                                     const void* data)
{
    return createBuffer(Buffer::Usage::eVertex, size, vertexStride, hostVisible, data);
}

std::shared_ptr<Buffer> Graphics::createIndexBuffer(size_t size, 
                                                    size_t indexStride,
                                                    bool hostVisible,
                                                    const void* data)
{
    return createBuffer(Buffer::Usage::eIndex, size, indexStride, hostVisible, data);
}

std::shared_ptr<Buffer> Graphics::createUniformBuffer(size_t size, 
                                                      bool hostVisible,
                                                      const void* data)
{
    return createBuffer(Buffer::Usage::eUniform, size, size, hostVisible, data);
}

void Graphics::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
    vkDestroyBuffer(_device, buffer->_buffer, nullptr);
    buffer.reset();
}

std::shared_ptr<Texture> Graphics::createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels, 
                                                 Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount,
                                                 bool hostVisible, const void* data,
                                                 VkImage existingImage)
{
    VkImageType imageType = VK_IMAGE_TYPE_2D;
    VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
    switch (type) 
    {
    case Texture::Type::e1D:
        imageType = VK_IMAGE_TYPE_1D;
        imageViewType = VK_IMAGE_VIEW_TYPE_1D;
        break;
    case Texture::Type::e2D:
        imageType = VK_IMAGE_TYPE_2D;
        imageViewType = VK_IMAGE_VIEW_TYPE_2D;
        break;
    case Texture::Type::e3D:
        imageType = VK_IMAGE_TYPE_3D; 
        imageViewType = VK_IMAGE_VIEW_TYPE_3D;
        break;    
    }

    VkFormat format = lookupVkFormat[static_cast<uint32_t>(pixelFormat)];
    VkImage image;
    VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
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
            // Format is not supported for host visible images
            VkFormatFeatureFlags flags = formatProps.linearTilingFeatures & formatFeatureFlags;
            GP_ASSERT(flags != 0);
        }
        else
        {
            // Format is not supported for GPU local images (i.e. not host visible images
            VkFormatFeatureFlags flags = formatProps.optimalTilingFeatures & formatFeatureFlags;
            GP_ASSERT(flags != 0);
        }

        // Apply some bounds to the image
        VkImageFormatProperties imageFormatProps = {};
        VkResult vkResult = vkGetPhysicalDeviceImageFormatProperties(_physicalDevice,
                                                                     imageCreateInfo.format,
                                                                     imageCreateInfo.imageType,
                                                                     imageCreateInfo.tiling,
                                                                     imageCreateInfo.usage,
                                                                     imageCreateInfo.flags, &imageFormatProps);
        GP_ASSERT(VK_SUCCESS == vkResult);
        if (imageCreateInfo.mipLevels > 1)
        {
            mipLevels = GP_MATH_MIN(mipLevels, imageFormatProps.maxMipLevels);
            imageCreateInfo.mipLevels = mipLevels;
        }

        // Create image
        VK_CHECK_RESULT(vkCreateImage(_device, &imageCreateInfo, nullptr, &image));

        // Find memory requirements and set flags and allocate memory for image
        VkMemoryRequirements memReqs = {};
        vkGetImageMemoryRequirements(_device, image, &memReqs);

        VkMemoryPropertyFlags memFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        if (hostVisible)
        {
            memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        }
        uint32_t memoryTypeIndex = UINT32_MAX;
        bool found = getMemoryType(memReqs.memoryTypeBits, memFlags, &memoryTypeIndex);
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

    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->_type = type;
    texture->_width = width;
    texture->_height = height;
    texture->_depth = depth;
    texture->_mipLevels = mipLevels;
    texture->_pixelFormat = pixelFormat;
    texture->_usage = usage;
    texture->_sampleCount = sampleCount;
    texture->_hostVisible = hostVisible;
    texture->_hostOwned = hostOwned;
    texture->_image = image;
    texture->_deviceMemory = deviceMemory;

    if (hostVisible)
    {
        VK_CHECK_RESULT(vkMapMemory(_device, texture->_deviceMemory, 0, VK_WHOLE_SIZE, 0, &texture->_hostMemory));
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

    VK_CHECK_RESULT(vkCreateImageView(_device, &imageViewCreateinfo, nullptr, &(texture->_imageView)));

    texture->_imageAspectFlags = imageViewCreateinfo.subresourceRange.aspectMask;
    texture->_imageViewInfo.imageView = texture->_imageView;
    texture->_imageViewInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return texture;
}


std::shared_ptr<Texture> Graphics::createTexture1d(size_t width, 
                                                   Format pixelFormat, 
                                                   Texture::Usage usage,
                                                   Texture::SampleCount sampleCount,
                                                   bool hostVisible,
                                                   const void* data)
{
    return createTexture(Texture::Type::e1D, width, 1, 1, 1,
                         pixelFormat, usage, sampleCount,
                         hostVisible, data, nullptr);
}

std::shared_ptr<Texture> Graphics::createTexture2d(size_t width, size_t height, size_t mipLevels,
                                                   Format pixelFormat,
                                                   Texture::Usage usage,
                                                   Texture::SampleCount sampleCount,
                                                   bool hostVisible, const void* data)
{
    return createTexture(Texture::Type::e2D, width, height, 1, mipLevels,
                         pixelFormat, usage, sampleCount,
                         hostVisible, data, nullptr);
}

std::shared_ptr<Texture> Graphics::createTexture3d(size_t width, size_t height, size_t depth,
                                                     Format pixelFormat,
                                                     Texture::Usage usage,
                                                     Texture::SampleCount sampleCount,
                                                     bool hostVisible,
                                                     const void* data)
{
    return createTexture(Texture::Type::e3D, width, height, depth, 1,
                         pixelFormat, usage, sampleCount,
                         hostVisible, data, nullptr);
}

void Graphics::destroyTexture(std::shared_ptr<Texture> texture)
{
    if (texture->isHostOwned())
    {
        if (texture->_deviceMemory)
        {
            vkFreeMemory(_device, texture->_deviceMemory, nullptr);
        }
        if (texture->_image)
        {
            vkDestroyImage(_device, texture->_image, nullptr);
        }
    }
    if (texture->_imageView)
    {
        vkDestroyImageView(_device, texture->_imageView, nullptr);
    }
    texture.reset();
}

std::shared_ptr<RenderPass> Graphics::createRenderPass(size_t width, size_t height,
                                                       size_t colorAttachmentCount,
                                                       std::vector<VkFormat> colorFormatsVK,
                                                       VkFormat depthStencilFormatVK,
                                                       VkSampleCountFlagBits sampleCountVK,
                                                       std::vector<std::shared_ptr<Texture>> colorAttachments,
                                                       std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
                                                       std::shared_ptr<Texture> depthStencilAttachment)
{
    std::vector<Format> colorFormats;
    colorFormats.resize(colorAttachmentCount);
    for (size_t i = 0; i < colorAttachmentCount; i++)
    {
        colorFormats[i] = toFormat(colorFormatsVK[i]);
    }
    Format depthStencilFormat = toFormat(depthStencilFormatVK);

    // Create the render pass
    VkAttachmentDescription* attachments = nullptr;
    VkAttachmentReference* colorAttachmentRefs = nullptr;
    VkAttachmentReference* resolveAttachmentRefs = nullptr;
    VkAttachmentReference* depthStencilAttachmentRef = nullptr;
    size_t depthStencilAttachmentCount = (depthStencilFormat == Format::eUndefined) ? 0 : 1;

    if (sampleCountVK > VK_SAMPLE_COUNT_1_BIT)
    {
        attachments = (VkAttachmentDescription*)calloc((2 * colorAttachmentCount) + depthStencilAttachmentCount, sizeof(*attachments));
        GP_ASSERT(attachments != nullptr);
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
            attachments[colorIndex].flags = 0;
            attachments[colorIndex].format = colorFormatsVK[i];
            attachments[colorIndex].samples = sampleCountVK;
            attachments[colorIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[colorIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[colorIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[colorIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[colorIndex].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[colorIndex].flags = 0;

            attachments[multiSampleIndex].format = colorFormatsVK[i];
            attachments[multiSampleIndex].samples = sampleCountVK;
            attachments[multiSampleIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[multiSampleIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[multiSampleIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[multiSampleIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[multiSampleIndex].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachments[multiSampleIndex].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            colorAttachmentRefs[i].attachment = multiSampleIndex;
            colorAttachmentRefs[i].layout = attachments[multiSampleIndex].initialLayout;
            resolveAttachmentRefs[i].attachment = colorIndex;
            resolveAttachmentRefs[i].layout = attachments[colorIndex].initialLayout;
        }
    
        if (depthStencilAttachmentCount > 0)
        {
            const uint32_t index = (2 * colorAttachmentCount);
            attachments[index].flags = 0;
            attachments[index].format = depthStencilFormatVK;
            attachments[index].samples = sampleCountVK;
            attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[index].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachments[index].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthStencilAttachmentRef[0].attachment = index;
            depthStencilAttachmentRef[0].layout = attachments[index].initialLayout;
        }
    }
    else
    {
        attachments = (VkAttachmentDescription*)calloc(colorAttachmentCount + depthStencilAttachmentCount, sizeof(*attachments));
        GP_ASSERT(attachments != nullptr);

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
            attachments[colorIndex].flags = 0;
            attachments[colorIndex].format = colorFormatsVK[i];
            attachments[colorIndex].samples = sampleCountVK;
            attachments[colorIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[colorIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[colorIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[colorIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[colorIndex].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[colorIndex].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            colorAttachmentRefs[i].attachment = colorIndex;
            colorAttachmentRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        if (depthStencilAttachmentCount > 0) 
        {
            uint32_t index = colorAttachmentCount;
            attachments[index].flags = 0;
            attachments[index].format = depthStencilFormatVK;
            attachments[index].samples = sampleCountVK;
            attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[index].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[index].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            depthStencilAttachmentRef[0].attachment = index;
            depthStencilAttachmentRef[0].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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
    VkSubpassDependency subpassDep= {};
    subpassDep.srcSubpass = 0;
    subpassDep.dstSubpass = 0;
    subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDep.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDep.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    uint32_t attachmentCount = (sampleCountVK > VK_SAMPLE_COUNT_1_BIT) ? (2 * colorAttachmentCount) : colorAttachmentCount;
    attachmentCount += depthStencilAttachmentCount;

    // Create render pass
    VkRenderPass renderPassVK;
    {
        VkRenderPassCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.attachmentCount = attachmentCount;
        createInfo.pAttachments = attachments;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &subpassDep;

        VK_CHECK_RESULT(vkCreateRenderPass(_device, &createInfo, nullptr, &renderPassVK));
    }

    GP_SAFE_FREE(attachments);
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
        if (depthStencilFormat != Format::eUndefined)
        {
            imageCount += 1;
        }

        // Create the framebuffer textures
        VkImageView* imageViews = (VkImageView*)calloc(imageCount, sizeof(*imageViews));
        VkImageView* imageView = imageViews;
        for (size_t i = 0; i < colorAttachmentCount; ++i)
        {
            *imageView = colorAttachments[i]->_imageView;
            ++imageView;
            if (sampleCountVK > VK_SAMPLE_COUNT_1_BIT)
            {
                *imageView = colorMultisampleAttachments[i]->_imageView;
                ++imageView;
            }
        }
        if (depthStencilFormat != Format::eUndefined)
        {
            *imageView = depthStencilAttachment->_imageView;
            ++imageView;
        }

        // Create the framebuffer
        VkFramebufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.renderPass = renderPassVK;
        createInfo.attachmentCount = attachmentCount;
        createInfo.pAttachments = imageViews;
        createInfo.width = width;
        createInfo.height = height;
        createInfo.layers = 1;

        VK_CHECK_RESULT(vkCreateFramebuffer(_device, &createInfo, nullptr, &framebuffer));
        GP_SAFE_FREE(imageViews);
    }

    std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
    renderPass->_width = width;
    renderPass->_height = height;
    renderPass->_colorAttachmentCount = colorAttachmentCount;
    renderPass->_colorFormats = colorFormats;
    renderPass->_depthStencilFormat = depthStencilFormat;
    renderPass->_sampleCount = (Texture::SampleCount)sampleCountVK;
    renderPass->_colorAttachments = colorAttachments;
    renderPass->_colorMultisampleAttachments = colorMultisampleAttachments;
    renderPass->_depthStencilAttachment = depthStencilAttachment;
    renderPass->_renderPass = renderPassVK;
    renderPass->_framebuffer = framebuffer;
                                                                              
    return renderPass;
}

std::shared_ptr<RenderPass> Graphics::createRenderPass(size_t width, size_t height,
                                                       size_t colorAttachmentCount,
                                                       std::vector<Format> colorFormats,
                                                       Format depthStencilFormat,
                                                       Texture::SampleCount sampleCount)
{
    std::vector<VkFormat> colorFormatsVK;
    colorFormatsVK.resize(colorAttachmentCount);
    for (size_t i = 0; i < colorAttachmentCount; ++i)
    {
        colorFormatsVK[i] = lookupVkFormat[static_cast<uint32_t>(colorFormats[i])];
    }
    VkFormat depthStencilFormatVK = lookupVkFormat[static_cast<uint32_t>(depthStencilFormat)];
    VkSampleCountFlagBits sampleCountVK = lookupVkSampleCountFlagBits[static_cast<uint32_t>(sampleCount)];

    // Create the textures for the framebuffer
    std::vector<std::shared_ptr<Texture>> colorAttachments;
    std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;
    std::shared_ptr<Texture> depthStencilAttachment = nullptr;
    size_t imageCount = colorAttachmentCount;
    if (sampleCount > Texture::SampleCount::e1X)
    {
        imageCount *= 2;
    }
    if (depthStencilFormat != Format::eUndefined)
    {
        imageCount += 1;
    }

    for (size_t i = 0; i < colorAttachmentCount; ++i)
    {
        std::shared_ptr<Texture> colorAttachment = createTexture2d(width, height, 1, colorFormats[i], 
                                                                   Texture::Usage::eColorAttachment,
                                                                   Texture::SampleCount::e1X, false, nullptr);
        colorAttachments.push_back(colorAttachment);
        if (sampleCount > Texture::SampleCount::e1X)
        {
            std::shared_ptr<Texture> colorMultisampleAttachment = createTexture2d(width, height, 1, colorFormats[i], 
                                                                                  Texture::Usage::eColorAttachment,
                                                                                  sampleCount, false, nullptr);
            colorMultisampleAttachments.push_back(colorMultisampleAttachment);
        }
    }
    if (depthStencilFormat != Format::eUndefined)
    {
        depthStencilAttachment = createTexture2d(width, height, 1, depthStencilFormat, 
                                                 Texture::Usage::eDepthStencilAttachment,
                                                 Texture::SampleCount::e1X, false, nullptr);
    }
    return createRenderPass(width, height,
                            colorAttachmentCount,
                            colorFormatsVK,
                            depthStencilFormatVK,
                            sampleCountVK,
                            colorAttachments,
                            colorMultisampleAttachments,
                            depthStencilAttachment);
}

void Graphics::destroyRenderPass(std::shared_ptr<RenderPass> renderPass)
{
    GP_ASSERT(renderPass);

    for (size_t i = 0; i < renderPass->getColorAttachmentCount(); i++)
    {
        destroyTexture(renderPass->getColorAttachment(i));
        if (renderPass->getSampleCount() > Texture::SampleCount::e1X)
        {
            destroyTexture(renderPass->getColorMultisampleAttachment(i));
        }
    }
    if (renderPass->getDepthStencilFormat() != Format::eUndefined)
    {
        destroyTexture(renderPass->getDepthStencilAttachment());
    }

    vkDestroyFramebuffer(_device, renderPass->_framebuffer, nullptr);
    vkDestroyRenderPass(_device, renderPass->_renderPass, nullptr);
    renderPass.reset();
}

std::shared_ptr<Sampler> Graphics::createSampler(Sampler::Filter filterMin,
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
    createInfo.minFilter = lookupVkFilter[static_cast<uint32_t>(filterMin)];
    createInfo.magFilter = lookupVkFilter[static_cast<uint32_t>(filterMag)];
    createInfo.mipmapMode = lookupVkSamplerMipmapMode[static_cast<uint32_t>(filterMip)];
    createInfo.addressModeU = lookupVkSamplerAddressMode[static_cast<uint32_t>(addressModeU)];
    createInfo.addressModeV = lookupVkSamplerAddressMode[static_cast<uint32_t>(addressModeV)];
    createInfo.addressModeW = lookupVkSamplerAddressMode[static_cast<uint32_t>(addressModeW)];
    createInfo.borderColor = lookupVkBorderColor[static_cast<uint32_t>(borderColor)];
    createInfo.compareEnable = compareEnabled ? VK_TRUE : VK_FALSE;
    createInfo.compareOp = lookupVkCompareOp[static_cast<uint32_t>(compareFunc)];
    createInfo.anisotropyEnable = anisotropyEnabled ? VK_TRUE : VK_FALSE;
    createInfo.maxAnisotropy = anisotropyMax;
    createInfo.minLod = lodMin;
    createInfo.maxLod = lodMax;
    createInfo.mipLodBias = lodMipBias;
    createInfo.unnormalizedCoordinates = VK_FALSE;

    VkSampler samplerVK;
    VK_CHECK_RESULT(vkCreateSampler(_device, &createInfo, nullptr, &samplerVK));
 
    std::shared_ptr<Sampler> sampler = std::make_shared<Sampler>();
    sampler->_filterMin = filterMin;
    sampler->_filterMag = filterMag;
    sampler->_filterMip = filterMip;
    sampler->_addressModeU = addressModeU;
    sampler->_addressModeV = addressModeV;
    sampler->_addressModeW = addressModeW;
    sampler->_borderColor = borderColor;
    sampler->_compareEnabled = compareEnabled;
    sampler->_compareFunc = compareFunc;
    sampler->_anisotropyEnabled = anisotropyEnabled;
    sampler->_anisotropyMax = anisotropyMax;
    sampler->_lodMin = lodMin;
    sampler->_lodMax = lodMax;
    sampler->_lodMipBias = lodMipBias;
    sampler->_sampler = samplerVK;
    
    return sampler;
}

void Graphics::destroySampler(std::shared_ptr<Sampler> sampler)
{
    vkDestroySampler(_device, sampler->_sampler, nullptr);
    sampler.reset();
}

std::shared_ptr<Shader> Graphics::createShader(const std::string& url)
{
    std::string shaderUrl = FileSystem::getHomePath();
    shaderUrl.append(SHADER_PATH);
    shaderUrl.append(url);
    shaderUrl.append(SHADER_EXT);
    std::string shaderData = FileSystem::readAll(shaderUrl);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.codeSize = shaderData.size();
    createInfo.pCode = (uint32_t*)shaderData.c_str();
    createInfo.flags = 0;

    VkShaderModule shaderModuleVK;
    VK_CHECK_RESULT(vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModuleVK));

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->_shaderModule = shaderModuleVK;

    return shader;
}

void Graphics::destroyShader(std::shared_ptr<Shader> shader)
{
    vkDestroyShaderModule(_device, shader->_shaderModule, nullptr);
    shader.reset();
}

std::shared_ptr<DescriptorSet> Graphics::createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
                                                             size_t descriptorCount)
{
    enum { maxTypes = 12 };
    VkDescriptorPoolSize poolSizesByType[maxTypes] = {};
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
        case DescriptorSet::Descriptor::Type::eSampler:
            typeIndex = VK_DESCRIPTOR_TYPE_SAMPLER; 
            break;
        case DescriptorSet::Descriptor::Type::eTexture:
            typeIndex = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE; 
            break;
        case DescriptorSet::Descriptor::Type::eUniform:
            typeIndex = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
            break;
        }
        if (typeIndex != UINT32_MAX) 
        {
            binding->binding = descriptor->binding;
            binding->descriptorType = (VkDescriptorType)typeIndex;
            binding->descriptorCount = descriptor->count;
            binding->stageFlags = lookupVkShaderStageFlags[static_cast<uint32_t>(descriptor->shaderStages)];
            binding->pImmutableSamplers = nullptr;

            poolSizesByType[typeIndex].descriptorCount += descriptor->count;
        }
    }

    size_t poolSizeCount = 0;
    VkDescriptorPoolSize poolSizes[maxTypes] = {};
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
    VkDescriptorSet descriptorSetVK;
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(_device, &allocInfo, &descriptorSetVK));
    }

    std::shared_ptr<DescriptorSet> descriptorSet = std::make_shared<DescriptorSet>();
    descriptorSet->_descriptors.resize(descriptorCount);
    for (size_t i = 0; i < descriptorCount; ++i)
    {
        memcpy(&descriptorSet->_descriptors[i], &descriptors[i], sizeof(DescriptorSet::Descriptor));
    }
    descriptorSet->_descriptorPool = descriptorPool;
    descriptorSet->_descriptorSetLayout = descriptorSetLayout;
    descriptorSet->_descriptorSet = descriptorSetVK;
                                                                                
    GP_SAFE_FREE(bindings);

    return descriptorSet;
}

void Graphics::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet)
{
    VK_CHECK_RESULT(vkFreeDescriptorSets(_device, descriptorSet->_descriptorPool, 1, &descriptorSet->_descriptorSet));
    vkDestroyDescriptorSetLayout(_device, descriptorSet->_descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(_device, descriptorSet->_descriptorPool, nullptr);
    descriptorSet.reset();
}

std::shared_ptr<RenderPipeline> Graphics::createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
            createInfo.module = vertShader->_shaderModule;
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
            createInfo.module = tescShader->_shaderModule;
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
            createInfo.module = teseShader->_shaderModule;
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
            createInfo.module = geomShader->_shaderModule;
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
            createInfo.module = fragShader->_shaderModule;
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
        inputAttributes[inputAttributeCount].format = lookupVkFormat[static_cast<uint32_t>(attribute.format)];
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
    case RenderPipeline::PrimitiveTopology::ePointList:
        topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; 
        break;
    case RenderPipeline::PrimitiveTopology::eLineList:
        topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; 
        break;
    case RenderPipeline::PrimitiveTopology::eLineStrip:
        topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; 
        break;
    case RenderPipeline::PrimitiveTopology::eTriangleList:
        topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; 
        break;
    case RenderPipeline::PrimitiveTopology::eTriangleStrip:
        topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; 
        break;
    }

    // InputAssemblyState
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
    inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.pNext = nullptr;
    inputAssemblyStateCreateInfo.flags = 0;
    inputAssemblyStateCreateInfo.topology = topology;
        
    // RasterizerState
    VkPipelineRasterizationStateCreateInfo rasterizerStateCreateInfo = {};
    rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerStateCreateInfo.pNext = nullptr;
    rasterizerStateCreateInfo.flags = 0;
    rasterizerStateCreateInfo.depthClampEnable = rasterizerState.depthClipEnabled ? VK_TRUE : VK_FALSE;
    rasterizerStateCreateInfo.polygonMode = lookupVkPolygonMode[static_cast<uint32_t>(rasterizerState.fillMode)];
    rasterizerStateCreateInfo.cullMode = lookupVkCullModeFlags[static_cast<uint32_t>(rasterizerState.cullMode)];
    rasterizerStateCreateInfo.frontFace = lookupVkFrontFace[static_cast<uint32_t>(rasterizerState.frontFace)];
    rasterizerStateCreateInfo.depthBiasEnable = rasterizerState.depthBias == 0 ? VK_FALSE : VK_TRUE;
    rasterizerStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizerStateCreateInfo.depthBiasClamp = rasterizerState.depthBiasClamp;
    rasterizerStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    rasterizerStateCreateInfo.lineWidth = rasterizerState.lineWidth;

    // ViewportState
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.scissorCount = 1;

    // MultisampleState
    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.pNext = nullptr;
    multisampleStateCreateInfo.flags = 0;
    multisampleStateCreateInfo.rasterizationSamples = lookupVkSampleCountFlagBits[static_cast<uint32_t>(renderPass->getSampleCount())];
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.pSampleMask = 0;

    // ColorBlendState
    // TODO: support setting multiple attachments
    VkPipelineColorBlendAttachmentState colorBlendAttachmentStates[1] = {};
    colorBlendAttachmentStates[0].blendEnable = colorBlendState.blendEnabled ? VK_TRUE : VK_FALSE;
    colorBlendAttachmentStates[0].colorWriteMask = colorBlendState.colorWriteMask;
    
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pNext = nullptr;
    colorBlendStateCreateInfo.flags = 0;
    colorBlendStateCreateInfo.attachmentCount = renderPass->getColorAttachmentCount();
    colorBlendStateCreateInfo.pAttachments = colorBlendAttachmentStates;

    // DepthStencilState
    VkPipelineDepthStencilStateCreateInfo  depthStencilStateCreateInfo = {};
    depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateCreateInfo.pNext = nullptr;
    depthStencilStateCreateInfo.flags = 0;
    depthStencilStateCreateInfo.depthTestEnable = depthStencilState.depthEnabled ? VK_TRUE : VK_FALSE;
    depthStencilStateCreateInfo.depthWriteEnable = depthStencilState.depthWrite ? VK_TRUE : VK_FALSE;
    depthStencilStateCreateInfo.depthCompareOp = lookupVkCompareOp[static_cast<uint32_t>(depthStencilState.depthFunc)];
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable = depthStencilState.stencilEnabled ? VK_TRUE : VK_FALSE;
    depthStencilStateCreateInfo.front.failOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateFront.failOp)];
    depthStencilStateCreateInfo.front.passOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateFront.passOp)];
    depthStencilStateCreateInfo.front.depthFailOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateFront.depthFailOp)];
    depthStencilStateCreateInfo.front.compareOp = lookupVkCompareOp[static_cast<uint32_t>(depthStencilState.stencilOpStateFront.compareFunc)];
    depthStencilStateCreateInfo.front.compareMask = depthStencilState.stencilOpStateFront.compareMask;
    depthStencilStateCreateInfo.front.writeMask = depthStencilState.stencilOpStateFront.writeMask;
    depthStencilStateCreateInfo.front.reference = 0;
    depthStencilStateCreateInfo.back.failOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateBack.failOp)];
    depthStencilStateCreateInfo.back.passOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateBack.passOp)];
    depthStencilStateCreateInfo.back.depthFailOp = lookupVkStencilOp[static_cast<uint32_t>(depthStencilState.stencilOpStateBack.depthFailOp)];
    depthStencilStateCreateInfo.back.compareOp = lookupVkCompareOp[static_cast<uint32_t>(depthStencilState.stencilOpStateBack.compareFunc)];
    depthStencilStateCreateInfo.back.compareMask = depthStencilState.stencilOpStateBack.compareMask;
    depthStencilStateCreateInfo.back.writeMask = depthStencilState.stencilOpStateBack.writeMask;
    depthStencilStateCreateInfo.back.reference = 0;
    depthStencilStateCreateInfo.minDepthBounds = 0.0f;
    depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
    
    // DynamicStates
   std::vector<VkDynamicState> dynamicStates = 
   {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
    
    VkPipelineDynamicStateCreateInfo dynamicStatesCreateInfo = {};
    dynamicStatesCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStatesCreateInfo.pNext = nullptr;
    dynamicStatesCreateInfo.flags = 0;
    dynamicStatesCreateInfo.dynamicStateCount = dynamicStates.size();
    dynamicStatesCreateInfo.pDynamicStates = dynamicStates.data();

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = (descriptorSet != nullptr) ? 1 : 0;
    pipelineLayoutCreateInfo.pSetLayouts = (descriptorSet != nullptr) ? &(descriptorSet->_descriptorSetLayout) : nullptr;
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
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    pipelineCreateInfo.pTessellationState = nullptr;
    pipelineCreateInfo.pRasterizationState = &rasterizerStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState = &dynamicStatesCreateInfo;
    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.renderPass = renderPass->_renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    VkPipeline pipeline;
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline));

    std::shared_ptr<RenderPipeline> renderPipeline = std::make_shared<RenderPipeline>();
    renderPipeline->_primitiveTopology = primitiveTopology;
    renderPipeline->_vertexLayout = vertexLayout;
    renderPipeline->_rasterizerState = rasterizerState;
    renderPipeline->_colorBlendState = colorBlendState;
    renderPipeline->_depthStencilState = depthStencilState;
    renderPipeline->_renderPass = renderPass;
    renderPipeline->_descriptorSet = descriptorSet;
    renderPipeline->_vertShader = vertShader;
    renderPipeline->_tescShader = tescShader;
    renderPipeline->_teseShader = teseShader;
    renderPipeline->_geomShader = geomShader;
    renderPipeline->_fragShader = fragShader;
    renderPipeline->_pipelineLayout = pipelineLayout;
    renderPipeline->_pipeline = pipeline;

    return renderPipeline;

}

void Graphics::destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline)
{
    GP_ASSERT(renderPipeline);

    vkDestroyPipeline(_device, renderPipeline->_pipeline, nullptr);
    vkDestroyPipelineLayout(_device, renderPipeline->_pipelineLayout, nullptr);
    renderPipeline.reset();
}


void Graphics::initialize()
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
    createSurface();
    createSwapchain();
    createRenderPasses();
    createCommandBuffers();
    createSynchronizationObjects();

    _initialized = true;
    _resized = true;
}

void Graphics::resize(size_t width, size_t height)
{
    if (!_resized)
        return;
    _resized = false;
    VK_CHECK_RESULT(vkDeviceWaitIdle(_device));

    _width = width;
    _height = height;

    destroyRenderPasses();  
    createSwapchain();
    createRenderPasses();
    destroyCommandBuffers();
    createCommandBuffers();

    VK_CHECK_RESULT(vkDeviceWaitIdle(_device));
    _resized = true;
}

void Graphics::render(float elapsedTime)
{
    Game* game = Game::getInstance();

    // TODO: Render scene...
}

void Graphics::createInstance()
{
    // Load vulkan library
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
    
#if GP_PLATFORM_WINDOWS
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif GP_PLATFORM_LINUX
    extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif GP_PLATFORM_MACOS
    extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif GP_PLATFORM_IOS
    extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif GP_PLATFORM_ANDROID
    extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
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

    // Load the vulkan functions
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

void Graphics::createDevice()
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
        {
            score += 1000;
        }
        score += properties.limits.maxImageDimension2D;
        if (!features.geometryShader)
        {
            score = 0;
        }
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
    vkGetPhysicalDeviceProperties(_physicalDevice, &_deviceProperties);
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_deviceMemoryProperties);
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
    _queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, _queueFamilyProperties.data());

    // Get queue create infos for queues
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    // Graphics queue create info
    const float defaultQueuePriority(0.0f);
    _queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = _queueFamilyIndices.graphics;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &defaultQueuePriority;
    queueCreateInfos.push_back(queueInfo);

    // Compute queue create info (if different) and requested
    _queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
    if (_queueFamilyIndices.compute != _queueFamilyIndices.graphics)
    {
        float queuePriority(0.0f);
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = _queueFamilyIndices.compute;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        _queueFamilyIndices.compute = _queueFamilyIndices.graphics;
    }

    if ((_queueFamilyIndices.transfer != _queueFamilyIndices.graphics) && (_queueFamilyIndices.transfer != _queueFamilyIndices.compute))
    {
        // If compute family index differs, we need an additional queue create info for the compute queue
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = _queueFamilyIndices.transfer;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        _queueFamilyIndices.transfer = _queueFamilyIndices.graphics;
    }
    // Add swapchain extensions
    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // Create the logical device
    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    
    // Add validation layers/extensions
    if (_validation && isDeviceExtensionPresent(_physicalDevice, VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
    {
        extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
    }
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
    vkGetDeviceQueue(_device, _queueFamilyIndices.graphics, 0, &_queue);
    // Create the command pool
    _commandPool = createCommandPool(_queueFamilyIndices.graphics, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
}

void Graphics::createSurface()
{
    Platform* platform = Platform::getPlatform();
    uint64_t window = platform->getNativeWindow();
    uint64_t connection = platform->getNativeConnection();

    // Lookup device surface extensions
    vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
    vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
    vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(_instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));

    // Create the platform surface connection
    VkResult result;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = (HINSTANCE)connection;
    surfaceCreateInfo.hwnd = (HWND)window;
    result = vkCreateWin32SurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VkXlibSurfaceCreateInfoKHR  surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.dpy = (Display*)connection;
    surfaceCreateInfo.window = (Window)window;
    result = vkCreateXlibSurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
    VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.pView = (void*)window;
    result = vkCreateMacOSSurfaceMVK(_instance, &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
    VkIOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.pView = (void*)window;
    result = vkCreateIOSSurfaceMVK(_instance, &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.window = (ANativeWindow*)window;
    result = vkCreateAndroidSurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
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
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _surface, &supportsPresent[i]);
    }

    // Find a queue that supports both graphics and presenting
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueCount; i++) 
    {
        if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) 
        {
            if (graphicsQueueNodeIndex == UINT32_MAX)
            {
                graphicsQueueNodeIndex = i;
            }
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

    _queuePresentIndex = graphicsQueueNodeIndex;

    // Get list of supported surface formats
    uint32_t surfaceFormatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, NULL) != VK_SUCCESS || surfaceFormatCount <= 0)
        GP_ERROR("Failed to find device surface formats.");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, surfaceFormats.data()) != VK_SUCCESS)
        GP_ERROR("Failed to find device surface formats.");

    // Get color format
    if ((surfaceFormatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
    {
        _colorFormat = VK_FORMAT_B8G8R8A8_UNORM;    
        _colorSpace = surfaceFormats[0].colorSpace;
    }
    else
    {
            // iterate over the list of available surface format and
            // check for the presence of VK_FORMAT_B8G8R8A8_UNORM
            bool found_B8G8R8A8_UNORM = false;
            for (auto&& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    _colorFormat = surfaceFormat.format;
                    _colorSpace = surfaceFormat.colorSpace;
                    found_B8G8R8A8_UNORM = true;
                    break;
                }
            }

            // in case VK_FORMAT_B8G8R8A8_UNORM is not available
            // select the first available color format
            if (!found_B8G8R8A8_UNORM)
            {
                _colorFormat = surfaceFormats[0].format;
                _colorSpace = surfaceFormats[0].colorSpace;
            }
    }
}

void Graphics::createSwapchain()
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

    // Find the transformation of the surface (prefer non-rotated)
    VkSurfaceTransformFlagsKHR preTransfer;
    if (swapchainInfo.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransfer = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransfer = swapchainInfo.capabilities.currentTransform;
    }

    // Request the swapchain backbuffer image count 
    _swapchainImageCount = GP_MATH_MAX(SWAPCHAIN_IMAGE_COUNT, swapchainInfo.capabilities.minImageCount);
    
    _width = GP_MATH_CLAMP(_width, 
                   swapchainInfo.capabilities.minImageExtent.width, 
                   swapchainInfo.capabilities.maxImageExtent.width);
    _height = GP_MATH_CLAMP(_height, 
                   swapchainInfo.capabilities.minImageExtent.height, 
                   swapchainInfo.capabilities.maxImageExtent.height);

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
    if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR) || 
        (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
    {
        createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    // Create the swapchain
    VK_CHECK_RESULT(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain));

    // Get the backbuffer images for color attachments
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_swapchainImageCount, nullptr));
    _swapchainImages.resize(_swapchainImageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_swapchainImageCount, _swapchainImages.data()));

    // Get the depth stencil format
    VkBool32 validDepthFormat = getDepthStencilFormat(_physicalDevice, &_depthStencilFormat);
    if (!validDepthFormat)
        GP_ERROR("Failed to find valid depth format.");
}

void Graphics::createRenderPasses()
{
    for (size_t i = 0; i < _swapchainImageCount; ++i)
    {
        std::vector<std::shared_ptr<Texture>> colorAttachments;
        std::shared_ptr<Texture> colorAttachment = createTexture(Texture::Type::e2D, _width, _height, 1, 1,
                                                                 toFormat(_colorFormat),
                                                                 Texture::Usage::eColorAttachment,
                                                                 Texture::SampleCount::e1X, false, nullptr,
                                                                 _swapchainImages[i]);
        colorAttachments.push_back(colorAttachment);
        
        // todo:
        std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;
        std::shared_ptr<Texture> depthStencilAttachment = createTexture(Texture::Type::e2D, _width, _height, 1, 1,
                                                                        toFormat(_depthStencilFormat),
                                                                        Texture::Usage::eDepthStencilAttachment,
                                                                        Texture::SampleCount::e1X, false, nullptr,
                                                                        nullptr);
        std::vector<VkFormat> colorFormats;
        colorFormats.push_back(_colorFormat);
        std::shared_ptr<RenderPass> renderPass = createRenderPass(_width, _height, 
                                                                  1,
                                                                  colorFormats,
                                                                  _depthStencilFormat,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  colorAttachments,
                                                                  colorMultisampleAttachments,
                                                                  depthStencilAttachment);
        _renderPasses.push_back(renderPass);
    }
    _renderPass = _renderPasses[0];
}

void Graphics::destroyRenderPasses()
{
    for (size_t i = 0; i < _swapchainImageCount; ++i)
    {
        destroyRenderPass(_renderPasses[i]);
    }
    _renderPasses.clear();
    _renderPass = nullptr;
}

void Graphics::createCommandBuffers()
{
    _commandBuffers.resize(_swapchainImageCount);
    for (size_t i = 0; i < _swapchainImageCount; i++)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.commandPool = _commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBufferVK;
        VK_CHECK_RESULT(vkAllocateCommandBuffers(_device, &allocInfo, &commandBufferVK));
        std::shared_ptr<CommandBuffer> commandBuffer = std::make_shared<CommandBuffer>();
        commandBuffer->_commandBuffer = commandBufferVK;
        _commandBuffers[i] = commandBuffer;
    }
}

void Graphics::destroyCommandBuffers()
{
    for (size_t i = 0; i < _swapchainImageCount; i++)
    {
        vkFreeCommandBuffers(_device, _commandPool, 1, &_commandBuffers[i]->_commandBuffer);
    }
}

void Graphics::createSynchronizationObjects()
{
    _presentCompleteSemaphore = createSemaphore();
    _renderCompleteSemaphore = createSemaphore();
    _waitFences.resize(_swapchainImageCount);
    for (size_t i = 0; i < _swapchainImageCount; i++)
    {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VK_CHECK_RESULT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_waitFences[i]));
    }
}

Graphics::SwapchainInfo Graphics::querySwapchainInfo(VkPhysicalDevice physicalDevice)
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

VkSurfaceFormatKHR Graphics::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
    {
        return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR Graphics::choosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
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

VkBool32 Graphics::getDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat* depthStencilFormat)
{
    std::vector<VkFormat> depthStencilFormats = 
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
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

VkBool32 Graphics::isDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const char* extensionName)
{
    uint32_t extensionCount = 0;
    std::vector<VkExtensionProperties> extensions;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
    extensions.resize(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, extensions.data());
    for (auto& ext : extensions)
    {
        if (!strcmp(extensionName, ext.extensionName))
        {
            return true;
        }
    }
    return false;
}

uint32_t Graphics::getQueueFamilyIndex(VkQueueFlagBits queueFlags)
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

VkBool32 Graphics::getMemoryType(uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex)
{
    for (uint32_t i = 0; i < _deviceMemoryProperties.memoryTypeCount; i++) 
    {
        if ((typeBits & 1) == 1) 
        {
            if ((_deviceMemoryProperties.memoryTypes[i].propertyFlags &requirements_mask) == requirements_mask) 
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

VkCommandPool Graphics::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
    cmdPoolInfo.flags = createFlags;
    VkCommandPool cmdPool;
    VK_CHECK_RESULT(vkCreateCommandPool(_device, &cmdPoolInfo, nullptr, &cmdPool));
    return cmdPool;
}

VkCommandBuffer Graphics::getCommandBuffer(bool begin)
{
    VkCommandBuffer cmdBuffer;
    VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
    cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufAllocateInfo.commandPool = _commandPool;
    cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufAllocateInfo.commandBufferCount = 1;
    
    VK_CHECK_RESULT(vkAllocateCommandBuffers(_device, &cmdBufAllocateInfo, &cmdBuffer));

    // If requested, also start the new command buffer
    if (begin)
    {
        VkCommandBufferBeginInfo cmdBufferBeginInfo {};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo));
    }

    return cmdBuffer;
}

void Graphics::flushCommandBuffer(VkCommandBuffer commandBuffer)
{
    assert(commandBuffer != VK_NULL_HANDLE);

    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    // Create fence to ensure that the command buffer has finished executing
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;
    VkFence fence;
    VK_CHECK_RESULT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &fence));

    // Submit to the queue
    VK_CHECK_RESULT(vkQueueSubmit(_queue, 1, &submitInfo, fence));
    // Wait for the fence to signal that command buffer has finished executing
    VK_CHECK_RESULT(vkWaitForFences(_device, 1, &fence, VK_TRUE, UINT64_MAX));

    vkDestroyFence(_device, fence, nullptr);
    vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
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
