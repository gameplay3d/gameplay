#include "Base.h"
#include "GraphicsD3D12.h"
#include "CommandPoolD3D12.h"
#include "CommandListD3D12.h"
#include "BufferD3D12.h"
#include "TextureD3D12.h"
#include "ShaderD3D12.h"
#include "Game.h"
#include "FileSystem.h"


namespace gameplay
{

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
	GP_SAFE_RELEASE(_fence);
	GP_SAFE_RELEASE(_commandList);
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
		GP_SAFE_RELEASE(_commandAllocators[i]);
	for (uint32_t i = 0; i < GP_GRAPHICS_BACK_BUFFERS; i++)
		GP_SAFE_RELEASE(_renderTargets[i]);
	GP_SAFE_RELEASE(_renderTargetViewHeap);
	GP_SAFE_RELEASE(_swapchain);
	GP_SAFE_RELEASE(_commandQueue);
	GP_SAFE_RELEASE(_device);
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
	GP_SAFE_RELEASE(output);
	GP_SAFE_RELEASE(adapter);
	
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

	GP_SAFE_RELEASE(factory);

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
		GP_SAFE_RELEASE(_renderTargets[i]);
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

std::shared_ptr<CommandPool> GraphicsD3D12::createCommandPool(bool transient)
{	
	ID3D12CommandAllocator* allocator = nullptr;
	if(FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
											  __uuidof(allocator), reinterpret_cast<void**>(&allocator))))
	{
		GP_ERROR("Failed to create command allocator.");
	}
	std::shared_ptr<CommandPoolD3D12> pool = std::make_shared<CommandPoolD3D12>(allocator);
	return std::static_pointer_cast<CommandPool>(pool);
}

void GraphicsD3D12::destroyCommandPool(std::shared_ptr<CommandPool> commandPool)
{
	std::shared_ptr<CommandPoolD3D12> commandPoolD3D = std::static_pointer_cast<CommandPoolD3D12>(commandPool);
	GP_SAFE_RELEASE(commandPoolD3D->_commandAllocator);
	commandPoolD3D.reset();
}

std::shared_ptr<CommandList> GraphicsD3D12::createCommandList(std::shared_ptr<CommandPool> pool, bool secondary)
{
	std::shared_ptr<CommandPoolD3D12> poolD3D = std::static_pointer_cast<CommandPoolD3D12>(pool);
	ID3D12GraphicsCommandList* commandListD3D = nullptr;
	ID3D12PipelineState* initialPipelineState = nullptr;
	if (FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, poolD3D->_commandAllocator, initialPipelineState,
			                              __uuidof(commandListD3D), reinterpret_cast<void**>(&commandListD3D))))
	{
		GP_ERROR("Failed to create graphics command list.");
	}
    commandListD3D->Close();

	std::shared_ptr<CommandListD3D12> commandList = std::make_shared<CommandListD3D12>(_device, poolD3D->_commandAllocator, commandListD3D);
	return std::static_pointer_cast<CommandList>(commandList);
}

void GraphicsD3D12::createCommandLists(std::shared_ptr<CommandPool> pool, bool secondary, size_t count,
									   std::vector<std::shared_ptr<CommandList>> out)
{
	for (size_t i = 0; i < count; ++i)
	{
		std::shared_ptr<CommandList> commandList = createCommandList(pool, secondary);
		out.push_back(commandList);
	}
}

void GraphicsD3D12::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
	std::shared_ptr<CommandListD3D12> commandListD3D = std::static_pointer_cast<CommandListD3D12>(commandList);
	GP_SAFE_RELEASE(commandListD3D->_commandList);
	commandList.reset();
}

void GraphicsD3D12::destroyCommandLists(std::vector<std::shared_ptr<CommandList>> commandLists)
{
	for (size_t i = 0; i < commandLists.size(); ++i)
	{
		destroyCommandList(commandLists[i]);
	}
}

void GraphicsD3D12::submitCommands(std::shared_ptr<CommandList> commands)
{
}

void GraphicsD3D12::submitCommands(std::vector<std::shared_ptr<CommandList>> commands)
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

ID3D12Resource* GraphicsD3D12::createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible)
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

	ID3D12Resource* resource = nullptr;
	if (FAILED(_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource))))
	{
		GP_ERROR("Failed to create buffer!\n");
	}
	return resource;
}

