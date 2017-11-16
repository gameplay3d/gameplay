#pragma once

#include "Graphics.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of Graphics
 *
 * @see Graphics
 */
class GraphicsVK : public Graphics
{
public:
 
    /**
     * Constructor
     */
    GraphicsVK();

	/**
     * Destructor
	 */
    ~GraphicsVK();

	/**
     * @see Graphics::onInitialize
	 */
    void onInitialize(unsigned long window, unsigned long connection = 0);

    /**
     * @see Graphics::isInitialized
     */
    bool isInitialized();

	/**
     * @see Graphics::onResize
	 */
    void onResize(int width, int height);

    /**
     * @see Graphics::isResized
     */
    bool isResized();

	/**
	 * @see Graphics::getWidth
	 */
    int getWidth();

	/**
	 * @see Graphics::getHeight
	 */
    int getHeight();

	/**
	 * @see Graphics::getSemaphore
	 */
	std::shared_ptr<Semaphore> getSemaphore(size_t imageIndex);

	/**
	 * @see Graphics::getFence
	 */
	std::shared_ptr<Fence> getFence(size_t imageIndex);

	/**
	 * @see Graphics::getRenderPass
	 */
	std::shared_ptr<RenderPass> getRenderPass(size_t imageIndex);

	/**
	 * @see Graphics::acquireNextImage
	 */
	void acquireNextImage(std::shared_ptr<Semaphore> signalSemaphore,
						  std::shared_ptr<Fence> fence);
	/**
	 * @see Graphics::present
	 */
	void present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores);

	/**
	 * @see Graphics::waitForFence
	 */
	void waitForFence(std::shared_ptr<Fence> fence);

	/**
	 * @see Graphics::createCommandBuffer
	 */
	std::shared_ptr<CommandBuffer> createCommandBuffer();

	/**
	 * @see Graphics::destroyCommandBuffer
	 */
	void destroyCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::submit
	 */
	void submit(std::shared_ptr<CommandBuffer> commandBuffer,
			    std::vector<std::shared_ptr<Semaphore>> signalSemaphores,
				std::vector<std::shared_ptr<Semaphore>> waitSemaphores);
	/**
	 * @see Graphics::cmdBegin
	 */
	void cmdBegin(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdEnd
	 */
	void cmdEnd(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdBeginRenderPass
	 */
	void cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdEndRenderPass
	 */
	void cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdSetViewport
	 */
	void cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
						float x, float, float width, float height, 
						float depthMin, float depthMax);
	/**
	 * @see Graphics::cmdSetScissor
	 */
	void cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
					   size_t x, size_t y, 
					   size_t width, size_t height);
	/**
	 * @see Graphics::cmdClearColorAttachment
	 */
	void cmdClearColorAttachment(std::shared_ptr<CommandBuffer> commandBuffer,
							     size_t attachmentTndex, 
								 const ClearValue& clearValue);
	/**
	 * @see Graphics::cmdBindRenderPipeline
	 */
	void cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
							   std::shared_ptr<RenderPipeline> pipeline);
	/**
	 * @see Graphics::cmdBindDescriptorSet
	 */
	void cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
							  std::shared_ptr<RenderPipeline> pipeline, 
							  std::shared_ptr<DescriptorSet> descriptorSet);
	/**
	 * @see Graphics::cmdBindVertexBuffer
	 */
	void cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
							 std::shared_ptr<Buffer> vertexBuffer);
	/**
	 * @see Graphics::cmdBindVertexBuffers
	 */
	void cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
							  std::vector<std::shared_ptr<Buffer>> vertexBuffers);
	/**
	 * @see Graphics::cmdBindIndexBuffer
	 */
	void cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									std::shared_ptr<Buffer> indexBuffer);
	/**
	 * @see Graphics::cmdDraw
	 */
	void cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
						 size_t vertexCount, size_t vertexStart);
	/**
	 * @see Graphics::cmdTransitionImage
	 */
	void cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
						size_t indexCount, size_t indexStart);
	/**
	 * @see Graphics::cmdTransitionImage
	 */
	void cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
							std::shared_ptr<Texture> texture, 
							Texture::Usage usagePrev, 
							Texture::Usage usageNext);
	/**
	 * @see Graphics::cmdTransitionRenderPass
	 */
    void cmdTransitionRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
								 std::shared_ptr<RenderPass> renderPass, 
								 Texture::Usage usagePrev, 
								 Texture::Usage usageNext);
	/**
	 * @see Graphics::createSemaphore
	 */
	std::shared_ptr<Semaphore> createSemaphore();

	/**
	 * @see Graphics::destroySemaphore
	 */
	void destroySemaphore(std::shared_ptr<Semaphore> semaphore);

	/**
	 * @see Graphics::createFence
	 */
	std::shared_ptr<Fence> createFence();

	/**
	 * @see Graphics::destroyFence
	 */
	void destroyFence(std::shared_ptr<Fence> fence);

	/**
	 * @see Graphics::createVertexBuffer
	 */
	std::shared_ptr<Buffer> createVertexBuffer(size_t size, size_t vertexStride, bool hostVisible);
 	
	/**
	 * @see Graphics::createIndexBuffer
	 */
	std::shared_ptr<Buffer> createIndexBuffer(size_t size, IndexFormat indexFormat, bool hostVisible);

	/**
	 * @see Graphics::createUniformBuffer
	 */
	std::shared_ptr<Buffer> createUniformBuffer(size_t size, bool hostVisible);

	/**
	 * @see Graphics::destroyBuffer
	 */
	void destroyBuffer(std::shared_ptr<Buffer> buffer);

	/**
	 * @see Graphics::createTexture1d
	 */
	std::shared_ptr<Texture> createTexture1d(size_t width, 
											 Format pixelFormat, 
											 Texture::Usage usage, 
											 Texture::SampleCount sampleCount, 
											 bool hostVisible);
	/**
     * @see Graphics::createTexture2d
     */
	std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels,
											 Format pixelFormat, 
											 Texture::Usage usage, 
											 Texture::SampleCount sampleCount, 
											 bool hostVisible);
	/**
     * @see Graphics::createTexture3d
     */
	std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth, 
											 Format pixelFormat, 
											 Texture::Usage usage, 
											 Texture::SampleCount sampleCount,  
											 bool hostVisible);
	/**
     * @see Graphics::destroyTexture
     */
	void destroyTexture(std::shared_ptr<Texture> texture);

	/**
     * @see Graphics::createRenderPass
     */
	std::shared_ptr<RenderPass> createRenderPass(size_t width,  size_t height, 
											     size_t colorAttachmentCount,
												 Format colorFormat, 
												 Format depthStencilFormat,
												 Texture::SampleCount sampleCount);
	/**
     * @see Graphics::destroyRenderPass
     */
	void destroyRenderPass(std::shared_ptr<RenderPass> renderPass);

	/**
     * @see Graphics::createSampler
     */
	std::shared_ptr<Sampler> createSampler(Sampler::Filter filterMag,
										   Sampler::Filter filterMin,
										   Sampler::Filter filterMip,
										   Sampler::AddressMode addressModeU,
										   Sampler::AddressMode addressModeV,
										   Sampler::AddressMode addressModeW,
										   Sampler::CompareFunc compareFunc,
										   Sampler::BorderColor borderColor,
										   float anisotropyMax,
										   float lodMin,
										   float lodMax,
										   float lodMipBias);
	/**
     * @see Graphics::destroySampler
     */
	void destroySampler(std::shared_ptr<Sampler> sampler);

	/**
     * @see Graphics::createShader
     */
	std::shared_ptr<Shader> createShader(const std::string& url);

	/**
     * @see Graphics::destroyShader
     */
	void destroyShader(std::shared_ptr<Shader> shader);

	/**
     * @see Graphics::createDescriptorSet
     */
	std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
													   size_t descriptorCount);;
	/**
     * @see Graphics::destroyDescriptorSet
     */
	void destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet);

	/**
     * @see Graphics::createRenderPipeline
     */
	std::shared_ptr<RenderPipeline> createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
														 std::shared_ptr<Shader> fragShader);
	/**
     * @see Graphics::destroyRenderPipeline
     */
	void destroyRenderPipeline(std::shared_ptr<RenderPipeline> pipeline);

