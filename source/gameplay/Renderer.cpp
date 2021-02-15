#include "Renderer.h"
#include "Config.h"
#include "FileSystem.h"
#include "Logging.h"
#include "RendererVK.h"
#include "AppGLFW.h"
#include "Path.h"
#include "Window.h"
#include "imgui_impl_vulkan.h"
#include <vector>

#define GP_RENDERER_VALIDATION false
#define GP_RENDERER_MIN_IMAGE_COUNT 2

namespace gameplay
{

struct RenderFrame
{
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkFence fence{VK_NULL_HANDLE};
    VkImage backbuffer{VK_NULL_HANDLE};
    VkImageView backbufferView{VK_NULL_HANDLE};
    VkFramebuffer framebuffer{VK_NULL_HANDLE};
    VkSemaphore imageAcquiredSemaphore{VK_NULL_HANDLE};
    VkSemaphore renderCompleteSemaphore{VK_NULL_HANDLE};
};

struct Renderer::Impl
{   
    bool validation{GP_RENDERER_VALIDATION};
    uint32_t minImageCount{GP_RENDERER_MIN_IMAGE_COUNT};
    GLFWwindow* glfwWindow{nullptr};
    VkAllocationCallbacks* allocator{nullptr};
    VkDebugReportCallbackEXT debugReport{nullptr}; 
    VkInstance instance{VK_NULL_HANDLE};
    VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};
    VkDevice device{VK_NULL_HANDLE};
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    struct
    {
        uint32_t present{0};
        uint32_t graphics{0};
        uint32_t compute{0};
        uint32_t transfer{0};
    } queueFamilyIndices;
    VkQueue queue{VK_NULL_HANDLE};
    int surfaceWidth{0};
    int surfaceHeight{0};
    VkSurfaceKHR surface{VK_NULL_HANDLE};
    VkSurfaceFormatKHR surfaceFormat;
    VkColorSpaceKHR colorSpace;
    VkSwapchainKHR swapchain{VK_NULL_HANDLE};
    VkPresentModeKHR presentMode;
    VkRenderPass renderPass{VK_NULL_HANDLE};
    VkPipeline pipeline{VK_NULL_HANDLE};
    bool clearEnable{true};
    VkClearValue clearValue = {};
    uint32_t frameIndex{0};
    uint32_t semaphoreIndex{0};
    VkFormat imageFormat;
    uint32_t imageCount{0};
    std::vector<RenderFrame> frames;
    VkPipelineCache pipelineCache{VK_NULL_HANDLE};
    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
    bool rebuildSwapchain{false};

    void startup_vulkan();
    void shutdown_vulkan();
    void startup_window_surface();
    void shutdown_window_surface();
    void create_or_resize_window_swapchain();
    void destroy_render_frame(RenderFrame* renderFrame);
    uint32_t get_queue_family_index(VkQueueFlagBits queueFlags);
    VkBool32 is_device_extension_present(VkPhysicalDevice physicalDevice, const char* extensionName);
    VkSurfaceFormatKHR select_surface_format(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkFormat* requestFormats, int requestFormatCount, VkColorSpaceKHR requestColorSpace);
    VkPresentModeKHR select_present_mode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,  const VkPresentModeKHR* requestModes, int requestModeCount);
    uint32_t get_image_count_for_present_mode(VkPresentModeKHR present_mode);
    void next_frame();
    void present_frame();
    void startup_imgui();
    void shutdow_imgui();
    void render_imgui();
};

Renderer::Renderer()
{
    _impl = std::make_unique<Renderer::Impl>();
    auto config = App::get_app()->get_config();
    _impl->validation = config->set_bool("graphics.validation", false);
    _impl->minImageCount = (uint32_t)config->set_int("graphics.minImageCount", 2);
}

Renderer::~Renderer()
{
}

void Renderer::startup()
{
    if (!glfwVulkanSupported())
    {
        GP_LOG_ERROR("GLFW: Vulkan Not Supported");
    }
    _impl->startup_vulkan();
    _impl->startup_window_surface();
    _impl->startup_imgui();
}

