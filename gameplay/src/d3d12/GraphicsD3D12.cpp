#include "Base.h"
#include "GraphicsD3D12.h"
#include "UtilsD3D12.h"
#include "BufferD3D12.h"
#include "TextureD3D12.h"
#include "RenderPassD3D12.h"
#include "SamplerD3D12.h"
#include "ShaderD3D12.h"
#include "DescriptorSetD3D12.h"
#include "RenderPipelineD3D12.h"
#include "CommandBufferD3D12.h"
#include "Game.h"
#include "FileSystem.h"

namespace gameplay
{

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
	_queue(nullptr),
	_swapchain(nullptr),
	_swapchainImagesViewHeap(nullptr),
	_swapchainFenceEvent(0),
	_swapchainImageIndex(0),
	_renderPass(nullptr),
	_commandBuffer(nullptr)
{
}

GraphicsD3D12::~GraphicsD3D12()
{
	if(_swapchain)
		_swapchain->SetFullscreenState(false, nullptr);

	for (uint32_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	{
		GP_SAFE_RELEASE(_swapchainImages[i]);
	}
	GP_SAFE_RELEASE(_swapchainImagesViewHeap);
	GP_SAFE_RELEASE(_swapchain);
	GP_SAFE_RELEASE(_queue);
	GP_SAFE_RELEASE(_device);
}

int GraphicsD3D12::getWidth()
{
    return _width;
}

int GraphicsD3D12::getHeight()
{
    return _height;
}

std::shared_ptr<RenderPass> GraphicsD3D12::acquireNextSwapchainImage()
{
    _swapchainImageIndex = _swapchain->GetCurrentBackBufferIndex();
	return _renderPasses[_swapchainImageIndex];
}

void GraphicsD3D12::present()
{
	UINT syncInterval = 1;
    UINT flags = 0;
    _swapchain->Present(syncInterval, flags);
}

void GraphicsD3D12::waitIdle()
{
    if (_swapchainFences[_swapchainImageIndex]->GetCompletedValue() < _swapchainFenceValues[_swapchainImageIndex])
    {
		D3D_CHECK_RESULT(_swapchainFences[_swapchainImageIndex]->SetEventOnCompletion(_swapchainFenceValues[_swapchainImageIndex], _swapchainFenceEvent));

        WaitForSingleObject(_swapchainFenceEvent, INFINITE);
    }
    _swapchainFenceValues[_swapchainImageIndex]++;
}

std::shared_ptr<CommandBuffer> GraphicsD3D12::beginCommands()
{
	_commandBuffer = _commandBuffers[_swapchainImageIndex];
	std::shared_ptr<CommandBufferD3D12> commandBuffer = std::static_pointer_cast<CommandBufferD3D12>(_commandBuffer);

	D3D_CHECK_RESULT(commandBuffer->_commandAllocator->Reset());
	D3D_CHECK_RESULT(commandBuffer->_commandList->Reset(commandBuffer->_commandAllocator, nullptr));

	return _commandBuffer;
}

void GraphicsD3D12::endCommands()
{
	GP_ASSERT(_commandBuffer);

	D3D_CHECK_RESULT(std::static_pointer_cast<CommandBufferD3D12>(_commandBuffer)->_commandList->Close());
}

void GraphicsD3D12::submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
    ID3D12CommandList* commandLists[1];
	commandLists[0] = std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList;

    _queue->ExecuteCommandLists(1, commandLists);
}

void GraphicsD3D12::cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
								       std::shared_ptr<RenderPass> renderPass)
{
	GP_ASSERT(commandBuffer);
	GP_ASSERT(renderPass);

	std::shared_ptr<TextureD3D12> renderTexture = std::static_pointer_cast<TextureD3D12>(renderPass->getColorAttachment(0));
	D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = renderTexture->_resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE* rtv = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE* dsv = nullptr;

	std::shared_ptr<RenderPassD3D12> renderPassD3D = std::static_pointer_cast<RenderPassD3D12>(renderPass);
	if (renderPass->getColorAttachmentCount() > 0) 
	{
        rtvHandle = renderPassD3D->_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        rtv = &rtvHandle;
    }
	if (renderPass->getDepthStencilFormat() != Format::FORMAT_UNDEFINED) 
	{
        dsvHandle = renderPassD3D->_depthStencilHeap->GetCPUDescriptorHandleForHeapStart();
        dsv = &dsvHandle;
    }
    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->OMSetRenderTargets(renderPass->getColorAttachmentCount(), rtv, TRUE, dsv);
}

void GraphicsD3D12::cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer)
{
	GP_ASSERT(commandBuffer);

	if ((_renderPass != nullptr) && (_renderPass->getSampleCount() > Texture::SAMPLE_COUNT_1X)) 
	{
        std::shared_ptr<RenderPassD3D12> renderPassD3D = std::static_pointer_cast<RenderPassD3D12>(_renderPass);
        std::shared_ptr<Texture> colorAttachment = _renderPass->getColorAttachment(0);
        std::shared_ptr<Texture> colorMultisampleAttachment = _renderPass->getColorMultisampleAttachment(0);
        
        bool isUsagePresent = ( (colorMultisampleAttachment->getUsage() & Texture::USAGE_PRESENT) == Texture::USAGE_PRESENT);

        // This means we're dealing with a multisample swapchain
		uint32_t colorAttachmentCount = _renderPass->getColorAttachmentCount();
        if (colorAttachmentCount == 0 && isUsagePresent) 
		{
            if ((colorAttachment->getUsage() & Texture::USAGE_PRESENT) == Texture::USAGE_PRESENT)
			{
                // If the render targets have transitioned correctly, we can expect them to be in the require states
                cmdTransitionImage(commandBuffer, colorAttachment, Texture::USAGE_COLOR_ATTACHMENT, Texture::USAGE_RESOLVE_DST);
                cmdTransitionImage(commandBuffer, colorMultisampleAttachment, Texture::USAGE_COLOR_ATTACHMENT, Texture::USAGE_RESOLVE_SRC);

				std::shared_ptr<TextureD3D12> colorAttachmentD3D = std::static_pointer_cast<TextureD3D12>(colorAttachment);
				std::shared_ptr<TextureD3D12> colorMultisampleAttachmentD3D = std::static_pointer_cast<TextureD3D12>(colorMultisampleAttachment);

                // Resolve from multisample to single sample
                std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->ResolveSubresource(colorAttachmentD3D->_resource, 0, 
																											  colorMultisampleAttachmentD3D->_resource, 0, 
																											  lookupDXGI_FORMAT[_renderPass->getColorFormat()]);
				// Put it back the way we found it
                cmdTransitionImage(commandBuffer, colorAttachmentD3D, Texture::USAGE_RESOLVE_DST, Texture::USAGE_COLOR_ATTACHMENT);
                cmdTransitionImage(commandBuffer, colorMultisampleAttachmentD3D, Texture::USAGE_RESOLVE_SRC, Texture::USAGE_COLOR_ATTACHMENT);
            }
        }
    }
}

