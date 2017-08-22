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
     * Direct3D 12 impl of Graphics::Fence
     *
     * @see Graphics::Fence
     */
    class FenceDirect3D : public Fence 
    {
    public:
        ID3D12Fence* fence;
    };

    /**
     * Direct3D 12 impl of Graphics::Semaphore
     *
     * @see Graphics::Semaphore
     */
    class SemaphoreDirect3D : public Semaphore 
    {
    public:
        void* semaphore;
    };

    /**
     * Direct3D 12 impl of Graphics::Buffer
     *
     * @see Graphics::Buffer
     */
    class BufferDirect3D : public Buffer
    {
    public:
        ID3D12Resource* resource;
        D3D12_CONSTANT_BUFFER_VIEW_DESC  cbvSDesc;
        D3D12_INDEX_BUFFER_VIEW ibv;
        D3D12_VERTEX_BUFFER_VIEW vbv;
    };

   /**
     * Direct3D 12 impl of Graphics::Texture
     *
     * @see Graphics::Texture
     */
    class TextureDirect3D : public Texture
    {
    public:
        ID3D12Resource* resource;
        D3D12_SHADER_RESOURCE_VIEW_DESC  srvDesc;
    };

    /**
     * Direct3D 12 impl of Graphics::Sampler
     *
     * @see Graphics::Sampler
     */
    class SamplerDirect3D : public Sampler
    {
    public:
        D3D12_SAMPLER_DESC samplerDesc;
    };

    /**
     * Direct3D 12 impl of Graphics::ShaderProgram
     *
     * @see Graphics::ShaderProgram
     */
    class ShaderProgramDirect3D : public ShaderProgram
    {
    public:
        ID3DBlob* vertBlob;
        ID3DBlob* tescBlob;
        ID3DBlob* teseBlob;
        ID3DBlob* geomBlob;
        ID3DBlob* fragBlob;
    };

    /**
     * Direct3D 12 impl of Graphics::Descriptor
     *
     * @see Graphics::Descriptor
     */
    class DescriptorDirect3D : public Descriptor
    {
    public:
        unsigned int heapOffset;
        unsigned int rootParameterIndex;
    };

    /**
     * Direct3D 12 impl of Graphics::DescriptorSet
     *
     * @see Graphics::DescriptorSet
     */
    class DescriptorSetDirect3D : public DescriptorSet
    {
    public:
        ID3D12DescriptorHeap* cbvsrvuavHeap;
        ID3D12DescriptorHeap* samplerHeap;
    };


    /**
     * Direct3D 12 impl of Graphics::Pipeline
     *
     * @see Graphics::Pipeline
     */
    class PipelineDirect3D : public Pipeline
    {
    public:
        ID3D12RootSignature* rootSignature;
        ID3D12PipelineState* pipelineState;
    };

    /**
     * Direct3D 12 impl of Graphics::RenderPass
     *
     * @see Graphics::RenderPass
     */
    class RenderPassDirect3D : public RenderPass
    {
    public:
        ID3D12DescriptorHeap*  rtvHeap;
        ID3D12DescriptorHeap*  dsvHeap;
    };

    /**
     * Direct3D 12 impl of Graphics::CommandList
     *
     * @see Graphics::CommandList
     */
    class CommandListDirect3D : public CommandList
    {
    public:
        ID3D12Device* device;
        ID3D12GraphicsCommandList* commandList;

        /**
         * Constructor.
         */
        CommandListDirect3D(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

        /**
         * @see CommandList::begin
         */
        void begin();

        /**
         * @see CommandList::end
         */
        void end();

        /**
         * @see CommandList::transitionRenderPass
         */
        void transitionRenderPass(Graphics::RenderPass* pass, 
                                  Graphics::Texture::Usage textureUsageOld, 
                                  Graphics::Texture::Usage textureUsageNew);

        /**
         * @see CommandList::beginRender
         */
        void beginRender(Graphics::RenderPass* pass);

        /**
         * @see CommandList::endRender
         */
        void endRender(Graphics::RenderPass* pass);

        /**
         * @see CommandList::clearColor
         */
        void clearColor(Graphics::ClearValue clearValue, size_t colorAttachmentIndex);

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
        void bindPipeline(Graphics::Pipeline* pipeline);

        /**
         * @see CommandList::bindDescriptorSet
         */
        void bindDescriptorSet(Graphics::DescriptorSet* set);

        /**
         * @see CommandList::bindVertexBuffers
         */
        void bindVertexBuffers(Graphics::Buffer** buffers, size_t bufferCount);

        /**
         * @see CommandList::bindIndexBuffer
         */
        void bindIndexBuffer(Graphics::Buffer* buffer);

        /**
         * @see CommandList::draw
         */
        void draw(size_t vertexCount, size_t vertexOffset);

        /**
         * @see CommandList::drawInstanced
         */
        void drawInstanced(size_t vertexCount, size_t vertexOffset);

        /**
         * @see CommandList::drawIndexed
         */
        void drawIndexed(size_t indexCount, size_t indexOffset);

        /**
         * @see CommandList::drawIndexedInstanced
         */
        void drawIndexedInstanced(size_t indexCount, size_t indexOffset);
    };

    /**
     * Direct3D 12 impl of Graphics::CommandPool
     *
     * @see Graphics::CommandPool
     */
    class CommandPoolDirect3D : public CommandPool
    {
    public:
        ID3D12Device* device;
        ID3D12CommandAllocator* commandAllocator;

        /**
         * Constructor.
         */
        CommandPoolDirect3D(ID3D12Device* device, ID3D12CommandAllocator* commandAllocator);

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
    Graphics::DescriptorSet* createDescriptorSet(Graphics::Descriptor* descriptors, size_t descriptorCount);

    /**
     * @see Graphics::createVertexBuffer
     */
    Graphics::Buffer* createVertexBuffer(size_t count, bool hostVisible,  size_t vertexStride);

    /**
     * @see Graphics::createIndexBuffer
     */
    Graphics::Buffer* createIndexBuffer(size_t count, bool hostVisible, Graphics::IndexType indexType);

    /**
     * @see Graphics::createUniformBuffer
     */
    Graphics::Buffer* createUniformBuffer(size_t size, bool hostVisible);

    /**
     * @see Graphics::destroyBuffer
     */
    void destroyBuffer(Graphics::Buffer* buffer);

    /**
     * @see Graphics::createTexture1D
     */
    Graphics::Texture* createTexture1D(Graphics::Format format, size_t width, 
                                       Graphics::Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::createTexture2D
     */
    Graphics::Texture* createTexture2D(Graphics::Format format, size_t width, size_t height, size_t mipLevelCount, 
                                       Graphics::Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::createTexture3D
     */
    Graphics::Texture* createTexture3D(Graphics::Format format,  size_t width, size_t height, size_t depth, 
                                       Graphics::Texture::Usage usage, bool hostVisible);

    /**
     * @see Graphics::destroyTexture
     */
    void destroyTexture(Graphics::Texture* texture);

    /**
     * @see Graphics::createSampler
     */
    Graphics::Sampler* createSampler(const Graphics::SamplerState& samplerState);

    /**
     * @see Graphics::destroySampler
     */
    void destroySampler(Graphics::Sampler* sampler);

    /**
     * @see Graphics::createShaderProgram
     */
    Graphics::ShaderProgram* createShaderProgram(size_t vertSize, const char* vertByteCode, const char* vertEntryPoint,
                                                 size_t fragSize, const char* fragByteCode, const char* fragEntryPoint);

    /**
     * @see Graphics::createShaderProgram
     */
    Graphics::ShaderProgram* createShaderProgram(size_t vertSize, const char* vertByteCode, const char* vertEntryPoint,
                                                 size_t tescSize, const char* tescByteCode, const char* tescEntryPoint,
                                                 size_t teseSize, const char* teseByteCode, const char* teseEntryPoint,
                                                 size_t geomSize, const char* geomByteCode, const char* geomEntryPoint,
                                                 size_t fragSize, const char* fragByteCode, const char* fragEntryPoint);
    /**
     * @see Graphics::destroyShaderProgram
     */
    void destroyShaderProgram(Graphics::ShaderProgram* shaderProgram);

    /**
     * @see Graphics::createRenderPass
     */
    Graphics::RenderPass* createRenderPass(Graphics::Format colorFormat, size_t colorAttachmentCount,
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
    void submit(Graphics::CommandList* commandList, 
                Graphics::Semaphore* waitSemaphore, 
                Graphics::Semaphore* signalSemaphore);
    /**
     * @see Graphics::present
     */
    void present(Graphics::Semaphore* waitSemaphore);

    /**
     * @see Graphics::waitIdle
     */
    void waitIdle();

    /**
     * @see Graphics::getRenderPass
     */
    Graphics::RenderPass* getRenderPass(size_t imageIndex);

    /**
     * @see Graphics::getImageAcquiredFence
     */
    Graphics::Fence* getImageAcquiredFence(size_t imageIndex);

    /**
     * @see Graphics::getImageAcquiredSemaphore
     */
    Graphics::Semaphore* getImageAcquiredSemaphore(size_t imageIndex);

    /**
     * @see Graphics::getRenderCompleteSemaphore
     */
    Graphics::Semaphore* getRenderCompleteSemaphore(size_t imageIndex);

    /**
     * @see Graphics::acquireNextSwapchainImage
     */
    void acquireNextImage(Graphics::Semaphore* acquiredImageSemaphore, 
                          Graphics::Fence* acquiredImageFence);

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
