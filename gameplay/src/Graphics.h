#pragma once

#include "Format.h"
#include "Buffer.h"
#include "Texture.h"
#include "RenderPass.h"
#include "Sampler.h"
#include "Shader.h"
#include "VertexLayout.h"
#include "DescriptorSet.h"
#include "RenderPipeline.h"
#include "CommandBuffer.h"
#include "Semaphore.h"

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines direct hardware access to gpu functionality
 * for graphics, compute and transfer.
 */
class Graphics
{
    friend class Game;

public:

    /**
     * Constructor.
     */
    Graphics();

    /**
     * Destructor.
     */
    ~Graphics();

    /**
     * Gets the width of the graphics sytem swap images.
     *
     * @return The width of the graphics sytem swap images.
     */
    uint32_t getWidth();

    /**
     * Gets the height of the graphics sytem swap images.
     *
     * @return The height of the graphics sytem swap images.
     */
    uint32_t getHeight();

    /**
     * Gets the currently bound render pass.
     *
     * @return The currently bound render pass.
     */
    std::shared_ptr<RenderPass> getRenderPass();

    /**
     * Acquires the next image for workload submission.
     */
    std::shared_ptr<RenderPass> acquireNextFrame();

    /**
     * Present the image along with any unsubmitted work.
     */
    void presentFrame(std::shared_ptr<Semaphore> waitSemaphore = nullptr);

    /**
     * Records a command to start gpus command processing.
     *
     * @return The command buffer that is now initialized to being recording commands.
     */
    std::shared_ptr<CommandBuffer> beginCommands();

    /**
     * Records a command to end gpu command processing.
     *
     * @param commandBuffer The command buffer to be recorded into.
     */
    void endCommands();

    /**
     * Creates a new semaphore that can be used to insert a 
     * dependency between batches submitted to queues. 
     *
     * @return The created semaphore.
     */
    std::shared_ptr<Semaphore> createSemaphore();

    /**
     * Destroys a semaphore.
     *
     * @param semaphore The semaphore to be destroyed.
     */
    void destroySemaphore(std::shared_ptr<Semaphore> semaphore);

    /** 
     * Gets the semaphore for present completed.
     *
     * @return The semaphore for present completed.
     */
    std::shared_ptr<Semaphore> getSemaphorePresentComplete();

    /** 
     * Gets the semaphore for render complete.
     *
     * @return The semaphore for render complete.
     */
    std::shared_ptr<Semaphore> getSemaphoreRenderComplete();

    /**
     * Submits a sequence of commands in a command buffer to the gpu.
     *
     * @param commandBuffer The command buffer to be submitted.
     * @param waitSemaphore The semaphore to wait on before processing.
     * @param signalSemaphore The semaphore to signal when  precessing is complete.
     */
    void submit(std::shared_ptr<CommandBuffer> commandBuffer, 
                std::shared_ptr<Semaphore> waitSemaphore,
                std::shared_ptr<Semaphore> signalSemaphore);

