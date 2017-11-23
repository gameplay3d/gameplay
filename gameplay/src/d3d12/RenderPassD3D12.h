#pragma once

#include "RenderPass.h"
#include <d3d12.h>

namespace gameplay
{

class TextureD3D12;

/**
 * Direct3D 12 impl of a RenderPass.
 */
class RenderPassD3D12 : public RenderPass
{
public:

    /**
     * Constructor.
     */
	RenderPassD3D12();

    /**
     * Constructor.
     */
    RenderPassD3D12(size_t width, size_t height, 
					size_t colorAttachmentCount, 
					Format colorFormat,
					Format depthStencilFormat, 
					Texture::SampleCount sampleCount,
					std::vector<std::shared_ptr<Texture>> colorAttachments,
					std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
					std::shared_ptr<Texture> depthStencilAttachment,
					ID3D12Device* device, 
					ID3D12DescriptorHeap* renderTargetHeap, 
					ID3D12DescriptorHeap* depthStencilHeap);

    /**
	 * Destructor.
     */
    ~RenderPassD3D12();

	ID3D12Device* _device;
    ID3D12DescriptorHeap* _renderTargetHeap;
    ID3D12DescriptorHeap* _depthStencilHeap;
};

}