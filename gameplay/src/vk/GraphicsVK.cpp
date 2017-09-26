#include "Base.h"
#include "Game.h"
#include "GraphicsVK.h"
#include "MeshVK.h"
#include "CommandListVK.h"

namespace gameplay
{

const std::vector<const char*> __validationLayers = 
{
	"VK_LAYER_LUNARG_standard_validation"
};

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal: VkResult is \"" << getErrorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
		GP_ASSERT(res == VK_SUCCESS);																		\
	}																									\
}

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
	_queueGraphics(VK_NULL_HANDLE),
	_queueIndex(0),
	_device(VK_NULL_HANDLE),
	_swapchain(VK_NULL_HANDLE),
	_backBufferIndex(0),
	_backBufferCount(0),
	_colorSpace(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR),
	_colorFormat(VK_FORMAT_UNDEFINED),
	_depthStencilFormat(VK_FORMAT_UNDEFINED),
	_depthStencilMemory(VK_NULL_HANDLE),
	_depthStencilImage(VK_NULL_HANDLE),
	_depthStencilImageView(VK_NULL_HANDLE),
	_renderPass(VK_NULL_HANDLE),
	_pipelineCache(VK_NULL_HANDLE),
	_commandPool(VK_NULL_HANDLE),
	_debugMessageCallback(VK_NULL_HANDLE)
{
}

