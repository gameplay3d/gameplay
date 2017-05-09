#ifdef _WINDOWS

#pragma once

#include "Graphics.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Direct3D 12.x impl of Graphics
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
     * @see Graphics::initialize
	 */
    void initialize(unsigned long window, unsigned long connection = 0);

	/**
     * @see Graphics::resize
	 */
    void resize(int width, int height);

	/**
     * @see Graphics::render
	 */
    void render(float elapsedTime);

    /**
     * @see Graphics::isInitialized
     */
    bool isInitialized();

	/**
	 * @see Graphics::isPrepared
	 */
    bool isPrepared();

private:

	void getHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
	void waitForGpu();
	void createRenderTargets();
	void buildCommands();

    bool _initialized;
	bool _prepared;
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
