#pragma once

#include "Graphics.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan 1.x impl of Graphics
 *
 * @see Graphics
 */
class GraphicsVulkan : public Graphics
{
public:

    /**
     * Constructor
     */
    GraphicsVulkan();

	/**
	 * Destructor
	 */
    ~GraphicsVulkan();

	/**
     * @see Graphics::initialize
	 */
    void initialize(unsigned long window, unsigned long connection = 0);

	/**
     * @see Graphics::resize
	 */
    void resize(int width, int height);

	/**
     * @see Graphics::render
	 */
    void render(float elapsedTime);

    /**
     * @see Graphics::isInitialized
     */
    bool isInitialized();

	/**
	 * @see Graphics::isPrepared
	 */
    bool isPrepared();

private:

	void createInstance();
	void createDevice();
	void createSurface(unsigned long window, unsigned long connection);
	void createCommandPool();
	void createSwapchain();
	void createDepthStencil();
	void createRenderPass();
	void createFrameBuffers();
	void createCommandBuffers();
	void createSynchronizationPrimitives();
	void createPipelineCache();
	void createEditor();
	void buildCommands();

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
	static VkBool32 validationDebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
							         uint64_t srcObject, size_t location, int32_t msgCode,
							         const char* layerPrefix, const char* msg, void* userData);
private:
    bool _initialized;
	bool _prepared;
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