GraphicsVK::~GraphicsVK()
{
	vkDeviceWaitIdle(_device);
	for (uint32_t i = 0; i < _frameBuffers.size(); i++)
		vkDestroyFence(_device, _fences[i], nullptr);

	vkFreeCommandBuffers(_device, _commandPool, _commandBuffers.size(), _commandBuffers.data());
	vkDestroyRenderPass(_device, _renderPass, nullptr);
	for (uint32_t i = 0; i < _frameBuffers.size(); i++)
		vkDestroyFramebuffer(_device, _frameBuffers[i], nullptr);

	vkDestroyImageView(_device, _depthStencilImageView, nullptr);
	vkDestroyImage(_device, _depthStencilImage, nullptr);
	vkFreeMemory(_device, _depthStencilMemory, nullptr);
	vkDestroyPipelineCache(_device, _pipelineCache, nullptr);
	vkDestroyCommandPool(_device, _commandPool, nullptr);
	vkDestroySemaphore(_device, _semaphores.renderComplete, nullptr);
	vkDestroySemaphore(_device, _semaphores.presentComplete, nullptr);
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
	createCommandBuffers();
	createDepthStencil();
	createRenderPass();
	createBackBuffers();
	createSynchronizationPrimitives();
	createPipelineCache();

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

    // Destroy depth/stencil
	if (_depthStencilImageView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(_device, _depthStencilImageView, nullptr);
		_depthStencilImageView = VK_NULL_HANDLE;
	}
	if (_depthStencilImage != VK_NULL_HANDLE)
	{
		vkDestroyImage(_device, _depthStencilImage, nullptr);
		_depthStencilImage = VK_NULL_HANDLE;
	}
	if (_depthStencilMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(_device, _depthStencilMemory, nullptr);
		_depthStencilMemory = VK_NULL_HANDLE;
	}
	createDepthStencil();

    // Destroy framebuffer
	for (uint32_t i = 0; i < _backBufferCount; i++)
	{
		if (_frameBuffers[i] != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(_device, _frameBuffers[i], nullptr);
			_frameBuffers[i] = VK_NULL_HANDLE;
		}
	}
    // Destroy the render passes
	_frameBuffers.clear();
	if (_renderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(_device, _renderPass, nullptr);
		_renderPass = VK_NULL_HANDLE;
	}
	createRenderPass();
	createBackBuffers();

	// Recreate swapchain
	if (_swapchain != VK_NULL_HANDLE) 
	{ 
		for (uint32_t i = 0; i < _backBufferCount; i++)
		{
			vkDestroyImageView(_device, _backBufferImageViews[i], nullptr);
		}
		vkDestroySwapchainKHR(_device, _swapchain, nullptr);
		_swapchain = VK_NULL_HANDLE;
	}
	createSwapchain();

    // destroy command buffers
	vkFreeCommandBuffers(_device, _commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
	_commandBuffers.clear();
	createCommandBuffers();

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

std::shared_ptr<Mesh> GraphicsVK::createMesh(const VertexFormat& vertexFormat, size_t vertexCount, bool dynamic)
{
	VkBuffer vertexBufferVk = nullptr;
	// TODO

	std::shared_ptr<MeshVK> vertexBuffer = std::make_shared<MeshVK>(vertexFormat, vertexCount, dynamic, _device, vertexBufferVk);
	return std::static_pointer_cast<Mesh>(vertexBuffer);
}

std::shared_ptr<CommandList> GraphicsVK::createCommandList()
{
	return nullptr;
}

void GraphicsVK::submitCommandLists(std::shared_ptr<CommandList>* commandLists, size_t count)
{
}

bool GraphicsVK::beginScene()
{
	return true;
}

void GraphicsVK::endScene()
{
}

void GraphicsVK::present()
{
}

void GraphicsVK::flushAndWait()
{
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

	// Get the graphics queue
	vkGetDeviceQueue(_device, _queueFamilyIndexGraphics, 0, &_queueGraphics);
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

    // Create command pool
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = _queueFamilyIndexGraphics;
    VK_CHECK_RESULT(vkCreateCommandPool(_device, &info, nullptr, &_commandPool));
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
    /*
	if (surfaceInfo.capabilities.currentExtent.width != (uint32_t) -1)
	{
		_width = surfaceInfo.capabilities.currentExtent.width;
		_height = surfaceInfo.capabilities.currentExtent.height;
	}
    */

	// Request the swapchain backbuffer image count 
	_backBufferCount = surfaceInfo.capabilities.minImageCount;
	if (surfaceInfo.capabilities.maxImageCount > 0 && _backBufferCount > surfaceInfo.capabilities.maxImageCount) 
		_backBufferCount = surfaceInfo.capabilities.maxImageCount;

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
	createInfo.minImageCount = _backBufferCount;
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

	// Get the format properties
	VkFormatProperties formatProps;
	vkGetPhysicalDeviceFormatProperties(_physicalDevice, _colorFormat, &formatProps);
	if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)
		createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	// Create the swapchain
	VK_CHECK_RESULT(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain));

	// Get the backbuffer images
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_backBufferCount, NULL));
	_backBufferImages.resize(_backBufferCount);
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(_device, _swapchain, &_backBufferCount, _backBufferImages.data()));

	// Create backbuffer image views
	_backBufferImageViews.resize(_backBufferCount);
	for (uint32_t i = 0; i < _backBufferCount; i++)
	{
		VkImageViewCreateInfo colorAttachmentView = {};
		colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorAttachmentView.pNext = nullptr;
		colorAttachmentView.format = _colorFormat;
		colorAttachmentView.components = 
		{
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A
		};
		colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorAttachmentView.subresourceRange.baseMipLevel = 0;
		colorAttachmentView.subresourceRange.levelCount = 1;
		colorAttachmentView.subresourceRange.baseArrayLayer = 0;
		colorAttachmentView.subresourceRange.layerCount = 1;
		colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorAttachmentView.flags = 0;
		colorAttachmentView.image = _backBufferImages[i];
		VK_CHECK_RESULT(vkCreateImageView(_device, &colorAttachmentView, nullptr, &_backBufferImageViews[i]));
	}
}

void GraphicsVK::createDepthStencil()
{
	// Create a depth stencil image and view
	VkBool32 validDepthFormat = getDepthStencilFormat(_physicalDevice, &_depthStencilFormat);
	if (!validDepthFormat)
		GP_ERROR("Failed to find valid depth format.");

	// Create depth/stencil image
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.format = _depthStencilFormat;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.extent.width = (uint32_t)_width;
    imageCreateInfo.extent.height = (uint32_t)_height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.flags = 0;
	
	VK_CHECK_RESULT(vkCreateImage(_device, &imageCreateInfo, NULL, &_depthStencilImage));

	// Allocate memory for the depth/stencil image
	VkMemoryRequirements memoryReqs;
	vkGetImageMemoryRequirements(_device, _depthStencilImage, &memoryReqs);
	VkMemoryAllocateInfo memoryAlloc = {};
    memoryAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAlloc.pNext = nullptr;
    memoryAlloc.allocationSize = 0;
    memoryAlloc.memoryTypeIndex = 0;
	memoryAlloc.allocationSize = memoryReqs.size;

	if (!getMemoryTypeFromProperties(memoryReqs.memoryTypeBits, 0, &memoryAlloc.memoryTypeIndex))
		GP_ERROR("Failed to find compatible memory for depth stencil buffer.");
	VK_CHECK_RESULT(vkAllocateMemory(_device, &memoryAlloc, nullptr, &_depthStencilMemory));
	VK_CHECK_RESULT(vkBindImageMemory(_device, _depthStencilImage, _depthStencilMemory, 0));
	
	// Create the depth/stencil image view
	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = NULL;
    imageViewCreateInfo.image = VK_NULL_HANDLE;
    imageViewCreateInfo.format = _depthStencilFormat;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = _depthStencilImage;

	VK_CHECK_RESULT(vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &_depthStencilImageView));
}