void Renderer::Impl::startup_vulkan()
{
    load_vulkan_library();

    std::vector<const char*> instanceExtension;
    // get the required surface extensions from glfw
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for ( uint32_t i = 0; i < glfwExtensionCount; i++)
    {
        instanceExtension.push_back(glfwExtensions[i]);
    }

    // initialize the vulkan application
    auto fs = App::get_app()->get_file_system();
    std::string appName = Path(fs->get_app_executable_path()).get_stem();
    VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.pEngineName = appName.c_str();
	appInfo.apiVersion = VK_API_VERSION_1_0;

    // create the vulkan instance
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (instanceExtension.size() > 0)
    {
        if (validation)
        {
            instanceExtension.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtension.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtension.data();
    }
    if (validation)
    {
        instanceCreateInfo.enabledLayerCount = (uint32_t)vulkan_validation_layers.size();
        instanceCreateInfo.ppEnabledLayerNames = vulkan_validation_layers.data();
    }
    VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, allocator, &instance));

    // register validation debug report
    if (validation)
    {
        vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
        vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
        vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT"));
        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)log_vulkan_validation_debug_report;
        dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        VK_CHECK_RESULT(vkCreateDebugReportCallbackEXT(instance, &dbgCreateInfo, allocator, &debugReport));
    }

    // load functions (needed on some platgforms)
    load_vulkan_functions();

    // get the available gpus
    uint32_t gpuCount = 0;
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
    GP_ASSERT(gpuCount > 0);
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

    // rate the physical devices based on  important properties and features
    std::map<int, VkPhysicalDevice> physicalDevicesRated;
    for (const auto& physicalDev : physicalDevices)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDev, &properties);
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDev, &features);
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
        physicalDevicesRated[score] = physicalDev;
    }
    // take the first device from rated devices that support our queue requirements
    physicalDevice = physicalDevicesRated.begin()->second;

    // get properties various properties of the physical device
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    // get queue create infos for queues
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    // graphics queue create info
    const float defaultQueuePriority(0.0f);
    queueFamilyIndices.graphics = get_queue_family_index(VK_QUEUE_GRAPHICS_BIT);
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &defaultQueuePriority;
    queueCreateInfos.push_back(queueInfo);

    // compute queue create info (if different) and requested
    queueFamilyIndices.compute = get_queue_family_index(VK_QUEUE_COMPUTE_BIT);
    if (queueFamilyIndices.compute != queueFamilyIndices.graphics)
    {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        queueFamilyIndices.compute = queueFamilyIndices.graphics;
    }

    if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute))
    {
        // if compute family index differs, we need an additional queue create info for the compute queue
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        queueFamilyIndices.transfer = queueFamilyIndices.graphics;
    }
        
    // Create the logical device
    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // Add validation layers/extensions
    if (validation && is_device_extension_present(physicalDevice, VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
    {
        deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
    }
    
    if (deviceExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }
    else
    {
        deviceCreateInfo.enabledExtensionCount = 0;
    }
    if (validation)
    {
        deviceCreateInfo.enabledLayerCount = (uint32_t)vulkan_validation_layers.size();
        deviceCreateInfo.ppEnabledLayerNames = vulkan_validation_layers.data();
    }
    else
    {
        deviceCreateInfo.enabledLayerCount = 0;
    }
    // create device and get the queue
    VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));
    vkGetDeviceQueue(device, queueFamilyIndices.graphics, 0, &queue);

    // setup and descriptor pool
    VkDescriptorPoolSize poolSizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * (uint32_t)GP_COUNTOF(poolSizes);
    poolInfo.poolSizeCount = (uint32_t)GP_COUNTOF(poolSizes);
    poolInfo.pPoolSizes = poolSizes;
    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, allocator, &descriptorPool));
}

void Renderer::Impl::shutdown_vulkan()
{
    vkDestroyDescriptorPool(device, descriptorPool, allocator);
    if (validation)
    {
        vkDestroyDebugReportCallbackEXT(instance, debugReport, allocator);
    }
    vkDestroyDevice(device, allocator);
    vkDestroyInstance(instance, allocator);
}

