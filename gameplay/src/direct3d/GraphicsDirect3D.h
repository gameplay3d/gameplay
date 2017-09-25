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
     * Constructor.
     */
    GraphicsDirect3D();

	/**
	 * Destructor
	 */
    ~GraphicsDirect3D();

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
	std::shared_ptr<Buffer> createVertexBuffer(const VertexFormat& vertexFormat, size_t size, bool dynamic = false);

    /**
     * @see Graphics::createIndexBuffer
     */
	std::shared_ptr<Buffer> createIndexBuffer(IndexFormat indexFormat, size_t size, bool dynamic = false);

	/**
     * @see Graphics::createCommandList
     */
	std::shared_ptr<CommandList> createCommandList();

	/**
     * @see Graphics::submitCommandLists
     */
	void submitCommandLists(std::shared_ptr<CommandList>* commandLists, size_t count);

    /**
     * @see Graphics::beginScene
     */
	bool beginScene();

   /**
    * @see Graphics::endScene
    */
    void endScene();

    /**
     * @see Graphics::present
     */
    void present();

    /**
     * @see Graphics::flushAndWait
     */
    void flushAndWait();

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