std::shared_ptr<Buffer> GraphicsD3D12::createVertexBuffer(size_t size, size_t vertexStride, bool hostVisible)
{
	ID3D12Resource* bufferD3D = createBuffer(Buffer::USAGE_VERTEX, size, vertexStride, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_VERTEX, size, vertexStride, hostVisible, _device, bufferD3D);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(bufferD3D->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
		}
    }
	buffer->_vertexBufferView.BufferLocation = bufferD3D->GetGPUVirtualAddress();
	buffer->_vertexBufferView.SizeInBytes = size;
	buffer->_vertexBufferView.StrideInBytes = vertexStride;

	return std::static_pointer_cast<Buffer>(buffer);
}

std::shared_ptr<Buffer> GraphicsD3D12::createIndexBuffer(size_t size, IndexFormat indexFormat, bool hostVisible)
{
	size_t stride = (indexFormat == INDEX_FORMAT_UINT) ? sizeof(unsigned int) : sizeof(unsigned short);
	ID3D12Resource* bufferD3D = createBuffer(Buffer::USAGE_INDEX, size, stride, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_INDEX, size, stride, hostVisible, _device, bufferD3D);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(bufferD3D->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
		}
    }
	buffer->_indexBufferView.BufferLocation = bufferD3D->GetGPUVirtualAddress();
	buffer->_indexBufferView.SizeInBytes = size;
	buffer->_indexBufferView.Format = (indexFormat == INDEX_FORMAT_UINT) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

	return std::static_pointer_cast<Buffer>(buffer);
}

std::shared_ptr<Buffer> GraphicsD3D12::createUniformBuffer(size_t size, bool hostVisible)
{
	ID3D12Resource* bufferD3D = createBuffer(Buffer::USAGE_UNIFORM, size, size, hostVisible);
	std::shared_ptr<BufferD3D12> buffer = std::make_shared<BufferD3D12>(Buffer::USAGE_UNIFORM, size, size, hostVisible, _device, bufferD3D);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(bufferD3D->Map(0, &readRange, reinterpret_cast<void**>(&buffer->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
		}
    }
	buffer->_constantBufferView.BufferLocation = bufferD3D->GetGPUVirtualAddress();
	buffer->_constantBufferView.SizeInBytes = size;

	return std::static_pointer_cast<Buffer>(buffer);
}

void GraphicsD3D12::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
	std::shared_ptr<BufferD3D12> bufferD3D = std::static_pointer_cast<BufferD3D12>(buffer);
	GP_SAFE_RELEASE(bufferD3D->_buffer);
	bufferD3D.reset();
}

ID3D12Resource* GraphicsD3D12::createTexture(Texture::Type type,
											 size_t width, size_t height, size_t depth, size_t mipLevels,
											 Format pixelFormat,
											 Texture::Usage usage,
											 Texture::SampleCount sampleCount,
											 bool hostVisible, D3D12_SHADER_RESOURCE_VIEW_DESC* textureView)
{
	
	D3D12_RESOURCE_DIMENSION resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	D3D12_SRV_DIMENSION viewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	switch (type) 
	{
	case Texture::TYPE_1D: 
		resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		viewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
		break;
	case Texture::TYPE_2D:
		resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		viewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		break;
	case Texture::TYPE_3D:
		resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D; 
		viewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		break;
    }

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	DXGI_FORMAT format = toFormat(pixelFormat);

	D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = resourceDimension;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = width;
    resourceDesc.Height = height;
    resourceDesc.DepthOrArraySize = depth;
    resourceDesc.MipLevels = (UINT16)mipLevels;
	resourceDesc.Format = format;
    resourceDesc.SampleDesc.Count = toSamples(sampleCount);
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    if (usage & Texture::USAGE_COLOR_ATTACHMENT) 
	{
        resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (usage & Texture::USAGE_DEPTH_STENCIL_ATTACHMENT) 
	{
        resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
	D3D12_RESOURCE_STATES resourceStates = toResourceStates(usage);

	ID3D12Resource* resource = nullptr;
	if (FAILED(_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource))))
	{
		GP_ERROR("Failed to create texture!\n");
	}

	textureView->Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	textureView->Format = format;
    textureView->ViewDimension = viewDimension;
    textureView->Texture2D.MipLevels = (UINT)mipLevels;

	return resource;
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture1d(size_t width, 
														Format pixelFormat,
														Texture::Usage usage, 
														Texture::SampleCount sampleCount, 
														bool hostVisible)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	D3D12_SHADER_RESOURCE_VIEW_DESC  textureView;
	ID3D12Resource* textureD3D = createTexture(Texture::TYPE_1D, width, 1, 1, 1, pixelFormat, usage, sampleCount, hostVisible, &textureView);	
	std::shared_ptr<TextureD3D12> texture = std::make_shared<TextureD3D12>(Texture::TYPE_1D, width, 1, 1, 1, pixelFormat, usage, sampleCount, hostVisible,
																		   _device, textureD3D, textureView);	
	return std::static_pointer_cast<Texture>(texture);

}

