#include "Base.h"
#include "Game.h"
#include "GraphicsD3D12.h"
#include "BufferD3D12.h"
#include "CommandListD3D12.h"

namespace gameplay
{

#define SAFE_RELEASE(x)			\
   if(x != nullptr)				\
   {							\
      x->Release();				\
      x = nullptr;				\
   }

#define D3D_CHECK_RESULT(f)		\
{								\
	HRESULT hr = (f);			\
	if (FAILED(hr))				\
	{							\
		std::cout << "Fatal: HRESULT is \"" << std::to_string(hr).c_str() << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl << std::flush; \
		GP_ASSERT(SUCCEEDED(hr));	\
	}							\
}

GraphicsD3D12::GraphicsD3D12() :
    _initialized(false),
    _resized(false),
	_width(0),
	_height(0),
	_fullscreen(false),
	_vsync(false),
	_multisampling(0),
	_hwnd(nullptr),
	_displayMode(0),
	_device(nullptr),
	_commandQueue(nullptr),
	_swapchain(nullptr),
	_backBufferIndex(0),
	_renderTargetViewHeap(nullptr),
	_commandList(nullptr),
	_fence(nullptr)
{
}

GraphicsD3D12::~GraphicsD3D12()
{
	if(_swapchain)
		_swapchain->SetFullscreenState(false, nullptr);
	SAFE_RELEASE(_fence);
	SAFE_RELEASE(_commandList);
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
		SAFE_RELEASE(_commandAllocators[i]);
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
		SAFE_RELEASE(_renderTargets[i]);
	SAFE_RELEASE(_renderTargetViewHeap);
	SAFE_RELEASE(_swapchain);
	SAFE_RELEASE(_commandQueue);
	SAFE_RELEASE(_device);
}

void GraphicsD3D12::onInitialize(unsigned long window, unsigned long connection)
{
    if (_initialized)
        return;

    _hwnd = (HWND)window;

	// Get the game config
	std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
	_width = config->width;
	_height = config->height;
	_fullscreen = config->fullscreen;
	_vsync = config->vsync;
	_multisampling = config->multisampling;
	_validation = config->validation;

	// Graphics validation
	uint32_t dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	if (_validation)
	{
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController)))
			debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	// Create the graphics interface factory
	IDXGIFactory4* factory;
	if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, __uuidof(IDXGIFactory4), (void**)&factory)))
		GP_ERROR("Failed to create the graphics interface factory.");

	// Create the device
	IDXGIAdapter1* hardwareAdapter;
	getHardwareAdapter(factory, &hardwareAdapter);
	if (FAILED(D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&_device)))
		GP_ERROR("Failed to create a Direct3D 12 device.");

	//Create command queue
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;
	D3D_CHECK_RESULT(_device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_commandQueue));

	// Pick the first adapter and ouput
	IDXGIAdapter* adapter;
	D3D_CHECK_RESULT(factory->EnumAdapters(0, &adapter));
	IDXGIOutput* output;
	D3D_CHECK_RESULT(adapter->EnumOutputs(0, &output));

	// Get the number of display modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM
	uint32_t displayModeCount;
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, nullptr)) ||  displayModeCount < 1)
		GP_ERROR("Failed to find compatible display mode for adapter outputs(monitor).");

	// Get the display modes
	_displayModes.resize(displayModeCount);
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, &_displayModes[0])))
		GP_ERROR("Failed to get display mode list for adapter outputs(monitor).");
	
	// Find a display mode that matches our config
	for (size_t i = 0; i<displayModeCount; i++)
	{
		if (_displayModes[i].Height == _height && _displayModes[i].Width == _width)
		{
			_width = _displayModes[i].Width;
			_height = _displayModes[i].Height;
			_displayMode = i;
			break;
		}
	}
	SAFE_RELEASE(output);
	SAFE_RELEASE(adapter);
	
	// Create the swapchain
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.BufferCount = GP_GRAPHICS_BACK_BUFFERS;
	swapchainDesc.Width = _width;
	swapchainDesc.Height = _height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.SampleDesc.Count = 1 + _multisampling;
	IDXGISwapChain1* swapchain;
	D3D_CHECK_RESULT(factory->CreateSwapChainForHwnd(_commandQueue, _hwnd, &swapchainDesc, nullptr, nullptr, &swapchain));
	D3D_CHECK_RESULT(swapchain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&_swapchain));

	SAFE_RELEASE(factory);

	// Create a render target description heap for the back buffers
	_backBufferIndex = _swapchain->GetCurrentBackBufferIndex();
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	// One extra for the intermediate render target
	rtvHeapDesc.NumDescriptors = GP_GRAPHICS_BACK_BUFFERS;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_renderTargetViewHeap));
	
	// Create a command allocators
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
		D3D_CHECK_RESULT(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&_commandAllocators[i]));

	// Create the command list
	D3D_CHECK_RESULT(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocators[_backBufferIndex], nullptr, __uuidof(ID3D12CommandList), (void**)&_commandList));
	D3D_CHECK_RESULT(_commandList->Close());

    // Create the back buffer (render targets)
    createBackBuffers();

	// Create a fence and event for synchronization.
	D3D_CHECK_RESULT(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_fence));
	_fenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);

    _initialized = true;
    _resized = true;
}