void GraphicsD3D12::cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
								   float x, float y, float width, float height, 
								   float depthMin, float depthMax)
{
	GP_ASSERT(commandBuffer);

	D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = x;
    viewport.TopLeftY = y;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = depthMin;
    viewport.MaxDepth = depthMax;

    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->RSSetViewports(1, &viewport);
}

void GraphicsD3D12::cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
							      size_t x, size_t y, 
							      size_t width, size_t height)
{
	GP_ASSERT(commandBuffer);

	D3D12_RECT scissor = {};
    scissor.left = x;
    scissor.top = y;
    scissor.right = x + width;
    scissor.bottom = y + height;

    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->RSSetScissorRects(1, &scissor);
}

void GraphicsD3D12::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
									      std::shared_ptr<RenderPipeline> renderPipeline)
{
	GP_ASSERT(commandBuffer);

	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	switch (renderPipeline->getPrimitiveTopology()) 
	{
	case RenderPipeline::PRIMITIVE_TOPOLOGY_POINT_LIST: 
		topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_LINE_LIST: 
		topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_LINE_STRIP: 
		topology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; 
		break;
    case RenderPipeline::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: 
		topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; 
		break;
    }

	std::shared_ptr<CommandBufferD3D12> commandBufferD3D = std::static_pointer_cast<CommandBufferD3D12>(commandBuffer);
	std::shared_ptr<RenderPipelineD3D12> renderPipelineD3D = std::static_pointer_cast<RenderPipelineD3D12>(renderPipeline);

    commandBufferD3D->_commandList->SetPipelineState(renderPipelineD3D->_pipelineState);
    commandBufferD3D->_commandList->SetGraphicsRootSignature(renderPipelineD3D->_rootSignature);
    commandBufferD3D->_commandList->IASetPrimitiveTopology(topology);
}

void GraphicsD3D12::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
									     std::shared_ptr<RenderPipeline> renderPipeline, 
									     std::shared_ptr<DescriptorSet> descriptorSet)
{
	GP_ASSERT(commandBuffer);

    ID3D12DescriptorHeap* descriptorHeaps[2];
    uint32_t descriptorHeapCount = 0;

	std::shared_ptr<DescriptorSetD3D12> descriptorSetD3D = std::static_pointer_cast<DescriptorSetD3D12>(descriptorSet);
	if (descriptorSetD3D->_cbvSrvUavHeap != nullptr) 
	{
        descriptorHeaps[descriptorHeapCount] = descriptorSetD3D->_cbvSrvUavHeap;
        ++descriptorHeapCount;
    }
    if (descriptorSetD3D->_samplerHeap != nullptr) 
	{
        descriptorHeaps[descriptorHeapCount] = descriptorSetD3D->_samplerHeap;
        ++descriptorHeapCount;
    }

	std::shared_ptr<CommandBufferD3D12> commandBufferD3D = std::static_pointer_cast<CommandBufferD3D12>(commandBuffer);
    if (descriptorHeapCount > 0) 
	{
        commandBufferD3D->_commandList->SetDescriptorHeaps(descriptorHeapCount, descriptorHeaps);
    }

    for (size_t i = 0; i < descriptorSet->getDescriptorCount(); i++) 
	{
		DescriptorSet::Descriptor descriptor = descriptorSet->getDescriptor(i);
		DescriptorSetD3D12::Binding descriptorBinding = descriptorSetD3D->_bindings[i];
		D3D12_GPU_DESCRIPTOR_HANDLE descriptorHandle;
        if (descriptorBinding.rootParameterIndex == UINT32_MAX) 
		{
            continue;
        }

        switch (descriptor.type)
		{
		case DescriptorSet::Descriptor::TYPE_SAMPLER:
            descriptorHandle = descriptorSetD3D->_samplerHeap->GetGPUDescriptorHandleForHeapStart();
            descriptorHandle.ptr += descriptorBinding.heapOffset * _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            commandBufferD3D->_commandList->SetGraphicsRootDescriptorTable(descriptorBinding.rootParameterIndex, descriptorHandle);
			break;

		case DescriptorSet::Descriptor::TYPE_TEXTURE:
        case DescriptorSet::Descriptor::TYPE_UNIFORM:
            descriptorHandle = descriptorSetD3D->_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
            descriptorHandle.ptr += descriptorBinding.heapOffset * _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            commandBufferD3D->_commandList->SetGraphicsRootDescriptorTable(descriptorBinding.rootParameterIndex, descriptorHandle);
			 break;
        }
    }
}

void GraphicsD3D12::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									    std::shared_ptr<Buffer> vertexBuffer)
{
	GP_ASSERT(commandBuffer != nullptr);
	GP_ASSERT(vertexBuffer != nullptr);

	std::shared_ptr<BufferD3D12> bufferD3D = std::static_pointer_cast<BufferD3D12>(vertexBuffer);
    GP_ASSERT(bufferD3D->_vertexBufferView.BufferLocation != 0);

	D3D12_VERTEX_BUFFER_VIEW views[1];
    views[0] = bufferD3D->_vertexBufferView;

    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->IASetVertexBuffers(0, 1, views);
}

void GraphicsD3D12::cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
									     std::vector<std::shared_ptr<Buffer>> vertexBuffers)
{
	GP_ASSERT(commandBuffer != nullptr);

	D3D12_VERTEX_BUFFER_VIEW views[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX];

	size_t vertexBufferCount = vertexBuffers.size();
    for (size_t i = 0; i < vertexBufferCount; ++i) 
	{
        std::shared_ptr<BufferD3D12> bufferD3D = std::static_pointer_cast<BufferD3D12>(vertexBuffers[i]);
		GP_ASSERT(bufferD3D->_vertexBufferView.BufferLocation != 0);

        views[i] = bufferD3D->_vertexBufferView;
    }
    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->IASetVertexBuffers(0, vertexBufferCount, views);
}

void GraphicsD3D12::cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
									   std::shared_ptr<Buffer> indexBuffer)
{
	GP_ASSERT(commandBuffer);
	GP_ASSERT(indexBuffer);

    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->IASetIndexBuffer(&std::static_pointer_cast<BufferD3D12>(indexBuffer)->_indexBufferView);
}

void GraphicsD3D12::cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
								  float red, float green, float blue, float alpha, 
								  size_t attachmentIndex)
{
	GP_ASSERT(commandBuffer);

	std::shared_ptr<CommandBufferD3D12> commandBufferD3D = std::static_pointer_cast<CommandBufferD3D12>(commandBuffer);
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = std::static_pointer_cast<RenderPassD3D12>(_renderPass)->_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    UINT rtvIncrementSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    renderTargetView.ptr += attachmentIndex * rtvIncrementSize;

	FLOAT clearColor[4] = {};
    clearColor[0] = red;
    clearColor[1] = green;
    clearColor[2] = blue;
    clearColor[3] = alpha;

    commandBufferD3D->_commandList->ClearRenderTargetView(renderTargetView, clearColor, 0, nullptr);
}