void Renderer::Impl::startup_window_surface()
{
    glfwWindow = App::get_app()->get_window()->handle->glfwWindow;

    // create the window surface
    VK_CHECK_RESULT(glfwCreateWindowSurface(instance, glfwWindow, allocator, &surface));

    // get the framebuffer size
    glfwGetFramebufferSize(glfwWindow, &surfaceWidth, &surfaceHeight);

    // lookup device surface and swapchain extensions
    vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
    vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
    vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
    vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR"));
    vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetInstanceProcAddr(instance, "vkDestroySwapchainKHR"));
    vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetInstanceProcAddr(instance, "vkGetSwapchainImagesKHR"));
    vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetInstanceProcAddr(instance, "vkAcquireNextImageKHR"));
    vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetInstanceProcAddr(instance, "vkQueuePresentKHR"));

    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndices.graphics, surface, &res);
    if (res != VK_TRUE)
    {
        GP_LOG_ERROR("Failed vkGetPhysicalDeviceSurfaceSupportKHR");
        return;
    }
    // select surface format
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    surfaceFormat = select_surface_format(physicalDevice, surface, requestSurfaceImageFormat, (uint32_t)GP_COUNTOF(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // get the present mode and image count for the present mode
    VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
    presentMode = select_present_mode(physicalDevice, surface, &presentModes[0], (uint32_t)GP_COUNTOF(presentModes));

    // create the swapchain with render frames
    create_or_resize_window_swapchain();
}


void Renderer::Impl::shutdown_window_surface()
{
    vkDeviceWaitIdle(device);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        destroy_render_frame(&frames[i]);
    }
    frames.clear();
    vkDestroyPipeline(device, pipeline, allocator);
    vkDestroyRenderPass(device, renderPass, allocator);
    vkDestroySwapchainKHR(device,swapchain, allocator);
    vkDestroySurfaceKHR(instance, surface, allocator);
}