bool GraphicsD3D12::isInitialized()
{
    return _initialized;
}

void GraphicsD3D12::onResize(int width, int height)
{
    if (!_resized || (width == _width && height == _height))
		return;
	
	// Wait for the gpu to finish processing cbefore resizing
    flushCommands();

    _resized = false;
	
    // Release the backbuffers
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
	{
		SAFE_RELEASE(_renderTargets[i]);
		_fenceValues[i] = _fenceValues[_backBufferIndex];
	}

	// Resize the swap chain to the desired dimensions.
	DXGI_SWAP_CHAIN_DESC desc = {};
	_swapchain->GetDesc(&desc);
	D3D_CHECK_RESULT(_swapchain->ResizeBuffers(GP_GRAPHICS_BACK_BUFFERS, width, height, desc.BufferDesc.Format, desc.Flags));

	// Reset the frame index to the current back buffer index.
	_backBufferIndex = _swapchain->GetCurrentBackBufferIndex();

    // Create teh back buffers again
    createBackBuffers();

	_width = width;
	_height = height;

    _resized = true;
}

bool GraphicsD3D12::isResized()
{
    return _resized;
}

int GraphicsD3D12::getWidth()
{
    return _width;
}

int GraphicsD3D12::getHeight()
{
    return _height;
}

ID3D12Resource* GraphicsD3D12::createResource(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible)
{
	if (usage == Buffer::USAGE_UNIFORM)
		size = GP_MATH_ROUNDUP(size, 256);
    
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Alignment = 0;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Height = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Width = size;
	
	// Adjust for padding
    UINT64 paddedSize = 0;
    _device->GetCopyableFootprints(&resourceDesc, 0, 1, 0, nullptr, nullptr, nullptr, &paddedSize);
    size = (uint64_t)paddedSize;
    resourceDesc.Width = paddedSize;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COPY_DEST;
	switch (usage) 
	{
	case Buffer::USAGE_VERTEX: 
	case Buffer::USAGE_UNIFORM: 
        resourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		break;
    case Buffer::USAGE_INDEX: 
        resourceStates = D3D12_RESOURCE_STATE_INDEX_BUFFER;
		break;
    }
	
	if (hostVisible) 
	{
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        resourceStates = D3D12_RESOURCE_STATE_GENERIC_READ;
    }

	ID3D12Resource* resource;
	if (FAILED(_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource))))
	{
		GP_ERROR("Failed to create buffer!\n");
		return nullptr;
	}
	return resource;
}

std::shared_ptr<Buffer> GraphicsD3D12::createVertexBuffer(const VertexFormat& vertexFormat, size_t vertexCount, bool hostVisible)
{
	size_t stride = vertexFormat.getStride();
	size_t size = vertexCount * stride;
	ID3D12Resource* resource = createResource(Buffer::USAGE_VERTEX, size, stride, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_VERTEX, size, stride, hostVisible, _device, resource);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(resource->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
			return nullptr;
		}
    }
	buffer->_vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	buffer->_vertexBufferView.SizeInBytes = size;
	buffer->_vertexBufferView.StrideInBytes = stride;

	return std::static_pointer_cast<Buffer>(buffer);
}

