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
	 * @see Graphics::createVertexBuffer
	 */
	std::shared_ptr<Buffer> createVertexBuffer(const VertexFormat& vertexFormat, size_t vertexCount, bool hostVisible);
 	
	/**
	 * @see Graphics::createIndexBuffer
	 */
	std::shared_ptr<Buffer> createIndexBuffer(IndexFormat indexFormat,  size_t indexCount, bool hostVisible);

	/**
	 * @see Graphics::createUniformBuffer
	 */
	std::shared_ptr<Buffer> createUniformBuffer(size_t size, bool hostVisible);

	/**
	 * @see Graphics::destroyBuffer
	 */
	void destroyBuffer(std::shared_ptr<Buffer> buffer);

	/**
     * @see Graphics::createCommandPool
     */
	std::shared_ptr<CommandPool> createCommandPool(bool transient = false);

	/**
     * @see Graphics::destroyCommandPool
     */
	void destroyCommandPool(std::shared_ptr<CommandPool> commandPool);

	/**
     * @see Graphics::createCommandLists
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

private:

	void getHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
    void createBackBuffers();
	void buildCommands();
	ID3D12Resource* createResource(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible);

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
