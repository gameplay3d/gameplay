#ifdef _WINDOWS

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
class GraphicsDirect3D : public Graphics
{
public:
    /**
     * Direct3D 12 impl of Graphics::CommandList
     *
     * @see Graphics::CommandList
     */
    class CommandListDirect3D : public CommandList
    {
    public:
        /**
         * Constructor.
         */
        CommandListDirect3D();

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
     * Direct3D 12 impl of Graphics::CommandPool
     *
     * @see Graphics::CommandPool
     */
    class CommandPoolDirect3D : public CommandPool
    {
    public:
        /**
         * Constructor.
         */
        CommandPoolDirect3D();

        /**
         * @see CommandPool::createCommandList
         */
        Graphics::CommandList* createCommandList();

        /**
         * @see CommandPool::destroyCommandList
         */
        void destroyCommandList(Graphics::CommandList* commandList);
    };

    /**
     * Constructor.
     */
    GraphicsDirect3D();

	/**
	 * Destructor
	 */
    ~GraphicsDirect3D();

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

	void getHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
    void createBackBuffers();
	void buildCommands();

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
	ID3D12CommandQueue* _commandQueue;
	IDXGISwapChain4* _swapchain;
	uint32_t _backBufferIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE _renderTargetViewHandle;
	ID3D12DescriptorHeap* _renderTargetViewHeap;
	ID3D12Resource* _renderTargets[GP_GRAPHICS_BACK_BUFFERS];
	ID3D12CommandAllocator* _commandAllocators[GP_GRAPHICS_BACK_BUFFERS];
	ID3D12GraphicsCommandList* _commandList;
	ID3D12Fence* _fence;
	HANDLE _fenceEvent;
	uint64_t _fenceValues[GP_GRAPHICS_BACK_BUFFERS];
};

}

#endif