uint32_t Renderer::Impl::get_queue_family_index(VkQueueFlagBits queueFlags)
{
    if (queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
        {
            if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }
    for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
    {
        if (queueFamilyProperties[i].queueFlags & queueFlags)
        {
            return i;
            break;
        }
    }
    return 0;
}

VkBool32 Renderer::Impl::is_device_extension_present(VkPhysicalDevice physicalDevice, const char* extensionName)
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

VkSurfaceFormatKHR Renderer::Impl::select_surface_format(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkFormat* requestFormats, int requestFormatCount, VkColorSpaceKHR requestColorSpace)
{
    uint32_t availSurfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &availSurfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> availFormats;
    availFormats.resize((int)availSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &availSurfaceFormatCount, availFormats.data());

    if (availSurfaceFormatCount == 1)
    {
        if (availFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            VkSurfaceFormatKHR ret;
            ret.format = requestFormats[0];
            ret.colorSpace = requestColorSpace;
            return ret;
        }
        else
        {
            return availFormats[0];
        }
    }
    else
    {
        for (int i = 0; i < requestFormatCount; i++)
        {
            for (uint32_t i = 0; i < availSurfaceFormatCount; i++)
            {
                if (availFormats[i].format == requestFormats[i] && availFormats[i].colorSpace == requestColorSpace)
                {
                    return availFormats[i];
                }
            }
        }
        return availFormats[0];
    }
}

VkPresentModeKHR Renderer::Impl::select_present_mode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkPresentModeKHR* requestModes, int requestModeCount)
{
    // request a certain mode and confirm that it is available. If not use VK_PRESENT_MODE_FIFO_KHR which is mandatory
    uint32_t availModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &availModeCount, nullptr);
    std::vector<VkPresentModeKHR> availModes;
    availModes.resize((int)availModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &availModeCount, availModes.data());

    for (int req = 0; req < requestModeCount; req++)
    {
        for (uint32_t avail = 0; avail < availModeCount; avail++)
        {
            if (requestModes[req] == availModes[avail])
            {
                return requestModes[req];
            }
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t Renderer::Impl::get_image_count_for_present_mode(VkPresentModeKHR mode)
{
    switch(mode)
    {
    case VK_PRESENT_MODE_MAILBOX_KHR:
        return 3;
    case VK_PRESENT_MODE_FIFO_KHR:
    case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
        return 2;
    case VK_PRESENT_MODE_IMMEDIATE_KHR:
        return 1;
    default:
        return 1;
    }
}

void Renderer::Impl::create_or_resize_window_swapchain()
{
    // preserve previous swa
    VkSwapchainKHR oldSwapchain = swapchain;
    swapchain = VK_NULL_HANDLE;
    VK_CHECK_RESULT(vkDeviceWaitIdle(device));

    // cleanup previous swapchain resources
    for (size_t i = 0; i <imageCount; i++)
    {
        destroy_render_frame(&frames[i]);
    }
    if (renderPass)
    {
        vkDestroyRenderPass(device, renderPass, allocator);
    }
    if (pipeline)
    {
        vkDestroyPipeline(device, pipeline, allocator);
    }
    uint32_t minImageCountForMode = get_image_count_for_present_mode(presentMode);
    if (minImageCount < minImageCountForMode)
    {
        minImageCount = minImageCountForMode;
    }

    // setup new swapchain
    VkSwapchainCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface = surface;
    info.minImageCount = minImageCount;
    info.imageFormat = surfaceFormat.format;
    info.imageColorSpace = surfaceFormat.colorSpace;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;           // Assume that graphics family == present family
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = presentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = swapchain;
    VkSurfaceCapabilitiesKHR surfaceCaps;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps));
    if (info.minImageCount < surfaceCaps.minImageCount)
    {
        info.minImageCount = surfaceCaps.minImageCount;
    }
    else if (surfaceCaps.maxImageCount != 0 && info.minImageCount > surfaceCaps.maxImageCount)
    {
        info.minImageCount = surfaceCaps.maxImageCount;
    }

    if (surfaceCaps.currentExtent.width == 0xffffffff)
    {
        info.imageExtent.width = surfaceWidth;
        info.imageExtent.height = surfaceHeight;
    }
    else
    {
        info.imageExtent.width = surfaceWidth = surfaceCaps.currentExtent.width;
        info.imageExtent.height = surfaceHeight = surfaceCaps.currentExtent.height;
    }
    VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &info, allocator, &swapchain));
    

    // get the number of back buffers
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL));
    frames.resize(imageCount);

    // get the back buffer images and assign into the render frames
    std::vector<VkImage> backbuffers;
    backbuffers.resize(imageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, backbuffers.data()));    
    for (uint32_t i = 0; i < imageCount; i++)
    {
        frames[i].backbuffer = backbuffers[i];
    }
    // destroy any previous swapchain that may exist
    if (oldSwapchain)
    {
        vkDestroySwapchainKHR(device, oldSwapchain, allocator);
    }

    // create a render pass for ui
    {
        VkAttachmentDescription attachment = {};
        attachment.format = surfaceFormat.format;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = clearEnable ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        VK_CHECK_RESULT(vkCreateRenderPass(device, &info, allocator, &renderPass));
    }

    // create the iamge view
    {
        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = surfaceFormat.format;
        info.components.r = VK_COMPONENT_SWIZZLE_R;
        info.components.g = VK_COMPONENT_SWIZZLE_G;
        info.components.b = VK_COMPONENT_SWIZZLE_B;
        info.components.a = VK_COMPONENT_SWIZZLE_A;
        VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        info.subresourceRange = image_range;
        for (uint32_t i = 0; i < imageCount; i++)
        {
            RenderFrame* renderFrame = &frames[i];
            info.image = renderFrame->backbuffer;
            VK_CHECK_RESULT(vkCreateImageView(device, &info, allocator, &renderFrame->backbufferView));
        }
    }

    // create the framebuffer
    {
        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = renderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = surfaceWidth;
        info.height = surfaceHeight;
        info.layers = 1;
        for (uint32_t i = 0; i < imageCount; i++)
        {
            RenderFrame* renderFrame = &frames[i];
            attachment[0] = renderFrame->backbufferView;
            VK_CHECK_RESULT(vkCreateFramebuffer(device, &info, allocator, &renderFrame->framebuffer));
        }
    }

    // create command buffers for each render frame
    for (uint32_t i = 0; i < imageCount; i++)
    {
        RenderFrame* renderFrame = &frames[i];
        // create the command pool
        {
            VkCommandPoolCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            info.queueFamilyIndex = queueFamilyIndices.graphics;
            VK_CHECK_RESULT(vkCreateCommandPool(device, &info, allocator, &renderFrame->commandPool));
        }
        // create the command buffer
        {
            VkCommandBufferAllocateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.commandPool = renderFrame->commandPool;
            info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            info.commandBufferCount = 1;
            VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &info, &renderFrame->commandBuffer));
        }
        // create the fence
        {
            VkFenceCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VK_CHECK_RESULT(vkCreateFence(device, &info, allocator, &renderFrame->fence));
        }
        // create the semaphore
        {
            VkSemaphoreCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            VK_CHECK_RESULT(vkCreateSemaphore(device, &info, allocator, &renderFrame->imageAcquiredSemaphore));
            VK_CHECK_RESULT(vkCreateSemaphore(device, &info, allocator, &renderFrame->renderCompleteSemaphore));
        }
    }
}

void Renderer::Impl::destroy_render_frame(RenderFrame* renderFrame)
{
    vkDestroyFence(device, renderFrame->fence, allocator);
    vkFreeCommandBuffers(device, renderFrame->commandPool, 1, &renderFrame->commandBuffer);
    vkDestroyCommandPool(device, renderFrame->commandPool, allocator);
    renderFrame->fence = VK_NULL_HANDLE;
    renderFrame->commandBuffer = VK_NULL_HANDLE;
    renderFrame->commandPool = VK_NULL_HANDLE;

    vkDestroyImageView(device, renderFrame->backbufferView, allocator);
    vkDestroyFramebuffer(device, renderFrame->framebuffer, allocator);

    vkDestroySemaphore(device, renderFrame->imageAcquiredSemaphore, allocator);
    vkDestroySemaphore(device, renderFrame->renderCompleteSemaphore, allocator);
    renderFrame->imageAcquiredSemaphore = renderFrame->renderCompleteSemaphore = VK_NULL_HANDLE;
}

