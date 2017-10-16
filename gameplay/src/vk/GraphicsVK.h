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
     * @see Graphics::createCommandPool
     */
	std::shared_ptr<CommandPool> createCommandPool(bool transient = false);

	/**
     * @see Graphics::destroyCommandPool
     */
	void destroyCommandPool(std::shared_ptr<CommandPool> commandPool);

	/**
     * @see Graphics::createCommandList
     */
	std::shared_ptr<CommandList> createCommandList(std::shared_ptr<CommandPool> pool, bool secondary);

	/**
     * @see Graphics::createCommandLists
     */
	void createCommandLists(std::shared_ptr<CommandPool> pool, bool secondary, size_t count,
							std::vector<std::shared_ptr<CommandList>> out);
	/**
     * @see Graphics::destroyCommandList
     */
	void destroyCommandList(std::shared_ptr<CommandList> commandList);

	/**
     * @see Graphics::destroyCommandLists
     */
	void destroyCommandLists(std::vector<std::shared_ptr<CommandList>> commandLists);

	/**
     * @see Graphics::submitCommands
     */
	void submitCommands(std::shared_ptr<CommandList> commands);

	/**
     * @see Graphics::submitCommands
     */
	void submitCommands(std::vector<std::shared_ptr<CommandList>> commands);

    /**
     * @see Graphics::flushCommands
     */
    void flushCommands();

    /**
     * @see Graphics::present
     */
    void present();

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
     * @see Graphics::createShader
     */
	std::shared_ptr<Shader> createShader(const std::string& url);

	/**
     * @see Graphics::destroyShader
     */
	void destroyShader(std::shared_ptr<Shader> shader);

private:

	void createInstance();
	void createDevice();
	void createSurface(unsigned long window, unsigned long connection);
	void createSwapchain();
	void createDepthStencil();
	void createRenderPass();
	void createBackBuffers();
	void createCommandBuffers();
	void createSynchronizationPrimitives();
	void createPipelineCache();
	void buildCommands();
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
