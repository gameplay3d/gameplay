#pragma once

#include "RenderPass.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

class TextureVK;

/**
 * Vulkan impl of a RenderPass.
 */
class RenderPassVK : public RenderPass
{
public:

    /**
     * Constructor.
     */
	RenderPassVK();

    /**
     * Constructor.
     */
    RenderPassVK(size_t width, size_t height, 
				 size_t colorAttachmentCount, 
				 Format colorFormat,
				 Format depthStencilFormat, 
				 Texture::SampleCount sampleCount,
				 std::vector<std::shared_ptr<Texture>> colorAttachments,
				 std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
				 std::shared_ptr<Texture> depthStencilAttachment,
				 VkDevice device, 
                 VkRenderPass renderPass,
                 VkFramebuffer framebuffer);

    /**
	 * Destructor.
     */
    ~RenderPassVK();

	VkDevice _device;
    VkRenderPass _renderPass;
    VkFramebuffer _framebuffer;
};

}