void GraphicsD3D12::cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
									     float depth, uint32_t stencil)
{
	GP_ASSERT(commandBuffer);

	std::shared_ptr<CommandBufferD3D12> commandBufferD3D = std::static_pointer_cast<CommandBufferD3D12>(commandBuffer);
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = std::static_pointer_cast<RenderPassD3D12>(_renderPass)->_depthStencilHeap->GetCPUDescriptorHandleForHeapStart();

	commandBufferD3D->_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
}

void GraphicsD3D12::cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
							size_t vertexCount, size_t vertexStart)
{
	GP_ASSERT(commandBuffer);

	std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->DrawInstanced((UINT)vertexCount, 
																							 (UINT)1, 
																							 (UINT)vertexStart, 
																							 (UINT)0);
}

void GraphicsD3D12::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
								   size_t indexCount, size_t indexStart)
{
	GP_ASSERT(commandBuffer);

	std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->DrawIndexedInstanced((UINT)indexCount, 
																									(UINT)1, 
																									(UINT)indexStart,
																								    (UINT)0,
																									(UINT)0);
}

void GraphicsD3D12::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
									   std::shared_ptr<Texture> texture, 
									   Texture::Usage usageOld, 
									   Texture::Usage usageNew)
{
	GP_ASSERT(commandBuffer);
	GP_ASSERT(texture);

	D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = std::static_pointer_cast<TextureD3D12>(texture)->_resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = toD3D12_RESOURCE_STATES(usageOld);
    barrier.Transition.StateAfter = toD3D12_RESOURCE_STATES(usageNew);

    std::static_pointer_cast<CommandBufferD3D12>(commandBuffer)->_commandList->ResourceBarrier(1, &barrier);
}


std::shared_ptr<Buffer> GraphicsD3D12::createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible, bool is32bit)
{
	if (usage == Buffer::USAGE_UNIFORM)
	{
		size = GP_MATH_ROUNDUP(size, 256);
	}
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

	std::shared_ptr<BufferD3D12> bufferD3D = std::make_shared<BufferD3D12>(Buffer::USAGE_INDEX, size, stride, hostVisible, _device, resource);
	if (hostVisible) 
	{
        D3D12_RANGE readRange = { 0, 0 };
		if (FAILED(resource->Map(0, &readRange, reinterpret_cast<void**>(&bufferD3D->_hostMemory))))
		{
			GP_ERROR("Failed to map host memory.");
		}
    }

	switch (usage)
	{
	case Buffer::USAGE_VERTEX: 
		bufferD3D->_vertexBufferView.BufferLocation = bufferD3D->_buffer->GetGPUVirtualAddress();
		bufferD3D->_vertexBufferView.SizeInBytes = size;
		bufferD3D->_vertexBufferView.StrideInBytes = stride;
		break;
    case Buffer::USAGE_INDEX: 
		bufferD3D->_indexBufferView.BufferLocation = bufferD3D->_buffer->GetGPUVirtualAddress();
		bufferD3D->_indexBufferView.SizeInBytes = size;
		bufferD3D->_indexBufferView.Format = is32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		break;

	case Buffer::USAGE_UNIFORM: 
        bufferD3D->_constantBufferView.BufferLocation = bufferD3D->_buffer->GetGPUVirtualAddress();
		bufferD3D->_constantBufferView.SizeInBytes = size;
		break;
	}
	return bufferD3D;
}

std::shared_ptr<Buffer> GraphicsD3D12::createVertexBuffer(size_t size, 
														  size_t vertexStride, 
														  bool hostVisible,
														  const void* data)
{
	return createBuffer(Buffer::USAGE_VERTEX, size, vertexStride, hostVisible, true);
}

std::shared_ptr<Buffer> GraphicsD3D12::createIndexBuffer(size_t size, 
														 size_t indexStride,
														 bool hostVisible,
														 const void* data)
{
	GP_ASSERT(indexStride == sizeof(uint32_t) || indexStride == sizeof(uint16_t));
	return createBuffer(Buffer::USAGE_INDEX, size, indexStride, hostVisible, indexStride == sizeof(uint32_t));
}

std::shared_ptr<Buffer> GraphicsD3D12::createUniformBuffer(size_t size, 
														   bool hostVisible,
														   const void* data)
{
	return createBuffer(Buffer::USAGE_UNIFORM, size, size, hostVisible, true);
}

void GraphicsD3D12::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
	std::shared_ptr<BufferD3D12> bufferD3D = std::static_pointer_cast<BufferD3D12>(buffer);
	GP_SAFE_RELEASE(bufferD3D->_buffer);
	bufferD3D.reset();
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture(Texture::Type type,
													  size_t width, size_t height, size_t depth, size_t mipLevels,
													  Format pixelFormat,
													  Texture::Usage usage,
													  Texture::SampleCount sampleCount,
													  bool hostVisible,
													  const void* data,
													  ID3D12Resource* resource)
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

	DXGI_FORMAT format = lookupDXGI_FORMAT[pixelFormat];

	D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = resourceDimension;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = width;
    resourceDesc.Height = height;
    resourceDesc.DepthOrArraySize = depth;
    resourceDesc.MipLevels = (UINT16)mipLevels;
	resourceDesc.Format = format;
    resourceDesc.SampleDesc.Count = sampleCount;
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
	D3D12_RESOURCE_STATES resourceStates = toD3D12_RESOURCE_STATES(usage);

	bool hostOwned;
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc {};
	if (resource == nullptr)
	{
		if (FAILED(_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource))))
		{
			GP_ERROR("Failed to create texture!\n");
		}
		hostOwned = true;
	}
	else
	{
		hostOwned = false;
	}
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resourceViewDesc.Format = format;
	resourceViewDesc.ViewDimension = viewDimension;
	resourceViewDesc.Texture2D.MipLevels = (UINT)mipLevels;
	std::shared_ptr<TextureD3D12> textureD3D = std::make_shared<TextureD3D12>(Texture::TYPE_3D, width, height, depth, 1, 
																			   pixelFormat, usage, sampleCount, hostVisible, hostOwned,
																		       _device, resource);
	textureD3D->_resourceViewDesc = resourceViewDesc;
	return std::static_pointer_cast<Texture>(textureD3D);
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture1d(size_t width, 
														Format pixelFormat,
														Texture::Usage usage, 
														Texture::SampleCount sampleCount,
														bool hostVisible,
													    const void* data)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	return createTexture(Texture::TYPE_1D, width, 1, 1, 1, 
						 pixelFormat, usage, sampleCount, hostVisible, data, nullptr);
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture2d(size_t width, size_t height, size_t mipLevels,
														Format pixelFormat,
														Texture::Usage usage,
														Texture::SampleCount sampleCount,
														bool hostVisible,
													    const void* data)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	if (mipLevels >= GP_GRAPHICS_MIP_LEVELS_MAX)
	{
        mipLevels = Graphics::computeMipLevels(width, height);
    }
	return createTexture(Texture::TYPE_2D, width, height, 1, mipLevels, 
						 pixelFormat, usage, sampleCount, hostVisible, data, nullptr);
}