void Renderer::Impl::next_frame()
{
    if (rebuildSwapchain)
    {
        glfwGetFramebufferSize(glfwWindow, &surfaceWidth, &surfaceWidth);
        if (surfaceWidth > 0 && surfaceHeight > 0)
        {
            create_or_resize_window_swapchain();
            frameIndex = 0;
            rebuildSwapchain = false;
        }
    }
}

void Renderer::Impl::present_frame()
{
     if (rebuildSwapchain)
     {
        return;
     }
    VkSemaphore render_complete_semaphore = frames[semaphoreIndex].renderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &swapchain;
    info.pImageIndices = &frameIndex;
    VkResult res = vkQueuePresentKHR(queue, &info);
    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        rebuildSwapchain = true;
        return;
    }
    check_vulkan_result(res);
    semaphoreIndex = (semaphoreIndex + 1) % imageCount; 
}

void Renderer::Impl::startup_imgui()
{
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.QueueFamily = queueFamilyIndices.graphics;
    init_info.Queue = queue;
    init_info.PipelineCache = pipelineCache;
    init_info.DescriptorPool = descriptorPool;
    init_info.Allocator = allocator;
    init_info.MinImageCount = minImageCount;
    init_info.ImageCount = imageCount;
    init_info.CheckVkResultFn = check_vulkan_result;
    ImGui_ImplVulkan_Init(&init_info, renderPass);

    // upload the fonts
    {
        // Use any command queue
        VkCommandPool commandPool = frames[frameIndex].commandPool;
        VkCommandBuffer commandBuffer = frames[frameIndex].commandBuffer;

        VK_CHECK_RESULT(vkResetCommandPool(device, commandPool, 0));
        
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo endInfo = {};
        endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        endInfo.commandBufferCount = 1;
        endInfo.pCommandBuffers = &commandBuffer;
        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
        
        VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &endInfo, VK_NULL_HANDLE));
        
        VK_CHECK_RESULT(vkDeviceWaitIdle(device));

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

void Renderer::Impl::shutdow_imgui()
{
    vkDeviceWaitIdle(device);
    ImGui_ImplVulkan_Shutdown();
}

void Renderer::Impl::render_imgui()
{
    VkSemaphore imageAcquiredSemaphore = frames[semaphoreIndex].imageAcquiredSemaphore;
    VkSemaphore renderCompleteSemaphore = frames[semaphoreIndex].renderCompleteSemaphore;
    VkResult res = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &frameIndex);
    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        rebuildSwapchain = true;
        return;
    }
    check_vulkan_result(res);

    RenderFrame* frame = &frames[frameIndex];

    VK_CHECK_RESULT(vkWaitForFences(device, 1, &frame->fence, VK_TRUE, UINT64_MAX));
    VK_CHECK_RESULT(vkResetFences(device, 1, &frame->fence));
    VK_CHECK_RESULT(vkResetCommandPool(device, frame->commandPool, 0));

    // begin a command buffer
    {
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK_RESULT(vkBeginCommandBuffer(frame->commandBuffer, &info));
    }

    // begin a render pass
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = renderPass;
        info.framebuffer = frame->framebuffer;
        info.renderArea.extent.width = surfaceWidth;
        info.renderArea.extent.height = surfaceHeight;
        info.clearValueCount = 1;
        info.pClearValues = &clearValue;
        vkCmdBeginRenderPass(frame->commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // build commands from imgui draw data
    ImDrawData* drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, frame->commandBuffer);

    // end the render pass
    vkCmdEndRenderPass(frame->commandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &imageAcquiredSemaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &frame->commandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &renderCompleteSemaphore;

        VK_CHECK_RESULT(vkEndCommandBuffer(frame->commandBuffer));
        VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &info, frame->fence));
    }
}

void Renderer::shutdown()
{
    _impl->shutdow_imgui();
    _impl->shutdown_window_surface();
    _impl->shutdown_vulkan();
    unload_vulkan_library();
}

void Renderer::next_frame()
{
    _impl->next_frame();
}

void Renderer::update()
{
    // update the scene with ecs
}

void Renderer::render_frame()
{
    _impl->render_imgui();

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Renderer::present_frame()
{
    _impl->present_frame();
}
}