    /**
     * Records a command that begins the render pass for rendering.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param renderPass The render pass to begin rendering into
     */
    void cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<RenderPass> renderPass);

    /**
     * Records a command that ends a render pass.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param renderPipeline The render pipeline to be bound for rendering.
     */
    void cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer);

    /**
     * Records a command that sets the viewport to render within.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param x The viewport x coordinate.
     * @param y The viewport y coordinate.
     * @param width The viewport width.
     * @param height The viewport height.
     * @param depthMin The minimum depth range for the viewport.
     * @param depthMax The maximum depth range for the viewport.
     */
    void cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
                        float x, float y, float width, float height, 
                        float depthMin, float depthMax);
    /**
     * Records a command that sets the dynamic scissor test region.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param x The scissor x coordinate.
     * @param y The scissor y coordinate.
     * @param width The scissor width.
     * @param height The scissor height.
     */
    void cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
                       size_t x, size_t y, size_t width, size_t height);

    /**
     * Records a command that binds the render pipeline for rendering.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param renderPipeline The render pipeline to be bound for rendering.
     */
    void cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
                               std::shared_ptr<RenderPipeline> renderPipeline);
    /**
     * Records a command that binds the resource descriptor to a render pipline.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param renderPipeline The render pipeline to bind the descriptors to access.
     * @param descriptorSet The descriptor set of resources to be bound.
     */
    void cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                              std::shared_ptr<RenderPipeline> renderPipeline, 
                              std::shared_ptr<DescriptorSet> descriptorSet);
    /**
     * Records a command that binds a vertex buffer.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param vertexBuffer The vertex buffer to be bound.
     */
    void cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                             std::shared_ptr<Buffer> vertexBuffer);
    /**
     * Records a command that one or more vertex buffers.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param vertexBuffers The vertex buffers to be bound.
     */
    void cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
                              std::vector<std::shared_ptr<Buffer>> vertexBuffers);

    /**
     * Records a command that binds an index buffer.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param indexBuffer The index buffer to be bound.
     */
    void cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<Buffer> indexBuffer);

    /**
     * Records a command that clears the specified color attachment.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param red The red component value to clear the color attachment to.
     * @param green The green component value to clear the color attachment to.
     * @param blue The blue component value to clear the color attachment to.
     * @param alpha The alpha component value to clear the color attachment to.
     * @param attachmentIndex the index of the color attachment to be cleared.
     */
    void cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
                       float red, float green, float blue, float alpha, 
                       size_t attachmentIndex);

    /**
     * Records a command that clears the depth/stencil attachment.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param attachmentIndex the index of the color attachment to be cleared.
     * @param clearValue The value to clear the color attachment.
     */
    void cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
                              float depth, uint32_t stencil);

    /**
     * Records a command that draws.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param vertexCount The number of vertices to be drawn.
     * @param vertexStart The starting vertex to be drawn.
     */
    void cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
                size_t vertexCount, 
                size_t vertexStart);

    /**
     * Records a command that draws indexed.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param indexCount The number of indices to be drawn.
     * @param indexStart The starting index to be drawn.
     */
    void cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                        size_t indexCount, 
                        size_t indexStart);
    /**
     * Records a command that transitions an image/texture from one usage to another.
     *
     * @param commandBuffer The command buffer to be recorded into.
     * @param texture The texture to be transitioned.
     * @param usageOld The old/previous usage before the transition.
     * @param usageNew The new usage to be transitioned to.
     */
    void cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<Texture> texture, 
                            Texture::Usage usageOld, 
                            Texture::Usage usageNew);

    /**
     * Creates a vertex buffer.
     *
     * @param size The size of the vertex buffer (in bytes).
     * @param vertexStride The vertex stride.
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The vertex data to initialize the buffer with.
     * @return The created vertex buffer.
     */
    std::shared_ptr<Buffer> createVertexBuffer(size_t size, 
                                               size_t vertexStride, 
                                               bool hostVisible,
                                               const void* data);   

    /**
     * Creates a index buffer.
     *
     * @param size The size of the index buffer (in bytes).
     * @param indexStride The stride of the index data. sizeof(uint32_t) or sizeof(uint16_t)
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The index data to initialize thes buffer with.
     * @return The created index buffer.
     */
    std::shared_ptr<Buffer> createIndexBuffer(size_t size, 
                                              size_t indexStride,
                                              bool hostVisible,
                                              const void* data);

    /**
     * Creates a uniform buffer.
     *
     * @param size The size of the uniform buffer (in bytes).
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The uniform data to initialize the buffer with.
     * @return The created uniform buffer.
     */
    std::shared_ptr<Buffer> createUniformBuffer(size_t size, 
                                                bool hostVisible,
                                                const void* data);

    /**
     * Destroys a buffer.
     *
     * @param buffer The buffer to be destroyed.
     */
    void destroyBuffer(std::shared_ptr<Buffer> buffer);

    /**
     * Creates a 1-dimensional texture.
     *
     * @param width The width of the texture.    
     * @param usage The supported usage for the texture.
     * @param pixelFormat The pixel format of the texture.
     * @param sampleCount The supported sample counts for texture and used for storage operations.
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The texture data to initialize the texture with.
     * @return The created texture.
     */
    std::shared_ptr<Texture> createTexture1d(size_t width, 
                                             Format pixelFormat, 
                                             Texture::Usage usage, 
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data);
    /**
     * Creates a 2-dimensional texture.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param mipLevels The number of mip levels stored in the texture.
     * @param usage The supported usage for the texture.
     * @param pixelFormat The pixel format of the texture.
     * @param sampleCount The supported sample counts for texture and used for storage operations.
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The texture data to initialize the texture with.
     * @return The created texture.
     */
    std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels, 
                                             Format pixelFormat, 
                                             Texture::Usage usage, 
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data);
    /**
     * Creates a 3-dimensional texture.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param depth The depth of the texture.
     * @param usage The supported usage for the texture.
     * @param pixelFormat The pixel format of the texture.
     * @param sampleCount The supported sample counts for texture and used for storage operations.
     * @param hostVisible true if this buffer memory can be access from the client, false if not.
     * @param data The texture data to initialize the texture with.
     * @return The created texture.
     */
    std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth, 
                                             Format pixelFormat, 
                                             Texture::Usage usage, 
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data);
    /**
     * Destroys a texture.
     *
     * @param texture The texture to be destroyed.
     */
    void destroyTexture(std::shared_ptr<Texture> texture);

    /**
     * Create a render pass.
     *
     * @param width The width of the attachements.
     * @param height The height of the attachments.
     * @param colorAttachmentCount The number of color attachments.
     * @param colorFormat The format of the color attachment(s).
     * @param depthStencilFormat The format of the depth/stencil attachment.
     * @param sampleCount The number of samples used when sampling the render pass.
     * @return The created RenderPass.
     */
    std::shared_ptr<RenderPass> createRenderPass(size_t width, size_t height, 
                                                 size_t colorAttachmentCount,
                                                 std::vector<Format> colorFormats,
                                                 Format depthStencilFormat,
                                                 Texture::SampleCount sampleCount);
    /**
     * Destroys a render pass.
     * 
     * @param renderPass The RenderPass to be destroyed.
     */
    void destroyRenderPass(std::shared_ptr<RenderPass> renderPass);

    /**
     * Creates a sampler.
     *
     * @param filterMin The value specifying the minification filter to apply to lookups.
     * @param filterMag The value specifying the magnification filter to apply to lookups.
     * @param filterMip The value specifying the mipmap filter to apply to lookups.
     * @param addressModeU The value specifying the addressing mode for outside [0..1] range for U coordinate.
     * @param addressModeV The value specifying the addressing mode for outside [0..1] range for V coordinate.
     * @param addressModeW The value specifying the addressing mode for outside [0..1] range for W coordinate.
     * @param borderColor The value specifying the border color used for texture lookup.
     * @param compareEnabled Determines if the comparison function to apply to fetched data before filtering. 
     * @param compareFunc The value specifying the comparison function to apply to fetched data before filtering.
     * @param anisotropyEnabled Determines if anisotropy if applied.
     * @param anisotropyMax The anisotropy value clamp.
     * @param lodMin The minimum value used to clamp the computed level-of-detail value.
     * @param lodMax The maximum value used to clamp the computed level-of-detail value.
     * @param lodMipBias The bias to be added to mipmap LOD calculation and bias provided by image sampling functions.
     */
    std::shared_ptr<Sampler> createSampler(Sampler::Filter filterMin,
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
                                           float lodMipBias);
    /**
     * Destroys a sampler.
     *
     * @param sampler The sampler to be destroyed.
     */
    void destroySampler(std::shared_ptr<Sampler> sampler);

    /**
     * Creates a shader from graphics api specific 
     * byte-code/libraries.
     *
     * Example:
     * auto vertShader = createShader("nuklear.vert");
     *
     * Loads:
     * vk    -> "res/shaders/glsl/nuklear.vert.spv"
     *
     * @param url The url of the shader to load.
     * @return The shader that is created.
     */
    std::shared_ptr<Shader> createShader(const std::string& url);

    /**
     * Destroys the shader.
     *
     * @param shader The shader to be destroyed.
     */
    void destroyShader(std::shared_ptr<Shader> shader);

    /**
     * Creates a descriptor set.
     *
     * @param descriptors The array of descriptors.
     * @param descriptorCount The number of descriptors being created in the set.
     * @return The created descriptor set.
     */
    std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
                                                       size_t descriptorCount);

    /**
     * Destroys a descriptor set.
     *
     * @param descriptorSet The descriptor set to be destroyed.
     */
    void destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet);

    /**
     * Creates a render pipeline.
     *
     * @param primitiveTopology The primitive topology for how vertices are connected.
     * @param vertexLayout The layout of vertex attributes being input into the pipeline.
     * @param rasterizerState The state of how a primitive is converted (rasterized) to a two-dimensional image.
     * @param colorBlendState The blending state across all color attachments.
     * @param depthStencilState The depth stencil state.
     * @param renderPass The render pass to used by the pipeline.
     * @param descriptorSet The resource descriptor set to be used by the pipeline.
     * @param vertShader The vertex shader.
     * @param tescShader The tessellation control shader.
     * @param teseShader The tessellation evaluation shader.
     * @param geomShader The geometry shader.
     * @param fragShader The fragment shader.
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
     * Destroys a render pipeline.
     *
     * @param renderPipeline The render pipeline to be destroyed.
     */
    void destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline);