std::shared_ptr<Texture> GraphicsD3D12::createTexture3d(size_t width, size_t height, size_t depth,
														Format pixelFormat,
														Texture::Usage usage,
														Texture::SampleCount sampleCount,
														bool hostVisible,
													    const void* data)
{
	GP_ASSERT(pixelFormat != Format::FORMAT_UNDEFINED);

	return createTexture(Texture::TYPE_3D, width, height, depth, 1, 
						 pixelFormat, usage, sampleCount, hostVisible, data, nullptr);	
}

void GraphicsD3D12::destroyTexture(std::shared_ptr<Texture> texture)
{
	std::shared_ptr<TextureD3D12> textureD3D = std::static_pointer_cast<TextureD3D12>(texture);
	GP_SAFE_RELEASE(textureD3D->_resource);
	textureD3D.reset();
}

std::shared_ptr<RenderPass> GraphicsD3D12::createRenderPass(size_t width, size_t height, 
															size_t colorAttachmentCount,
															Format colorFormat,
															Format depthStencilFormat,
															Texture::SampleCount sampleCount,
															std::vector<std::shared_ptr<Texture>> colorAttachments,
															std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
															std::shared_ptr<Texture> depthStencilAttachment)
{
	 ID3D12DescriptorHeap* renderTargetHeap;
	 if (colorAttachmentCount > 0) 
	 {
		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
        descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        descriptorHeapDesc.NumDescriptors = (UINT)colorAttachmentCount;
        descriptorHeapDesc.NodeMask = 0;
        descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		
        D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(renderTargetHeap), (void**)&(renderTargetHeap)));

        D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandle = renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        const UINT renderTargetIncrementSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        for (uint32_t i = 0; i < colorAttachmentCount; ++i) 
		{
            if (sampleCount > Texture::SAMPLE_COUNT_1X) 
			{
				GP_ASSERT(colorMultisampleAttachments[i] != nullptr);

				std::shared_ptr<TextureD3D12> colorMultisampleAttachmentD3D = std::static_pointer_cast<TextureD3D12>(colorMultisampleAttachments[i]);
                _device->CreateRenderTargetView(colorMultisampleAttachmentD3D->_resource, nullptr, renderTargetHandle);
            }
            else 
			{
                GP_ASSERT(colorAttachments[i] != nullptr);

				std::shared_ptr<TextureD3D12> colorAttachmentD3D = std::static_pointer_cast<TextureD3D12>(colorAttachments[i]);
                _device->CreateRenderTargetView(colorAttachmentD3D->_resource, nullptr, renderTargetHandle);
            }
            renderTargetHandle.ptr += renderTargetIncrementSize;
        }
    }

	ID3D12DescriptorHeap* depthStencilHeap;
    if (depthStencilFormat != Format::FORMAT_UNDEFINED) 
	{
        GP_ASSERT(depthStencilAttachment != nullptr);

		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
        descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        descriptorHeapDesc.NumDescriptors = 1;
        descriptorHeapDesc.NodeMask = 0;
        descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		
		D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(depthStencilHeap), (void**)&(depthStencilHeap)));
        
		std::shared_ptr<TextureD3D12> depthStencilAttachmentD3D = std::static_pointer_cast<TextureD3D12>(depthStencilAttachment);
        D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle = depthStencilHeap->GetCPUDescriptorHandleForHeapStart();
        const UINT depthStencilIncrementSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        _device->CreateDepthStencilView(depthStencilAttachmentD3D->_resource, nullptr, depthStencilHandle);
    }

	std::shared_ptr<RenderPassD3D12> renderPassD3D = std::make_shared<RenderPassD3D12>(width, height,
																					   colorAttachmentCount, 
																					   colorFormat,
																			           depthStencilFormat, 
																			           sampleCount,
																			           colorAttachments,
																			           colorMultisampleAttachments,
																			           depthStencilAttachment,
																			           _device, 
																					   renderTargetHeap, 
																					   depthStencilHeap);
	return std::static_pointer_cast<RenderPass>(renderPassD3D);
}

std::shared_ptr<RenderPass> GraphicsD3D12::createRenderPass(size_t width, size_t height,
														    size_t colorAttachmentCount,
															Format colorFormat,
															Format depthStencilFormat,
															Texture::SampleCount sampleCount)
{
	// Create the textures for the framebuffer
	std::vector<std::shared_ptr<Texture>> colorAttachments;
	std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;
	std::shared_ptr<Texture> depthStencilAttachment = nullptr;

	for (size_t i = 0; i < colorAttachmentCount; ++i)
	{
		std::shared_ptr<Texture> colorAttachment = createTexture2d(width, height, 1, colorFormat, 
																   Texture::USAGE_COLOR_ATTACHMENT, 
																   Texture::SAMPLE_COUNT_1X, false, nullptr);
		std::shared_ptr<TextureD3D12> colorAttachmentD3D = std::static_pointer_cast<TextureD3D12>(colorAttachment);
		colorAttachments.push_back(colorAttachment);
		if (sampleCount > Texture::SAMPLE_COUNT_1X)
		{
			std::shared_ptr<Texture> colorMultisampleAttachment = createTexture2d(width, height, 1, colorFormat, 
																				  Texture::USAGE_COLOR_ATTACHMENT, 
																				  sampleCount, false, nullptr);
			colorMultisampleAttachments.push_back(colorMultisampleAttachment);
		}
	}
	if (depthStencilFormat != Format::FORMAT_UNDEFINED)
	{
		depthStencilAttachment = createTexture2d(width, height, 1, depthStencilFormat, 
												 Texture::USAGE_DEPTH_STENCIL_ATTACHMENT, 
												 Texture::SAMPLE_COUNT_1X, false, nullptr);
	}

	return createRenderPass(width, height, colorAttachmentCount, colorFormat, depthStencilFormat, sampleCount,
							colorAttachments, colorMultisampleAttachments, depthStencilAttachment);
}

void GraphicsD3D12::destroyRenderPass(std::shared_ptr<RenderPass> renderPass)
{
	std::shared_ptr<RenderPassD3D12> renderPassD3D = std::static_pointer_cast<RenderPassD3D12>(renderPass);
	GP_SAFE_RELEASE(renderPassD3D->_renderTargetHeap);
	GP_SAFE_RELEASE(renderPassD3D->_depthStencilHeap);
	renderPass.reset();
}

std::shared_ptr<Sampler> GraphicsD3D12::createSampler(Sampler::Filter filterMin,
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
													  float lodMipBias)
{
	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = toD3D12_FILTER(filterMin, filterMag, filterMip);
	samplerDesc.AddressU = lookupD3D12_TEXTURE_ADDRESS_MODE[addressModeU];
    samplerDesc.AddressV = lookupD3D12_TEXTURE_ADDRESS_MODE[addressModeV];
    samplerDesc.AddressW = lookupD3D12_TEXTURE_ADDRESS_MODE[addressModeW];
	samplerDesc.MaxAnisotropy = anisotropyEnabled ? anisotropyMax : 0;
	samplerDesc.ComparisonFunc = lookupD3D12_COMPARISON_FUNC[compareFunc];
    toD3D12_BORDER_COLOR(borderColor, samplerDesc.BorderColor);
    samplerDesc.MinLOD = lodMin;
    samplerDesc.MaxLOD = lodMax;
	samplerDesc.MipLODBias = lodMipBias;

	std::shared_ptr<SamplerD3D12> samplerD3D = std::make_shared<SamplerD3D12>(filterMin,
																			  filterMag,
																			  filterMip,
																			  addressModeU,
																			  addressModeV,
																			  addressModeW,
																			  borderColor,
																			  compareEnabled,
																			  compareFunc,
																			  anisotropyEnabled,
																			  anisotropyMax,
																			  lodMin,
																			  lodMax,
																			  lodMipBias,
																			  _device, 
																			  samplerDesc);
	return std::static_pointer_cast<SamplerD3D12>(samplerD3D);
}

