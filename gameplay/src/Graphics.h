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


namespace gameplay
{

/**
 * Defines an abstract graphics system.
 *
 * Provides direct gpu for rendering, compute and copy operations.
 */
class Graphics
{   
public:

    /**
     * Defines the low-level graphics api.
     */
    enum Api
    {
        API_VK,
        API_D3D12,
        API_MTL
    };
	
    /**
     * Gets the graphics system.
     *
     * @return The graphics system.
     */
    static Graphics* getGraphics();

    /**
     * Gets the graphics api.
     *
     * @return The graphics api.
     */
    Graphics::Api getApi();

    /**
     * Gets the width of the graphics sytem swap images.
     *
     * @return The width of the graphics sytem swap images.
     */
    virtual int getWidth() = 0;

    /**
     * Gets the height of the graphics sytem swap images.
     *
     * @return The height of the graphics sytem swap images.
     */
    virtual int getHeight() = 0;

	/**
	 * Gets the currently bound render pass.
	 *
	 * @return The currently bound render pass.
	 */
	virtual std::shared_ptr<RenderPass> getRenderPass() = 0;

	/**
	 * Acquires the next swapchain image and returns the associated render pass.
	 *
	 * @return The currently bound render pass.
	 */
	virtual std::shared_ptr<RenderPass> acquireNextSwapchainImage() = 0;

	/**
	 * Presents the next presenation image from the swapchain.
	 */
	virtual void present() = 0;

	/**
	 * Waits on the host for the completion of outstanding gpu operations.
	 *
	 * @param waitFence The fence to wait on.
	 */
	virtual void waitIdle() = 0;

	/**
	 * Records a command to start gpus command processing.
	 *
	 * @return The command buffer that is now initialized to being recording commands.
	 */
	virtual std::shared_ptr<CommandBuffer> beginCommands() = 0;

	/**
	 * Records a command to end gpu command processing.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 */
	virtual void endCommands() = 0;

	/**
	 * Submits a sequence of commands in a command buffer to the gpu.
	 *
	 * @param commandBuffer The command buffer to be submitted.
	 */
	virtual void submit(std::shared_ptr<CommandBuffer> commandBuffer) = 0;

	/**
	 * Records a command that begins the render pass for rendering.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param renderPass The render pass to begin rendering into
	 */
	virtual void cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
								    std::shared_ptr<RenderPass> renderPass) = 0;