void GraphicsVK::createRenderPass()
{
	std::array<VkAttachmentDescription, 2> attachments = {};

	// Color attachment
	attachments[0].format = _colorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Depth/stencil attachment
	attachments[1].format = _depthStencilFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Setup attachment references
	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkAttachmentReference depthStencilReference = {};
	depthStencilReference.attachment = 1;
	depthStencilReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Setup a single subpass reference
	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;	
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;	
	subpassDescription.pDepthStencilAttachment = &depthStencilReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;
	subpassDescription.pResolveAttachments = nullptr;

	// Setup subpass dependencies
	std::array<VkSubpassDependency, 2> dependencies;

	// First dependency at the start of the renderpass 
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;			
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// Second dependency at the end the renderpass
	dependencies[1].srcSubpass = 0;	
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// Create the actual renderpass
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VK_CHECK_RESULT(vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass));
}

void GraphicsVK::createBackBuffers()
{
	_frameBuffers.resize(_backBufferCount);
	for (size_t i = 0; i < _backBufferCount; i++)
	{
		std::array<VkImageView, 2> attachments;										
		attachments[0] = _backBufferImageViews[i];
		attachments[1] = _depthStencilImageView;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.renderPass = _renderPass;
		frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		frameBufferCreateInfo.pAttachments = attachments.data();
		frameBufferCreateInfo.width = _width;
		frameBufferCreateInfo.height =_height;
		frameBufferCreateInfo.layers = 1;
		VK_CHECK_RESULT(vkCreateFramebuffer(_device, &frameBufferCreateInfo, nullptr, &_frameBuffers[i]));
	}
}

void GraphicsVK::createSynchronizationPrimitives()
{
	// Create the semaphores
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	VK_CHECK_RESULT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_semaphores.presentComplete));
	VK_CHECK_RESULT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_semaphores.renderComplete));

	// Create the fences
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	_fences.resize(_commandBuffers.size());
	for (auto& fence : _fences)
		VK_CHECK_RESULT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &fence));
}

void GraphicsVK::createPipelineCache()
{
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	VK_CHECK_RESULT(vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_pipelineCache));
}

void GraphicsVK::buildCommands()
{
	// TODO: change this to builder pattern
	VkCommandBufferBeginInfo cmdBufInfo = {};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufInfo.pNext = nullptr;

	VkClearColorValue clearColor = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	VkClearDepthStencilValue clearDepthStencil = { 1.0f, 0 };

	VkImageSubresourceRange subResourceRange = {};
	subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subResourceRange.baseMipLevel = 0;
	subResourceRange.levelCount = 1;
	subResourceRange.baseArrayLayer = 0;
	subResourceRange.layerCount = 1;

	for (int32_t i = 0; i < _commandBuffers.size(); ++i)
	{
		VK_CHECK_RESULT(vkBeginCommandBuffer(_commandBuffers[i], &cmdBufInfo));

		vkCmdClearColorImage(_commandBuffers[i], _backBufferImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subResourceRange);
		vkCmdClearDepthStencilImage(_commandBuffers[i], _depthStencilImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearDepthStencil, 1, &subResourceRange);

		VK_CHECK_RESULT(vkEndCommandBuffer(_commandBuffers[i]));
	}
}

void GraphicsVK::createCommandBuffers()
{
	_commandBuffers.resize(_backBufferCount);
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandPool = _commandPool;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	info.commandBufferCount = _backBufferCount;
	VK_CHECK_RESULT(vkAllocateCommandBuffers(_device, &info, _commandBuffers.data()));
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