void GraphicsD3D12::destroySampler(std::shared_ptr<Sampler> sampler)
{
	sampler.reset();
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


std::shared_ptr<DescriptorSet> GraphicsD3D12::createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
																  size_t descriptorCount)
{
	uint32_t cbvSrvUavCount = 0;
    uint32_t samplerCount = 0;

    for (uint32_t i = 0; i < descriptorCount; i++) 
	{
        size_t count = descriptors[i].count;
        switch (descriptors[i].type) 
		{
		case DescriptorSet::Descriptor::TYPE_UNIFORM:
		case DescriptorSet::Descriptor::TYPE_TEXTURE:
			cbvSrvUavCount += count; 
			break;
		case DescriptorSet::Descriptor::TYPE_SAMPLER: 
			samplerCount += count; 
			break;
        }
    }

	ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
    if (cbvSrvUavCount > 0) 
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
        cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvSrvUavHeapDesc.NumDescriptors = cbvSrvUavCount;
        cbvSrvUavHeapDesc.NodeMask = 0;
        cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, __uuidof(cbvSrvUavHeap), (void**)&(cbvSrvUavHeap)));
    }

	ID3D12DescriptorHeap* samplerHeap = nullptr;
    if (samplerCount > 0) 
	{
		D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
        samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        samplerHeapDesc.NumDescriptors = samplerCount;
        samplerHeapDesc.NodeMask = 0;
        samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&samplerHeapDesc, __uuidof(samplerHeap), (void**)&(samplerHeap)));
    }

    
    uint32_t cbvSrvUavHeapOffset = 0;
    uint32_t samplerHeapOffset = 0;
	std::shared_ptr<DescriptorSetD3D12> descriptorSetD3D = std::make_shared<DescriptorSetD3D12>(descriptors, descriptorCount,
																							    _device, cbvSrvUavHeap, samplerHeap);
    for (uint32_t i = 0; i < descriptorCount; ++i) 
	{
        switch (descriptors[i].type) 
		{
		case DescriptorSet::Descriptor::TYPE_UNIFORM:
		case DescriptorSet::Descriptor::TYPE_TEXTURE:
           descriptorSetD3D->_bindings[i].heapOffset = cbvSrvUavHeapOffset;
            cbvSrvUavHeapOffset += descriptors[i].count;
			break;
        case DescriptorSet::Descriptor::TYPE_SAMPLER: 
            descriptorSetD3D->_bindings[i].heapOffset = samplerHeapOffset;
            samplerHeapOffset += descriptors[i].count;
			break;
        }
    }
	return std::static_pointer_cast<DescriptorSet>(descriptorSetD3D);
}

void GraphicsD3D12::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet)
{
	std::shared_ptr<DescriptorSetD3D12> descriptorSetD3D = std::static_pointer_cast<DescriptorSetD3D12>(descriptorSet); 
	GP_SAFE_RELEASE(descriptorSetD3D->_cbvSrvUavHeap);
	GP_SAFE_RELEASE(descriptorSetD3D->_samplerHeap);
	descriptorSet.reset();
}