	/**
	 * Records a command that ends a render pass.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param renderPipeline The render pipeline to be bound for rendering.
	 */
	virtual void cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer) = 0;

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
	virtual void cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
								float x, float y, float width, float height, 
								float depthMin, float depthMax) = 0;
	/**
	 * Records a command that sets the dynamic scissor test region.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param x The scissor x coordinate.
	 * @param y The scissor y coordinate.
	 * @param width The scissor width.
	 * @param height The scissor height.
	 */
	virtual void cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
							   size_t x, size_t y, size_t width, size_t height) = 0;

	/**
	 * Records a command that binds the render pipeline for rendering.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param renderPipeline The render pipeline to be bound for rendering.
	 */
	virtual void cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::shared_ptr<RenderPipeline> renderPipeline) = 0;
	/**
	 * Records a command that binds the resource descriptor to a render pipline.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param renderPipeline The render pipeline to bind the descriptors to access.
	 * @param descriptorSet The descriptor set of resources to be bound.
	 */
	virtual void cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::shared_ptr<RenderPipeline> renderPipeline, 
									  std::shared_ptr<DescriptorSet> descriptorSet) = 0;
	/**
	 * Records a command that binds a vertex buffer.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param vertexBuffer The vertex buffer to be bound.
	 */
	virtual void cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::shared_ptr<Buffer> vertexBuffer) = 0;
	/**
	 * Records a command that one or more vertex buffers.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param vertexBuffers The vertex buffers to be bound.
	 */
	virtual void cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
									  std::vector<std::shared_ptr<Buffer>> vertexBuffers) = 0;

	/**
	 * Records a command that binds an index buffer.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param indexBuffer The index buffer to be bound.
	 */
	virtual void cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									std::shared_ptr<Buffer> indexBuffer) = 0;

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
	virtual void cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
							   float red, float green, float blue, float alpha, 
							   size_t attachmentIndex) = 0;

	/**
	 * Records a command that clears the depth/stencil attachment.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param attachmentIndex the index of the color attachment to be cleared.
	 * @param clearValue The value to clear the color attachment.
	 */
	virtual void cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
									  float depth, uint32_t stencil) = 0;

	/**
	 * Records a command that draws.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param vertexCount The number of vertices to be drawn.
	 * @param vertexStart The starting vertex to be drawn.
	 */
	virtual void cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
						 size_t vertexCount, 
						 size_t vertexStart) = 0;

	/**
	 * Records a command that draws indexed.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param indexCount The number of indices to be drawn.
	 * @param indexStart The starting index to be drawn.
	 */
	virtual void cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
								size_t indexCount, 
								size_t indexStart) = 0;
	/**
	 * Records a command that transitions an image/texture from one usage to another.
	 *
	 * @param commandBuffer The command buffer to be recorded into.
	 * @param texture The texture to be transitioned.
	 * @param usageOld The old/previous usage before the transition.
	 * @param usageNew The new usage to be transitioned to.
	 */
	virtual void cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
									std::shared_ptr<Texture> texture, 
									Texture::Usage usageOld, 
									Texture::Usage usageNew) = 0;

	/**
	 * Creates a vertex buffer.
	 *
	 * @param size The size of the vertex buffer (in bytes).
	 * @param vertexStride The vertex stride.
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @param data The vertex data to initialize the buffer with.
	 * @return The created vertex buffer.
	 */
	virtual std::shared_ptr<Buffer> createVertexBuffer(size_t size, 
													   size_t vertexStride, 
													   bool hostVisible,
													   const void* data) = 0;	

	/**
	 * Creates a index buffer.
	 *
	 * @param size The size of the index buffer (in bytes).
	 * @param indexStride The stride of the index data. sizeof(uint32_t) or sizeof(uint16_t)
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @param data The index data to initialize thes buffer with.
	 * @return The created index buffer.
	 */
	virtual std::shared_ptr<Buffer> createIndexBuffer(size_t size, 
													  size_t indexStride,
													  bool hostVisible,
												      const void* data) = 0;

	/**
	 * Creates a uniform buffer.
	 *
	 * @param size The size of the uniform buffer (in bytes).
	 * @param hostVisible true if this buffer memory can be access from the client, false if not.
	 * @param data The uniform data to initialize the buffer with.
	 * @return The created uniform buffer.
	 */
	virtual std::shared_ptr<Buffer> createUniformBuffer(size_t size, 
														bool hostVisible,
													    const void* data) = 0;

	/**
	 * Destroys a buffer.
	 *
	 * @param buffer The buffer to be destroyed.
	 */
	virtual void destroyBuffer(std::shared_ptr<Buffer> buffer) = 0;

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
	virtual std::shared_ptr<Texture> createTexture1d(size_t width, 
													 Format pixelFormat, 
													 Texture::Usage usage, 
													 Texture::SampleCount sampleCount,
													 bool hostVisible,
													 const void* data) = 0;
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
	virtual std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels, 
													 Format pixelFormat, 
													 Texture::Usage usage, 
													 Texture::SampleCount sampleCount,
													 bool hostVisible,
													 const void* data) = 0;
	/**
	 * Creates a 2-dimensional texture.
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
	virtual std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth, 
													 Format pixelFormat, 
													 Texture::Usage usage, 
													 Texture::SampleCount sampleCount,
													 bool hostVisible,
												     const void* data) = 0;
	/**
	 * Destroys a texture.
	 *
	 * @param texture The texture to be destroyed.
	 */
	virtual void destroyTexture(std::shared_ptr<Texture> texture) = 0;

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
	virtual std::shared_ptr<RenderPass> createRenderPass(size_t width, size_t height, 
														 size_t colorAttachmentCount,
														 Format colorFormat,
														 Format depthStencilFormat,
														 Texture::SampleCount sampleCount) = 0;
	/**
	 * Destroys a render pass.
	 * 
	 * @param renderPass The RenderPass to be destroyed.
	 */
	virtual void destroyRenderPass(std::shared_ptr<RenderPass> renderPass) = 0;

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
	virtual std::shared_ptr<Sampler> createSampler(Sampler::Filter filterMin,
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
												   float lodMipBias) = 0;
	/**
	 * Destroys a sampler.
	 *
	 * @param sampler The sampler to be destroyed.
	 */
	virtual void destroySampler(std::shared_ptr<Sampler> sampler) = 0;

	/**
	 * Creates a shader from graphics api specific 
	 * byte-code/libraries.
	 *
	 * Example:
	 * auto vertShader = createShader("nuklear.vert");
	 *
	 * Loads:
	 * vk    -> "res/shaders/glsl/nuklear.vert.spv"
	 * d3d12 -> "res/shaders/hlsl/nuklear.vert.cso"
	 * mtl   -> "res/shaders/msl/nuklear.vert.metallib"
	 *
	 * @param url The url of the shader to load.
	 * @return The shader that is created.
	 */
	virtual std::shared_ptr<Shader> createShader(const std::string& url) = 0;

	/**
	 * Destroys the shader.
	 *
	 * @param shader The shader to be destroyed.
	 */
	virtual void destroyShader(std::shared_ptr<Shader> shader) = 0;

	/**
	 * Creates a descriptor set.
	 *
	 * @param descriptors The array of descriptors.
	 * @param descriptorCount The number of descriptors being created in the set.
	 * @return The created descriptor set.
	 */
	virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
															   size_t descriptorCount) = 0;

	/**
	 * Destroys a descriptor set.
	 *
	 * @param descriptorSet The descriptor set to be destroyed.
	 */
	virtual void destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet) = 0;

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
	virtual std::shared_ptr<RenderPipeline> createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
																 std::shared_ptr<Shader> fragShader) = 0;
	/**
	 * Destroys a render pipeline.
	 *
	 * @param renderPipeline The render pipeline to be destroyed.
	 */
	virtual void destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline) = 0;

    /**
     * Event occurs when the platform requests to initialize graphics.
	 *
	 * Exits application if fails.
	 *
	 * @param window The native window object/handle.
	 * @param connection The native connection to windowing system and application.
	 */
    virtual void onInitialize(unsigned long window, unsigned long connection) = 0;

    /**
     * Determines if the graphics system has completed initialization.
     *
     * @return true if the graphics system is initialized, false if not.
     */
    virtual bool isInitialized() = 0;

	/**
     * Event occurs when the platform window resizes.
	 *
	 * @param width The width to resize to.
	 * @param height The width to resize to.
	 */
    virtual void onResize(int width, int height) = 0;

    /**
     * Determins if the graphics system have completed resizing.
     *
     * @return true if if the graphics system have completed resizing, false if not completed.
     */
    virtual bool isResized() = 0;

protected:

	static size_t computeMipLevels(size_t width, size_t height);

    static Graphics* _graphics;
    Graphics::Api _api;
};

}
