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
     * Vulkan impl of Graphics::CommandList
     *
     * @see Graphics::CommandList
     */
    class CommandListVulkan : public CommandList
    {
    public:
        /**
         * Constructor.
         */
        CommandListVulkan();

        /**
         * @see CommandList::begin
         */
        void begin();

        /**
         * @see CommandList::end
         */
        void end();

        /**
         * @see CommandList::beginRenderPass
         */
        void beginRenderPass(RenderPass* pass);

        /**
         * @see CommandList::endRenderPass
         */
        void endRenderPass();

        /**
         * @see CommandList::setViewport
         */
        void setViewport(float x, float y, float width, float height, float depthMin, float depthMax);

        /**
         * @see CommandList::setScissor
         */
        void setScissor(float x, float y, float width, float height);

        /**
         * @see CommandList::bindPipeline
         */
        void bindPipeline(Pipeline* pipeline);

        /**
         * @see CommandList::bindDescriptorSet
         */
        void bindDescriptorSet(DescriptorSet* set);

        /**
         * @see CommandList::bindIndexBuffer
         */
        void bindIndexBuffer(Buffer* buffer);

        /**
         * @see CommandList::bindVertexBuffers
         */
        void bindVertexBuffers(Buffer** buffers, size_t bufferCount);

        /**
         * @see CommandList::bindVertexBuffers
         */
        void draw(size_t vertexCount, size_t vertexOffset);

        /**
         * @see CommandList::drawInstanced
         */
        void drawInstanced(size_t vertexCount, size_t vertexOffset);
    };

    /**
     * Vulkan impl of Graphics::CommandPool
     *
     * @see Graphics::CommandPool
     */
    class CommandPoolVulkan : public CommandPool
    {
    public:
        /**
         * Constructor.
         */
        CommandPoolVulkan();

        /**
         * Creates a command list.
         *
         * @return A command list.
         */
        Graphics::CommandList* createCommandList();

        /**
         * Destroys a command list.
         *
         * @param commandList The list of commands to be destroyed.
         */
        void destroyCommandList(Graphics::CommandList* commandList);
    };


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
     * @see Graphics::createDescriptorSet
     */
    Graphics::DescriptorSet* createDescriptorSet(Descriptor* descriptors, size_t descriptorCount);

    /**
     * @see Graphics::createVertexBuffer
     */
    Graphics::Buffer* createVertexBuffer(size_t count, bool hostVisible, VertexLayout vertexLayout);

    /**
     * @see Graphics::createIndexBuffer
     */
    Graphics::Buffer* createIndexBuffer(size_t count, bool hostVisible, IndexType indexType);

    /**
     * @see Graphics::createUniformBuffer
     */
    Graphics::Buffer* createUniformBuffer(size_t size, bool hostVisible, Buffer::Usage usage);

    /**
     * @see Graphics::destroyBuffer
     */
    void destroyBuffer(Graphics::Buffer* buffer);

    /**
     * @see Graphics::createTexture1D
     */
    Graphics::Texture* createTexture1D(Graphics::Format format, size_t width, Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::createTexture2D
     */
    Graphics::Texture* createTexture2D(Graphics::Format format, size_t width, size_t height, size_t mipLevelCount, Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::createTexture3D
     */
    Graphics::Texture* createTexture3D(Graphics::Format format,  size_t width, size_t height, size_t depth, Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::destroyTexture
     */
    void destroyTexture(Graphics::Texture* texture);

    /**
     * @see Graphics::createSampler
     */
    Graphics::Sampler* createSampler();

    /**
     * @see Graphics::destroySampler
     */
    void destroySampler(Graphics::Sampler* sampler);

    /**
     * @see Graphics::createShaderProgram
     */
    Graphics::ShaderProgram* createShaderProgram(size_t vertSize, const void* vertByteCode, const char* vertEntryPoint,
                                                         size_t tescSize, const void* tescByteCode, const char* tescEntryPoint,
                                                         size_t teseSize, const void* teseByteCode, const char* teseEntryPoint,
                                                         size_t geomSize, const void* geomByteCode, const char* geomEntryPoint,
                                                         size_t fragSize, const void* fragByteCode, const char* fragEntryPoint);
    /**
     * @see Graphics::destroyShaderProgram
     */
    void destroyShaderProgram(Graphics::ShaderProgram* shaderProgram);

    /**
     * @see Graphics::createRenderPass
     */
    Graphics::RenderPass* createRenderPass(Graphics::Format colorFormat,
                                                   size_t colorAttachmentCount,
                                                   Graphics::Format depthStencilFormat,
                                                   size_t width, size_t height,
                                                   bool hostVisible);
    /**
     * @see Graphics::destroyRenderPass
     */
    void destroyRenderPass(Graphics::RenderPass* renderPass);

    /**
     * @see Graphics::createPipeline
     */
    Graphics::Pipeline* createPipeline(Graphics::ShaderProgram* shaderProgram,
                                       Graphics::VertexLayout* vertexLayout,
                                       Graphics::DescriptorSet* descriptorSet,
                                       Graphics::RenderPass* renderPass,
                                       Graphics::PrimitiveTopology primitiveTopology,
                                       Graphics::RasterizerState rasterizerState,
                                       Graphics::DepthStencilState depthStencilState,
                                       Graphics::BlendState blendState);
    /**
     * @see Graphics::destroyPipeline
     */
    void destroyPipeline(Graphics::Pipeline* pipeline);

    /**
     * @see Graphics::createCommandPool
     */
    Graphics::CommandPool* createCommandPool();

    /**
     * @see Graphics::destroyCommandPool
     */
    void destroyCommandPool(Graphics::CommandPool* pool);

    /**
     * @see Graphics::submit
     */
    void submit(CommandList** commandLists, size_t commandListCount);

    /**
     * @see Graphics::waitIdle
     */
    void waitIdle();

    /**
     * @see Graphics::present
     */
    bool present();

    /**
     * @see Graphics::acquireNextSwapchainImage
     */
    void acquireNextSwapchainImage();

    /**
     * @see Graphics::render
     */
    void render(float elapsedTime);

private:

	void createInstance();
	void createDevice();
	void createSurface(unsigned long window, unsigned long connection);
	void createSwapchain();
	void createDepthStencil();
	void createRenderPass();
	void createFrameBuffers();
	void createCommandBuffers();
	void createSynchronizationPrimitives();
	void createPipelineCache();
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