std::shared_ptr<RenderPipeline> GraphicsD3D12::createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
																	std::shared_ptr<Shader> fragShader)
{
	// Create root signature
	//
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
    if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	{
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    uint32_t rangeCount = 0;
    D3D12_DESCRIPTOR_RANGE1* ranges11 = nullptr;
    D3D12_DESCRIPTOR_RANGE*  ranges10 = nullptr;
    uint32_t parameterCount = 0;
    D3D12_ROOT_PARAMETER1* parameters11 = nullptr;
    D3D12_ROOT_PARAMETER*  parameters10 = nullptr;

	size_t descriptorCount = descriptorSet->getDescriptorCount();
    size_t cbvSrvUavCount = 0;
    size_t samplerCount = 0;
    for (size_t i = 0; i < descriptorCount; ++i) 
	{
		DescriptorSet::Descriptor descriptor = descriptorSet->getDescriptor(i);
        uint32_t count = descriptor.count;
		switch (descriptor.type)
		{
		case DescriptorSet::Descriptor::TYPE_UNIFORM:
		case DescriptorSet::Descriptor::TYPE_TEXTURE:
			cbvSrvUavCount += count;
			break;
		case DescriptorSet::Descriptor::TYPE_SAMPLER:
			samplerCount += count;
			break;
		}

        ranges11 = (D3D12_DESCRIPTOR_RANGE1*)calloc(descriptorCount, sizeof(*ranges11));
        ranges10 = (D3D12_DESCRIPTOR_RANGE*)calloc(descriptorCount, sizeof(*ranges10));
        parameters11 = (D3D12_ROOT_PARAMETER1*)calloc(descriptorCount, sizeof(*parameters11));
        parameters10 = (D3D12_ROOT_PARAMETER*)calloc(descriptorCount, sizeof(*parameters10));

        for (size_t i = 0; i < descriptorCount; i++) 
		{
            D3D12_DESCRIPTOR_RANGE1* range11 = &ranges11[rangeCount];
            D3D12_DESCRIPTOR_RANGE* range10 = &ranges10[rangeCount];
            D3D12_ROOT_PARAMETER1* param11 = &parameters11[parameterCount];
            D3D12_ROOT_PARAMETER* param10 = &parameters10[parameterCount];
            param11->ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param10->ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            uint32_t shaderStageCount = 0;
            
            if (descriptor.shaderStages & DescriptorSet::Descriptor::SHADER_STAGE_VERT)
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                ++shaderStageCount;
            }
            if (descriptor.shaderStages & DescriptorSet::Descriptor::SHADER_STAGE_TESC)
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
                ++shaderStageCount;
            }
            if (descriptor.shaderStages & DescriptorSet::Descriptor::SHADER_STAGE_TESE)
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
                ++shaderStageCount;
            }
            if (descriptor.shaderStages  & DescriptorSet::Descriptor::SHADER_STAGE_GEOM)
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
                ++shaderStageCount;
            }
            if (descriptor.shaderStages & DescriptorSet::Descriptor::SHADER_STAGE_FRAG)
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                ++shaderStageCount;
            }

            if (shaderStageCount > 1) 
			{
                param11->ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                param10->ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            }

            bool assignRange = false;
            switch (descriptor.type) 
			{
            case DescriptorSet::Descriptor::TYPE_UNIFORM:
                range11->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                range10->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                assignRange = true;
				break;
            case DescriptorSet::Descriptor::TYPE_TEXTURE:
                range11->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                range10->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                assignRange = true;
				break;
            case DescriptorSet::Descriptor::TYPE_SAMPLER:
                range11->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                range10->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                assignRange = true;
                break;
            }

            if (assignRange) 
			{
                range11->NumDescriptors = descriptor.count;
                range11->BaseShaderRegister = descriptor.binding;
                range11->RegisterSpace = 0;
                range11->Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
                range11->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                range10->NumDescriptors = descriptor.count;
                range10->BaseShaderRegister = descriptor.binding;
                range10->RegisterSpace = 0;
                range10->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                param11->DescriptorTable.pDescriptorRanges = range11;
                param11->DescriptorTable.NumDescriptorRanges = 1;

                param10->DescriptorTable.pDescriptorRanges = range10;
                param10->DescriptorTable.NumDescriptorRanges = 1;

				std::shared_ptr<DescriptorSetD3D12> descriptorSetD3D = std::static_pointer_cast<DescriptorSetD3D12>(descriptorSet);
                descriptorSetD3D->_bindings[i].rootParameterIndex = parameterCount;

                ++rangeCount;
                ++parameterCount;
            }
        }
    }

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    if (D3D_ROOT_SIGNATURE_VERSION_1_1 == featureData.HighestVersion) 
	{
        rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
        rootSignatureDesc.Desc_1_1.NumParameters = parameterCount;
        rootSignatureDesc.Desc_1_1.pParameters = parameters11;
        rootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
        rootSignatureDesc.Desc_1_1.pStaticSamplers = nullptr;
        rootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    }
    else if (D3D_ROOT_SIGNATURE_VERSION_1_0 == featureData.HighestVersion) 
	{
        rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
        rootSignatureDesc.Desc_1_0.NumParameters = parameterCount;
        rootSignatureDesc.Desc_1_0.pParameters = parameters10;
        rootSignatureDesc.Desc_1_0.NumStaticSamplers = 0;
        rootSignatureDesc.Desc_1_0.pStaticSamplers = nullptr;
        rootSignatureDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    }

    ID3DBlob* signatureBlob = nullptr;
    ID3DBlob* errorMsgs = nullptr;
    D3D_CHECK_RESULT(D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signatureBlob, &errorMsgs));
    
	ID3D12RootSignature* rootSignature;
    D3D_CHECK_RESULT(_device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
												 __uuidof(rootSignature), (void**)&(rootSignature)));
    GP_SAFE_RELEASE(signatureBlob);
    GP_SAFE_RELEASE(errorMsgs);
    GP_SAFE_FREE(ranges11);
    GP_SAFE_FREE(ranges10);
    GP_SAFE_FREE(parameters11);
    GP_SAFE_FREE(parameters10);

	// Create the pipeline state
	//

	// Input Elements
	uint32_t inputElementCount = 0;
    D3D12_INPUT_ELEMENT_DESC inputElements[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX];
    char semanticNames[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX][GP_GRAPHICS_SEMANTIC_NAME_MAX];

    size_t attribCount = GP_MATH_MIN(vertexLayout.getAttributeCount(), GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);
    for (uint32_t i = 0; i < vertexLayout.getAttributeCount(); i++)
	{
		VertexLayout::Attribute attribute = vertexLayout.getAttribute(i);
		GP_ASSERT(attribute.semantic != VertexLayout::SEMANTIC_UNDEFINED);
		/*TODO
        if (attribute.semanticName.length() > 0)
		{
            size_t semanticNameLenth = GP_MATH_MIN(GP_GRAPHICS_SEMANTIC_NAME_MAX, attribute.semanticName.length());
            strncpy_s(semanticNames[i], attribute.semanticName.c_str(), semanticNameLenth);
        }
        else 
		{
            char semanticName[GP_GRAPHICS_SEMANTIC_NAME_MAX];
            switch (attribute.semantic) 
			{
			case VertexLayout::SEMANTIC_POSITION: 
				sprintf_s(semanticName, "POSITION"); 
				break;
            case VertexLayout::SEMANTIC_NORMAL: 
				sprintf_s(semanticName, "NORMAL"); 
				break;
            case VertexLayout::SEMANTIC_COLOR: 
				sprintf_s(semanticName, "COLOR"); 
				break;
            case VertexLayout::SEMANTIC_TANGENT: 
				sprintf_s(semanticName, "TANGENT"); 
				break;
            case VertexLayout::SEMANTIC_BITANGENT: 
				sprintf_s(semanticName, "BITANGENT"); 
				break;
            case VertexLayout::SEMANTIC_TEXCOORD0: 
            case VertexLayout::SEMANTIC_TEXCOORD1: 
            case VertexLayout::SEMANTIC_TEXCOORD2: 
            case VertexLayout::SEMANTIC_TEXCOORD3: 
            case VertexLayout::SEMANTIC_TEXCOORD4: 
            case VertexLayout::SEMANTIC_TEXCOORD5:
			case VertexLayout::SEMANTIC_TEXCOORD6:
			case VertexLayout::SEMANTIC_TEXCOORD7:
				sprintf_s(semanticName, "TEXCOORD"); 
				break;
            default: 
				break;
            }
            GP_ASSERT(strlen(semanticName) != 0);
            strncpy_s(semanticNames[i], semanticName, strlen(semanticName));
        }
		*/
        UINT semanticIndex = 0;
        switch (attribute.semantic)
		{
        case VertexLayout::SEMANTIC_TEXCOORD0:
			semanticIndex = 0;
			break;
        case VertexLayout::SEMANTIC_TEXCOORD1:
			semanticIndex = 1;
			break;
        case VertexLayout::SEMANTIC_TEXCOORD2:
			semanticIndex = 2; 
			break;
        case VertexLayout::SEMANTIC_TEXCOORD3:
			semanticIndex = 3; 
			break;
        case VertexLayout::SEMANTIC_TEXCOORD4:
			semanticIndex = 4;
			break;
        case VertexLayout::SEMANTIC_TEXCOORD5:
			semanticIndex = 5;
			break;
        case VertexLayout::SEMANTIC_TEXCOORD6:
			semanticIndex = 6;
			break;
        case VertexLayout::SEMANTIC_TEXCOORD7:
			semanticIndex = 7; 
			break;
        default: 
			break;
        }

		D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
        inputElements[inputElementCount].SemanticName = semanticNames[i];
        inputElements[inputElementCount].SemanticIndex = semanticIndex;
        inputElements[inputElementCount].Format = lookupDXGI_FORMAT[attribute.format];
        inputElements[inputElementCount].InputSlot = 0;
        inputElements[inputElementCount].AlignedByteOffset = attribute.offset;
        inputElements[inputElementCount].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        inputElements[inputElementCount].InstanceDataStepRate = 0;
        ++inputElementCount;
    }

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
    inputLayoutDesc.pInputElementDescs = inputElements;
    inputLayoutDesc.NumElements = inputElementCount;

	D3D12_STREAM_OUTPUT_DESC streamOutputDesc = {};
    streamOutputDesc.pSODeclaration = nullptr;
    streamOutputDesc.NumEntries = 0;
    streamOutputDesc.pBufferStrides = nullptr;
    streamOutputDesc.NumStrides = 0;
    streamOutputDesc.RasterizedStream = 0;

	// RasterizerDesc
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = lookupD3D12_FILL_MODE[rasterizerState.fillMode];
    rasterizerDesc.CullMode = lookupD3D12_CULL_MODE[rasterizerState.cullMode];
    rasterizerDesc.FrontCounterClockwise = rasterizerState.frontFace == RasterizerState::FRONT_FACE_CCW ? TRUE : FALSE ;
    rasterizerDesc.DepthBias = rasterizerState.depthBias;
    rasterizerDesc.DepthBiasClamp = rasterizerState.depthBiasClamp;
    rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizerDesc.DepthClipEnable = rasterizerState.depthClipEnabled ? TRUE : FALSE;
    rasterizerDesc.MultisampleEnable = rasterizerState.multiSampleEnabled ? TRUE : FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// Color BlendDesc
	D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
        blendDesc.RenderTarget[i].BlendEnable = colorBlendState.blendEnabled;
        blendDesc.RenderTarget[i].LogicOpEnable = FALSE;
        blendDesc.RenderTarget[i].SrcBlend = lookupD3D12_BLEND[colorBlendState.colorBlendSrc];
        blendDesc.RenderTarget[i].DestBlend = lookupD3D12_BLEND[colorBlendState.colorBlendDst];
        blendDesc.RenderTarget[i].BlendOp = lookupD3D12_BLEND_OP[colorBlendState.colorBlendOp];
        blendDesc.RenderTarget[i].SrcBlendAlpha = lookupD3D12_BLEND[colorBlendState.alphaBlendSrc];
		blendDesc.RenderTarget[i].DestBlendAlpha = lookupD3D12_BLEND[colorBlendState.alphaBlendDst];
        blendDesc.RenderTarget[i].BlendOpAlpha = lookupD3D12_BLEND_OP[colorBlendState.alphaBlendOp];
        blendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
        blendDesc.RenderTarget[i].RenderTargetWriteMask = colorBlendState.colorWriteMask;
    }

	// DepthStencilDesc
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = depthStencilState.depthEnabled ? TRUE : FALSE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = lookupD3D12_COMPARISON_FUNC[depthStencilState.depthFunc];
    depthStencilDesc.StencilEnable = depthStencilState.stencilEnabled ? TRUE : FALSE;
    depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilFunc = lookupD3D12_COMPARISON_FUNC[depthStencilState.stencilOpStateFront.compareFunc];
	depthStencilDesc.FrontFace.StencilPassOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateFront.passOp];
	depthStencilDesc.FrontFace.StencilFailOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateFront.failOp];
	depthStencilDesc.FrontFace.StencilDepthFailOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateFront.depthFailOp];
	depthStencilDesc.BackFace.StencilFunc = lookupD3D12_COMPARISON_FUNC[depthStencilState.stencilOpStateBack.compareFunc];
	depthStencilDesc.BackFace.StencilPassOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateBack.passOp];
	depthStencilDesc.BackFace.StencilFailOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateBack.failOp];
	depthStencilDesc.BackFace.StencilDepthFailOp = lookupD3D12_STENCIL_OP[depthStencilState.stencilOpStateBack.depthFailOp];

	// Shaders
	D3D12_SHADER_BYTECODE VS = {};
	D3D12_SHADER_BYTECODE DS = {};
	D3D12_SHADER_BYTECODE HS = {};
	D3D12_SHADER_BYTECODE GS = {};
	D3D12_SHADER_BYTECODE PS = {};

    if (vertShader != nullptr) 
	{
		std::shared_ptr<ShaderD3D12> vertShaderD3D = std::static_pointer_cast<ShaderD3D12>(vertShader);
        VS.BytecodeLength = vertShaderD3D->_shader->GetBufferSize();
        VS.pShaderBytecode = vertShaderD3D->_shader->GetBufferPointer();
    }
    if (tescShader != nullptr)
	{
		std::shared_ptr<ShaderD3D12> tescShaderD3D = std::static_pointer_cast<ShaderD3D12>(tescShader);
        DS.BytecodeLength = tescShaderD3D->_shader->GetBufferSize();
        DS.pShaderBytecode = tescShaderD3D->_shader->GetBufferPointer();
    }
    if (teseShader != nullptr)
	{
        std::shared_ptr<ShaderD3D12> teseShaderD3D = std::static_pointer_cast<ShaderD3D12>(tescShader);
        HS.BytecodeLength = teseShaderD3D->_shader->GetBufferSize();
        HS.pShaderBytecode = teseShaderD3D->_shader->GetBufferPointer();
    }
    if (geomShader != nullptr)
	{
		std::shared_ptr<ShaderD3D12> geomShaderD3D = std::static_pointer_cast<ShaderD3D12>(tescShader);
        GS.BytecodeLength = geomShaderD3D->_shader->GetBufferSize();
        GS.pShaderBytecode = geomShaderD3D->_shader->GetBufferPointer();
    }
	if (fragShader != nullptr) 
	{
		std::shared_ptr<ShaderD3D12> fragShaderD3D = std::static_pointer_cast<ShaderD3D12>(tescShader);
        PS.BytecodeLength = fragShaderD3D->_shader->GetBufferSize();
        PS.pShaderBytecode = fragShaderD3D->_shader->GetBufferPointer();
    }

	// Color attachments
    uint32_t colorAttachmentCount = GP_MATH_MIN(renderPass->getColorAttachmentCount(), GP_GRAPHICS_COLOR_ATTACHMENTS_MAX);
    colorAttachmentCount = GP_MATH_MIN(colorAttachmentCount, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT);

	// Multisampling
	DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = renderPass->getSampleCount();
    sampleDesc.Quality = 0;

	D3D12_CACHED_PIPELINE_STATE cachedPipelineDesc = {};
    cachedPipelineDesc.pCachedBlob = nullptr;
    cachedPipelineDesc.CachedBlobSizeInBytes = 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
    pipelineStateDesc.pRootSignature = rootSignature;
    pipelineStateDesc.VS = VS;
    pipelineStateDesc.DS = DS;
    pipelineStateDesc.HS = HS;
    pipelineStateDesc.GS = GS;
	pipelineStateDesc.PS = PS;
    pipelineStateDesc.StreamOutput = streamOutputDesc;
    pipelineStateDesc.BlendState = blendDesc;
    pipelineStateDesc.SampleMask = UINT_MAX;
    pipelineStateDesc.RasterizerState = rasterizerDesc;
    pipelineStateDesc.DepthStencilState = depthStencilDesc;
    pipelineStateDesc.InputLayout = inputLayoutDesc;
    pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.NumRenderTargets = colorAttachmentCount;
    pipelineStateDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
    pipelineStateDesc.SampleDesc = sampleDesc;
    pipelineStateDesc.NodeMask = 0;
    pipelineStateDesc.CachedPSO = cachedPipelineDesc;
    pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    for (uint32_t i = 0; i < colorAttachmentCount; i++)
	{
        pipelineStateDesc.RTVFormats[i] = lookupDXGI_FORMAT[renderPass->getColorAttachment(i)->getPixelFormat()];
    }

	ID3D12PipelineState* pipelineState;
    D3D_CHECK_RESULT(_device->CreateGraphicsPipelineState(&pipelineStateDesc, __uuidof(pipelineState), (void**)&(pipelineState)));

	std::shared_ptr<RenderPipelineD3D12> renderPipelineD3D = std::make_shared<RenderPipelineD3D12>(primitiveTopology, vertexLayout, 
																								   rasterizerState,  colorBlendState, depthStencilState,
																								   renderPass, descriptorSet,
																								   vertShader, tescShader, teseShader, geomShader, fragShader, 
																								   _device, rootSignature, pipelineState);
	return std::static_pointer_cast<RenderPipeline>(renderPipelineD3D);
}

