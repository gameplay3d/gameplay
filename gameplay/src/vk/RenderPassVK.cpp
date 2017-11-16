#include "Base.h"
#include "RenderPassVK.h"

namespace gameplay
{

RenderPassVK::RenderPassVK() :
	RenderPass(),
	_device(nullptr),
    _renderPass(nullptr),
    _framebuffer(nullptr)
{
}

RenderPassVK::RenderPassVK(size_t width, size_t height, 
						   size_t colorAttachmentCount, 
						   Format colorFormat,
						   Format depthStencilFormat, 
						   Texture::SampleCount sampleCount,
						   std::vector<std::shared_ptr<Texture>> colorAttachments,
						   std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
				           std::shared_ptr<Texture> depthStencilAttachment,
						   VkDevice device, 
                           VkRenderPass renderPass,
                           VkFramebuffer framebuffer) :
	RenderPass(width, height,
			   colorAttachmentCount,
			   colorFormat,
			   depthStencilFormat,
			   sampleCount,
			   colorAttachments,
			   colorMultisampleAttachments,
			   depthStencilAttachment),
	_device(device),
    _renderPass(renderPass),
    _framebuffer(framebuffer)
{
}

RenderPassVK::~RenderPassVK()
{
}

}