std::shared_ptr<Texture> GraphicsD3D12::createTexture2d(size_t width, size_t height, size_t mipLevels,
														Format pixelFormat,
														Texture::Usage usage,
														Texture::SampleCount sampleCount,
														bool hostVisible)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	if (mipLevels >= GP_GRAPHICS_MIP_LEVELS_MAX)
	{
        mipLevels = Graphics::computeMipLevels(width, height);
    }
	D3D12_SHADER_RESOURCE_VIEW_DESC  textureView;
	ID3D12Resource* textureD3D = createTexture(Texture::TYPE_2D, width, height, 1, mipLevels, pixelFormat, usage, sampleCount, hostVisible, &textureView);	
	std::shared_ptr<TextureD3D12> texture = std::make_shared<TextureD3D12>(Texture::TYPE_2D, width, height, 1, mipLevels, pixelFormat, usage, sampleCount, hostVisible,
																		   _device, textureD3D, textureView);	
	return std::static_pointer_cast<Texture>(texture);
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture3d(size_t width, size_t height, size_t depth,
														Format pixelFormat,
														Texture::Usage usage,
														Texture::SampleCount sampleCount,
														bool hostVisible)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	D3D12_SHADER_RESOURCE_VIEW_DESC  textureView;
	ID3D12Resource* textureD3D = createTexture(Texture::TYPE_3D, width, height, depth, 1, pixelFormat, usage, sampleCount, hostVisible, &textureView);	
	std::shared_ptr<TextureD3D12> texture = std::make_shared<TextureD3D12>(Texture::TYPE_3D, width, height, depth, 1, pixelFormat, usage, sampleCount, hostVisible,
																		   _device, textureD3D, textureView);	
	return std::static_pointer_cast<Texture>(texture);
}

void GraphicsD3D12::destroyTexture(std::shared_ptr<Texture> texture)
{
	std::shared_ptr<TextureD3D12> textureD3D = std::static_pointer_cast<TextureD3D12>(texture);
	GP_SAFE_RELEASE(textureD3D->_texture);
	textureD3D.reset();
}

std::shared_ptr<Shader> GraphicsD3D12::createShader(const std::string& url)
{
	ID3DBlob* shaderBlob = nullptr;
	std::string shaderUrl = FileSystem::getHomePath();
	shaderUrl.append(GP_GRAPHICS_D3D12_SHADER_PATH);
	shaderUrl.append(url);
	shaderUrl.append(GP_GRAPHICS_D3D12_SHADER_EXT);
	std::wstring shaderUrl_wstr = std::wstring(shaderUrl.begin(), shaderUrl.end());
	LPCWSTR path = shaderUrl_wstr.c_str();
	D3D_CHECK_RESULT(D3DReadFileToBlob(path, &shaderBlob));
	std::shared_ptr<ShaderD3D12> shader = std::make_shared<ShaderD3D12>(_device, shaderBlob);	
	return std::static_pointer_cast<Shader>(shader);
}