void GraphicsD3D12::destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline)
{
	GP_ASSERT(renderPipeline);
	std::shared_ptr<RenderPipelineD3D12> renderPipelineD3D = std::static_pointer_cast<RenderPipelineD3D12>(renderPipeline);
	GP_SAFE_RELEASE(renderPipelineD3D->_rootSignature);
	GP_SAFE_RELEASE(renderPipelineD3D->_pipelineState);
	renderPipeline.reset();
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
	if (true) //_validation)
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

	// Create render command queue
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;
	D3D_CHECK_RESULT(_device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_queue));
	

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
	for (size_t i = 0; i < displayModeCount; i++)
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
	swapchainDesc.BufferCount = GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT;
	swapchainDesc.Width = _width;
	swapchainDesc.Height = _height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.SampleDesc.Count = 1 + _multisampling;
	IDXGISwapChain1* swapchain;
	D3D_CHECK_RESULT(factory->CreateSwapChainForHwnd(_queue, _hwnd, &swapchainDesc, nullptr, nullptr, &swapchain));
	D3D_CHECK_RESULT(swapchain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&_swapchain));
	GP_SAFE_RELEASE(factory);

	// Create a render target description heap for the back buffers
	_swapchainImageIndex = _swapchain->GetCurrentBackBufferIndex();
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};

	// One extra for the intermediate render target
	rtvHeapDesc.NumDescriptors = GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	D3D_CHECK_RESULT(_device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_swapchainImagesViewHeap));

    // Create the swapchain image
    createSwapchainImages();

	 // Load root signature functions
    HMODULE module = ::GetModuleHandle(TEXT("d3d12.dll"));
    D3D12CreateRootSignatureDeserializer = (PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER)GetProcAddress(module, "D3D12SerializeVersionedRootSignature");
	GP_ASSERT(D3D12CreateRootSignatureDeserializer);

    D3D12SerializeVersionedRootSignature = (PFN_D3D12_SERIALIZE_VERSIONED_ROOT_SIGNATURE)GetProcAddress(module, "D3D12SerializeVersionedRootSignature");
	GP_ASSERT(D3D12SerializeVersionedRootSignature);

    D3D12CreateVersionedRootSignatureDeserializer = (PFN_D3D12_CREATE_VERSIONED_ROOT_SIGNATURE_DESERIALIZER)GetProcAddress(module,"D3D12CreateVersionedRootSignatureDeserializer");
    GP_ASSERT(D3D12CreateVersionedRootSignatureDeserializer);
    
	// Create the synchronization objects and command buffers
	 for (size_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	 {
		ID3D12CommandAllocator* commandAllocator = nullptr;
		D3D_CHECK_RESULT(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(commandAllocator), (void**)&(commandAllocator)));

		ID3D12PipelineState* pipelineState = nullptr;
		ID3D12GraphicsCommandList* commandList = nullptr;
		D3D_CHECK_RESULT(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState,
												    __uuidof(commandList), (void**)&(commandList)));
		commandList->Close();
		
		_commandBuffers.push_back(std::make_shared<CommandBufferD3D12>(_device, commandAllocator, commandList));
	 }

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
	
	// Wait for the gpu to finish processing on backbuffers before resizing them
	for (int i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; ++i)
	{
		//waitIdle(_fences[_backBufferCurrentIndex], _fenceValues[_backBufferCurrentIndex], _fenceEvents[_backBufferCurrentIndex]);
	}

    _resized = false;
	
    /* Release the backbuffers
	for (uint32_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	{
		GP_SAFE_RELEASE(_swapchainImages[i]);
		_fenceValues[i] = _fenceValues[_swapchainIndex];
	}
	*/

	// Resize the swap chain to the desired dimensions.
	DXGI_SWAP_CHAIN_DESC desc = {};
	_swapchain->GetDesc(&desc);
	D3D_CHECK_RESULT(_swapchain->ResizeBuffers(GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT, width, height, desc.BufferDesc.Format, desc.Flags));

	// Reset the frame index to the current back buffer index.
	_swapchainImageIndex = _swapchain->GetCurrentBackBufferIndex();

    // Create the swapchain
    createSwapchainImages();

	_width = width;
	_height = height;

    _resized = true;
}