std::shared_ptr<Buffer> GraphicsD3D12::createIndexBuffer(IndexFormat indexFormat, size_t indexCount, bool hostVisible)
{
	size_t stride = (indexFormat == INDEX_FORMAT_UNSIGNED_INT) ? sizeof(unsigned int) : sizeof(unsigned short);
	size_t size = indexCount * stride;
	ID3D12Resource* resource = createResource(Buffer::USAGE_INDEX, size, stride, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_INDEX, size, stride, hostVisible, _device, resource);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(resource->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
			return nullptr;
		}
    }
	buffer->_indexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	buffer->_indexBufferView.SizeInBytes = size;
	buffer->_indexBufferView.Format = (indexFormat == INDEX_FORMAT_UNSIGNED_INT) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

	return std::static_pointer_cast<Buffer>(buffer);
}

std::shared_ptr<Buffer> GraphicsD3D12::createUniformBuffer(size_t size, bool hostVisible)
{
	ID3D12Resource* resource = createResource(Buffer::USAGE_UNIFORM, size, size, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_UNIFORM, size, size, hostVisible, _device, resource);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(resource->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
			return nullptr;
		}
    }
	buffer->_constantBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	buffer->_constantBufferView.SizeInBytes = size;

	return std::static_pointer_cast<Buffer>(buffer);
}

void GraphicsD3D12::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
	std::shared_ptr<BufferD3D12> bufferD3D = std::static_pointer_cast<BufferD3D12>(buffer);
	SAFE_RELEASE(bufferD3D->_buffer);
	bufferD3D.reset();
}

std::shared_ptr<CommandPool> GraphicsD3D12::createCommandPool(bool transient)
{
	return nullptr;
}

void GraphicsD3D12::destroyCommandPool(std::shared_ptr<CommandPool> commandPool)
{
}

void GraphicsD3D12::submitCommands(std::shared_ptr<CommandList> commands)
{
}

void GraphicsD3D12::flushCommands()
{
    // Signal and increment the fence value.
	const uint64_t fenceToWaitFor = _fenceValues[_backBufferIndex];
	if (FAILED(_commandQueue->Signal(_fence, fenceToWaitFor)))
		return;

	// Wait until the GPU is done rendering.
	if (_fence->GetCompletedValue() < _fenceValues[_backBufferIndex])
	{
		if(FAILED(_fence->SetEventOnCompletion(_fenceValues[_backBufferIndex], _fenceEvent)))
			return;

		WaitForSingleObject(_fenceEvent, INFINITE);
	}
	_fenceValues[_backBufferIndex] = fenceToWaitFor + 1;
}

void GraphicsD3D12::present()
{
    _swapchain->Present(_vsync ? 1 : 0, 0);
}

void GraphicsD3D12::getHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	IDXGIAdapter1* adapter;
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;
		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			break;
	}
}

void GraphicsD3D12::createBackBuffers()
{
	uint32_t renderTargetDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = _renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
	{
		D3D_CHECK_RESULT(_swapchain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&_renderTargets[i]));
		_device->CreateRenderTargetView(_renderTargets[i], NULL, renderTargetViewHandle);
		renderTargetViewHandle.ptr += renderTargetDescriptorSize;
	}
	_backBufferIndex = _swapchain->GetCurrentBackBufferIndex();
}

void GraphicsD3D12::buildCommands()
{
	// Reset (re-use) the memory associated command allocator.
	D3D_CHECK_RESULT(_commandAllocators[_backBufferIndex]->Reset());

	// Reset the command list, use empty pipeline state for now since there are no shaders and we are just clearing the screen.
	if (FAILED(_commandList->Reset(_commandAllocators[_backBufferIndex], nullptr)))
		return;

	// Record commands in the command list starting by setting the resource barrier.
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = _renderTargets[_backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	_commandList->ResourceBarrier(1, &barrier);

	// Get the render target view handle for the current back buffer.
	uint32_t renderTargetDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	_backBufferIndex = _swapchain->GetCurrentBackBufferIndex();
	if (_backBufferIndex == 0)
		_renderTargetViewHandle = _renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	else
		_renderTargetViewHandle.ptr += renderTargetDescriptorSize;

	// Set the back buffer as the render target.
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	_commandList->OMSetRenderTargets(1, &_renderTargetViewHandle, FALSE, NULL);
	_commandList->ClearRenderTargetView(_renderTargetViewHandle, clearColor, 0, NULL);

	// Indicate that the back buffer will now be used to present.
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	_commandList->ResourceBarrier(1, &barrier);

	D3D_CHECK_RESULT(_commandList->Close());

    flushCommands();
}

}
