#pragma once

#include "Graphics.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of Graphics
 * 
 * @see Graphics
 */
class GraphicsD3D12 : public Graphics
{
public:

    /**
     * Constructor.
     */
    GraphicsD3D12();

	/**
	 * Destructor
	 */
    ~GraphicsD3D12();

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
	 * @see Graphics::acquireNextSwapchainImage
	 */
	std::shared_ptr<RenderPass> acquireNextSwapchainImage(std::shared_ptr<Fence> waitFence,
														  std::shared_ptr<Semaphore> signalSemaphore);
	/**
	 * @see Graphics::present
	 */
	void present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores);

	/**
	 * @see Graphics::waitIdle
	 */
	void waitIdle(std::shared_ptr<Fence> waitFence);

	/**
	 * @see Graphics::createCommandPool
	 */
	std::shared_ptr<CommandPool> createCommandPool();

	/**
	 * @see Graphics::destroyCommandPool
	 */
	void destroyCommandPool(std::shared_ptr<CommandPool> commandPool);

	/**
	 * @see Graphics::submit
	 */
	void submit(std::vector<std::shared_ptr<CommandBuffer>> commandBuffers,
				std::vector<std::shared_ptr<Semaphore>> waitSemaphores,
			    std::vector<std::shared_ptr<Semaphore>> signalSemaphores);
	/**
	 * @see Graphics::cmdBegin
	 */
	void cmdBegin(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdEnd
	 */
	void cmdEnd(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdBeginRender
	 */
	void cmdBeginRender(std::shared_ptr<CommandBuffer> commandBuffer,
						std::shared_ptr<RenderPass> renderPass);

	/**
	 * @see Graphics::cmdEndRender
	 */
	void cmdEndRender(std::shared_ptr<CommandBuffer> commandBuffer);

	/**
	 * @see Graphics::cmdSetViewport
	 */
	void cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
						float x, float y, float width, float height, 
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
							   std::shared_ptr<RenderPipeline> renderPipeline);
	/**
	 * @see Graphics::cmdBindDescriptorSet
	 */
	void cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
							  std::shared_ptr<RenderPipeline> renderPipeline, 
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
							Texture::Usage usageOld, 
							Texture::Usage usageNew);

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
	std::shared_ptr<Buffer> createVertexBuffer(size_t size, 
											   size_t vertexStride, 
											   bool hostVisible,
											   const void* data);
 	
	/**
	 * @see Graphics::createIndexBuffer
	 */
	std::shared_ptr<Buffer> createIndexBuffer(size_t size, 
											  IndexFormat indexFormat, 
											  bool hostVisible,
											  const void* data);

	/**
	 * @see Graphics::createUniformBuffer
	 */
	std::shared_ptr<Buffer> createUniformBuffer(size_t size, 
												bool hostVisible,
											    const void* data);

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
											const ClearValue& clearValue,
											bool hostVisible,
											const void* data);
	/**
     * @see Graphics::createTexture2d
     */
	std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels,
											 Format pixelFormat,
											 Texture::Usage usage,
											 Texture::SampleCount sampleCount,
											 const ClearValue& clearValue,
											 bool hostVisible,
											 const void* data);
	/**
     * @see Graphics::createTexture3d
     */
	std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth, 
											 Format pixelFormat,
											 Texture::Usage usage, 
											 Texture::SampleCount sampleCount,
											 const ClearValue& clearValue,
											 bool hostVisible,
											 const void* data);
	/**
     * @see Graphics::destroyTexture
     */
	void destroyTexture(std::shared_ptr<Texture> texture);

	/**
     * @see Graphics::createRenderPass
     */
	std::shared_ptr<RenderPass> createRenderPass(size_t width, size_t height, 
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
													   size_t descriptorCount);
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
	void destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline);

private:

	void getHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
    void createSwapchainImages();
	std::shared_ptr<Buffer> createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible, bool is32bit);
	std::shared_ptr<Texture> createTexture(Texture::Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
										   Format pixelFormat, Texture::Usage usage, Texture::SampleCount sampleCount, const ClearValue& clearValue, bool hostVisible, const void* data, ID3D12Resource* resource);
	std::shared_ptr<RenderPass> createRenderPass(size_t width, size_t height, 
												 size_t colorAttachmentCount,
												 Format colorFormat,
												 Format depthStencilFormat,
												 Texture::SampleCount sampleCount,
												 std::vector<std::shared_ptr<Texture>> colorAttachments,
												 std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
												 std::shared_ptr<Texture> depthStencilAttachment);
    bool _initialized;
    bool _resized;
	uint32_t _width;
	uint32_t _height;
	bool _fullscreen;
	bool _vsync;
	uint32_t _multisampling;
	bool _validation;
	HWND _hwnd;
	UINT _displayMode;
	std::vector<DXGI_MODE_DESC> _displayModes;
	ID3D12Device* _device;
	ID3D12CommandQueue* _queue;
	ID3D12Fence* _queueFence;
	HANDLE _queueFenceEvent;
	IDXGISwapChain4* _swapchain;
	uint32_t _swapchainImagesViewDescriptorSize;
	ID3D12DescriptorHeap* _swapchainImagesViewHeap;
	uint32_t _swapchainImageIndex;
	ID3D12Resource* _swapchainImages[GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT];
	std::vector<std::shared_ptr<RenderPass>> _renderPasses;
	std::shared_ptr<RenderPass> _renderPass;
};

#define D3D_CHECK_RESULT(f)		\
{								\
	HRESULT hr = (f);			\
	if (FAILED(hr))				\
	{							\
		std::cout << "Fatal: HRESULT is \"" << std::to_string(hr).c_str() << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl << std::flush; \
		GP_ASSERT(SUCCEEDED(hr));	\
	}							\
}

}
