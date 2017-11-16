#include "Base.h"
#include "RenderPassD3D12.h"

namespace gameplay
{

RenderPassD3D12::RenderPassD3D12() :
	RenderPass(),
	_device(nullptr),
	_renderTargetHeap(nullptr),
    _depthStencilHeap(nullptr)
{
}

RenderPassD3D12::RenderPassD3D12(size_t width, size_t height, 
								 size_t colorAttachmentCount, 
								 Format colorFormat,
								 Format depthStencilFormat, 
								 Texture::SampleCount sampleCount,
								 std::vector<std::shared_ptr<Texture>> colorAttachments,
								 std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
								 std::shared_ptr<Texture> depthStencilAttachment,
								 ID3D12Device* device, 
								 ID3D12DescriptorHeap* renderTargetHeap, 
								 ID3D12DescriptorHeap* depthStencilHeap) :
	RenderPass(width, height,
			   colorAttachmentCount, 
			   colorFormat, 
			   depthStencilFormat, 
			   sampleCount,
			   colorAttachments,
			   colorMultisampleAttachments,
			   depthStencilAttachment),
    _device(device),
	_renderTargetHeap(renderTargetHeap),
	_depthStencilHeap(depthStencilHeap)
{
}

RenderPassD3D12::~RenderPassD3D12()
{
}

}