private:

	void createInstance();
	void createDevice();
	void createSurface(unsigned long window, unsigned long connection);
	void createSwapchain();
	void createDepthStencil();
	void createFrameBuffer();
	void createBackBuffers();
	void createCommandBuffers();
	void createSynchronizationPrimitives();
	std::shared_ptr<Buffer> createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible);
	std::shared_ptr<Texture> createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
										   Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount, bool hostVisible);
	VkFormat toFormat(Format pixelFormat);
	VkSampleCountFlagBits toSamples(Texture::SampleCount sampleCount);
	VkImageUsageFlags toImageUsageFlags(Texture::Usage usage);	

	struct SwapchainSurfaceInfo
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	void loadLibrary();
	void freeLibrary();
	void loadFunctions();
	SwapchainSurfaceInfo querySwapchainSurfaceInfo(VkPhysicalDevice physicalDevice);
	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkBool32 isDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const char* extensionName);
	VkBool32 getDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat* depthStencilFormat);
	uint32_t getQueueFamiliyIndex(VkQueueFlagBits queueFlags);
	VkBool32 getMemoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);
	VkFormatFeatureFlags toFormatFeatureFlags(VkImageUsageFlags usage);
	VkImageAspectFlags toImageAspectFlags(VkFormat format);
	static VkBool32 validationDebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
							         uint64_t srcObject, size_t location, int32_t msgCode,
							         const char* layerPrefix, const char* msg, void* userData);
private:
    bool _initialized;
    bool _resized;
	uint32_t _width;
	uint32_t _height;
	bool _fullscreen;
	bool _vsync;
	bool _multisampling;
	bool _validation;
	VkInstance _instance;
    VkSurfaceKHR _surface;
	VkPhysicalDevice _physicalDevice;
	VkPhysicalDeviceProperties _physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties _physicalDeviceMemoryProperties;
    VkDevice _device;
    std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
	uint32_t _queueFamilyIndexGraphics;
	uint32_t _queueFamilyIndexCompute;
	VkQueue _queueGraphics;
    uint32_t _queueIndex;
	struct
	{
		VkSemaphore presentComplete;
		VkSemaphore renderComplete;
	} _semaphores;
	VkSwapchainKHR _swapchain;
	uint32_t _backBufferIndex;
	uint32_t _backBufferCount;
    std::vector<VkImage> _backBufferImages;
	std::vector<VkImageView> _backBufferImageViews;
	VkColorSpaceKHR _colorSpace;
	VkFormat _colorFormat;
	VkFormat _depthStencilFormat;
	VkDeviceMemory _depthStencilMemory;
	VkImage _depthStencilImage;
	VkImageView _depthStencilImageView;
	VkRenderPass _renderPass;
	VkPipelineCache _pipelineCache;
	std::vector<VkFramebuffer> _frameBuffers;
	VkCommandPool _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;
	std::vector<VkFence> _fences;

	VkDebugReportCallbackEXT _debugMessageCallback;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	PFN_vkQueuePresentKHR vkQueuePresentKHR;
};

std::string getErrorString(VkResult errorCode);

}