private:

    void initialize();
    void resize(size_t width, size_t height);
    void render(float elapsedTime);
    void createInstance();
    void createDevice();
    void createSurface();
    void createSwapchain();
    void createRenderPasses();
    void destroyRenderPasses();
    void createCommandBuffers();
    void destroyCommandBuffers();
    void createSynchronizationObjects();
    std::shared_ptr<Buffer> createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible, const void* data);
    std::shared_ptr<Texture> createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
                                           Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount, 
                                           bool hostVisible, const void* data, VkImage existingImage);
    std::shared_ptr<RenderPass> createRenderPass(size_t width,  size_t height, 
                                                 size_t colorAttachmentCount,
                                                 std::vector<VkFormat> colorFormats, 
                                                 VkFormat depthStencilFormat,
                                                 VkSampleCountFlagBits sampleCount,
                                                 std::vector<std::shared_ptr<Texture> > colorAttachments,
                                                 std::vector<std::shared_ptr<Texture> > colorMultisampleAttachments,
                                                 std::shared_ptr<Texture> depthStencilAttachment);
    struct SwapchainInfo
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapchainInfo querySwapchainInfo(VkPhysicalDevice physicalDevice);
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkBool32 isDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const char* extensionName);
    VkBool32 getDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat* depthStencilFormat);
    uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags);
    VkBool32 getMemoryType(uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);
    VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags);
    VkCommandBuffer getCommandBuffer(bool begin);
    void flushCommandBuffer(VkCommandBuffer commandBuffer);

    bool _initialized;
    bool _resized;
    uint32_t _width;
    uint32_t _height;
    bool _fullscreen;
    bool _vsync;
    bool _multisampling;
    bool _validation;
    VkInstance _instance;
    VkPhysicalDevice _physicalDevice;
    VkPhysicalDeviceProperties _deviceProperties;
    VkPhysicalDeviceMemoryProperties _deviceMemoryProperties;
    VkDevice _device;
    VkDebugReportCallbackEXT _debugMessageCallback; 
    std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
    struct
    {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    } _queueFamilyIndices;
    VkQueue _queue;
    uint32_t _queuePresentIndex;
    VkSurfaceKHR _surface;
    VkSwapchainKHR _swapchain;
    uint32_t _swapchainImageCount;
    uint32_t _swapchainImageIndex;
    std::vector<VkImage> _swapchainImages;
    VkColorSpaceKHR _colorSpace;
    VkFormat _colorFormat;
    VkFormat _depthStencilFormat;
    std::vector<std::shared_ptr<RenderPass> > _renderPasses;
    std::shared_ptr<RenderPass> _renderPass;
    VkCommandPool _commandPool;
    std::vector<std::shared_ptr<CommandBuffer> > _commandBuffers;
    std::shared_ptr<CommandBuffer> _commandBuffer;
    std::shared_ptr<Semaphore> _presentCompleteSemaphore;
    std::shared_ptr<Semaphore> _renderCompleteSemaphore;
    std::vector<VkFence> _waitFences;
};

}