void GraphicsD3D12::destroyShader(std::shared_ptr<Shader> shader)
{
	std::shared_ptr<ShaderD3D12> shaderD3D = std::static_pointer_cast<ShaderD3D12>(shader);
	GP_SAFE_RELEASE(shaderD3D->_shader);
	shaderD3D.reset();
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

DXGI_FORMAT GraphicsD3D12::toFormat(Format pixelFormat)
{
	DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;
    switch (pixelFormat) 
	{
	case Format::FORMAT_R8_UNORM: 
		result = DXGI_FORMAT_R8_UNORM; 
		break;
    case Format::FORMAT_R16_UNORM: 
		result = DXGI_FORMAT_R16_UNORM; 
		break;
    case Format::FORMAT_R16_FLOAT: 
		result = DXGI_FORMAT_R16_FLOAT; 
		break;
    case Format::FORMAT_R32_UINT: 
		result = DXGI_FORMAT_R32_UINT; 
		break;
    case Format::FORMAT_R32_FLOAT: 
		result = DXGI_FORMAT_R32_FLOAT; 
		break;        
    case Format::FORMAT_R8G8_UNORM: 
		result = DXGI_FORMAT_R8G8_UNORM; 
		break;
    case Format::FORMAT_R16G16_UNORM: 
		result = DXGI_FORMAT_R16G16_UNORM; 
		break;
    case Format::FORMAT_R16G16_FLOAT: 
		result = DXGI_FORMAT_R16G16_FLOAT; 
		break;
    case Format::FORMAT_R32G32_UINT: 
		result = DXGI_FORMAT_R32G32_UINT; 
		break;
    case Format::FORMAT_R32G32_FLOAT: 
		result = DXGI_FORMAT_R32G32_FLOAT; 
		break;
    case Format::FORMAT_R32G32B32_UINT: 
		result = DXGI_FORMAT_R32G32B32_UINT; 
		break;
    case Format::FORMAT_R32G32B32_FLOAT: 
		result = DXGI_FORMAT_R32G32B32_FLOAT; 
		break;
    case Format::FORMAT_B8G8R8A8_UNORM: 
		result = DXGI_FORMAT_B8G8R8A8_UNORM; 
		break;
    case Format::FORMAT_R8G8B8A8_UNORM: 
		result = DXGI_FORMAT_R8G8B8A8_UNORM; 
		break;
    case Format::FORMAT_R16G16B16A16_UNORM: 
		result = DXGI_FORMAT_R16G16B16A16_UNORM; 
		break;
    case Format::FORMAT_R16G16B16A16_FLOAT: 
		result = DXGI_FORMAT_R16G16B16A16_FLOAT; 
		break;
    case Format::FORMAT_R32G32B32A32_UINT: 
		result = DXGI_FORMAT_R32G32B32A32_UINT; 
		break;
    case Format::FORMAT_R32G32B32A32_FLOAT: 
		result = DXGI_FORMAT_R32G32B32A32_FLOAT; 
		break;
    case Format::FORMAT_D16_UNORM: 
		result = DXGI_FORMAT_D16_UNORM; 
		break;
    case Format::FORMAT_X8_D24_UNORM_PACK32: 
		result = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT; 
		break;
    case Format::FORMAT_D32_FLOAT: 
		result = DXGI_FORMAT_D32_FLOAT; 
		break;
    case Format::FORMAT_D24_UNORM_S8_UINT: 
		result = DXGI_FORMAT_D24_UNORM_S8_UINT; 
		break;
    case Format::FORMAT_D32_FLOAT_S8_UINT: 
		result = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; 
		break;
    }
    return result;
}

UINT GraphicsD3D12::toSamples(Texture::SampleCount sampleCount)
{
	UINT result = 1;
	switch (sampleCount)
	{
	case Texture::SAMPLE_COUNT_1X:
		result = 1;
	case Texture::SAMPLE_COUNT_2X:
		result = 2;
	case Texture::SAMPLE_COUNT_4X:
		result = 4;
	case Texture::SAMPLE_COUNT_8X:
		result = 8;
	case Texture::SAMPLE_COUNT_16X:
		result = 16;
	}
	return result;
	
}

D3D12_RESOURCE_STATES GraphicsD3D12::toResourceStates(Texture::Usage usage)
{
	D3D12_RESOURCE_STATES result = D3D12_RESOURCE_STATE_COMMON;
    if (Texture::USAGE_TRANSFER_SRC == (usage & Texture::USAGE_TRANSFER_SRC)) 
	{
        result |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    }
    if (Texture::USAGE_TRANSFER_DST == (usage & Texture::USAGE_TRANSFER_DST)) 
	{
        result |= D3D12_RESOURCE_STATE_COPY_DEST;
    }
    if (Texture::USAGE_SAMPLED_IMAGE == (usage & Texture::USAGE_SAMPLED_IMAGE)) 
	{
        result |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    }
    if (Texture::USAGE_STORAGE == (usage & Texture::USAGE_STORAGE)) 
	{
        result |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    }
    if (Texture::USAGE_COLOR_ATTACHMENT == (usage & Texture::USAGE_COLOR_ATTACHMENT)) 
	{
        result |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    if (Texture::USAGE_DEPTH_STENCIL_ATTACHMENT == (usage & Texture::USAGE_DEPTH_STENCIL_ATTACHMENT)) 
	{
        result |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    }
    if (Texture::USAGE_RESOLVE_SRC == (usage & Texture::USAGE_RESOLVE_SRC)) {
        result |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
    }
    if (Texture::USAGE_RESOLVE_DST == (usage & Texture::USAGE_RESOLVE_DST)) {
        result |= D3D12_RESOURCE_STATE_RESOLVE_DEST;
    }
    return result;
}

}