bool GraphicsD3D12::isResized()
{
    return _resized;
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

void GraphicsD3D12::createSwapchainImages()
{
	std::vector<std::shared_ptr<Texture>> colorAttachments;
	std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments;	

	_swapchainImagesViewDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE swapchainImageViewHandle = _swapchainImagesViewHeap->GetCPUDescriptorHandleForHeapStart();
	
	for (uint32_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	{
		D3D_CHECK_RESULT(_swapchain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&_swapchainImages[i]));
		_device->CreateRenderTargetView(_swapchainImages[i], nullptr, swapchainImageViewHandle);
		swapchainImageViewHandle.ptr += _swapchainImagesViewDescriptorSize;

		std::shared_ptr<Texture> colorAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
																 Format::FORMAT_R8G8B8A8_UNORM, 
																 Texture::USAGE_COLOR_ATTACHMENT,
																 Texture::SAMPLE_COUNT_1X, false, nullptr,
																 _swapchainImages[i]);
		colorAttachments.push_back(colorAttachment);
	}
	std::shared_ptr<Texture> depthStencilAttachment = nullptr;
	depthStencilAttachment = createTexture(Texture::TYPE_2D, _width, _height, 1, 1,
										   Format::FORMAT_D24_UNORM_S8_UINT,
										   Texture::USAGE_DEPTH_STENCIL_ATTACHMENT, 
										   Texture::SAMPLE_COUNT_1X, false, nullptr,
										   nullptr);

	_swapchainImageIndex = _swapchain->GetCurrentBackBufferIndex();
	std::shared_ptr<RenderPass> renderPass = createRenderPass(_width, _height, 1, 
															  Format::FORMAT_R8G8B8A8_UNORM, 
															  Format::FORMAT_D24_UNORM_S8_UINT,
															  Texture::SAMPLE_COUNT_1X,
															  colorAttachments, 
															  colorMultisampleAttachments,
															  depthStencilAttachment);
}

}
