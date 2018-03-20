#pragma once

#include "Format.h"
#include "Texture.h"

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines a collection of attachments used for
 * the output of render pass operations in a
 * render pipeline.
 *
 * @see RenderPipeline
 */
class RenderPass
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    RenderPass();

    /**
     * Constructor.
     */
    RenderPass(size_t width, size_t height, 
               size_t colorAttachmentCount,
               Format colorFormat,
               Format depthStancilFormat,
               Texture::SampleCount sampleCount,
               std::vector<std::shared_ptr<Texture> > colorAttachments,
               std::vector<std::shared_ptr<Texture> > colorMultisampleAttachments,
               std::shared_ptr<Texture> depthStencilAttachment);

    /**
     * Destructor.
     */
    ~RenderPass();

    /**
     * Gets the width of the render pass and all attachments.
     *
     * @return The width of the render pass and all attachments.
     */
    size_t getWidth() const;

    /**
     * Gets the height of the render pass and all attachments.
     *
     * @return The height of the render pass and all attachments.
     */
    size_t getHeight() const;

    /**
     * Gets the number of color attachments.
     * 
     * @return The number of color attachments.
     */
    size_t getColorAttachmentCount() const;

    /**
     * Gets the formats of the color attachments.
     *
     * @return The formats of the color attachments.
     */
    Format getColorFormat(size_t index) const;

    /**
     * Gets the format for the depth stencil attachment.
     *
     * @return The format for the depth stencil attachment.
     */
    Format getDepthStencilFormat() const;

    /**
     * Gets the sampling count for the render pass.
     *
     * @return The sampling count for the render pass.
     */
    Texture::SampleCount getSampleCount() const;

    /**
     * Gets the color attachment as a texture as the specifed index.
     *
     * @return The color attachment as a texture as the specifed index.
     */
    std::shared_ptr<Texture> getColorAttachment(size_t index);

    /**
     * Gets the color multi-sample attachment as a texture as the specifed index.
     *
     * @return The color multi-sample attachment as a texture as the specifed index.
     */
    std::shared_ptr<Texture> getColorMultisampleAttachment(size_t index);

    /**
     * Gets the depth stencil attachment as a texture.
     *
     * @return The depth stencil attachment as a texture.
     */
    std::shared_ptr<Texture> getDepthStencilAttachment();

private:

    size_t _width;
    size_t _height;
    size_t _colorAttachmentCount;
    std::vector<Format> _colorFormats;
    Format _depthStencilFormat;
    Texture::SampleCount _sampleCount;
    std::vector<std::shared_ptr<Texture> > _colorAttachments;
    std::vector<std::shared_ptr<Texture> > _colorMultisampleAttachments;
    std::shared_ptr<Texture> _depthStencilAttachment;
    VkRenderPass _renderPass;
    VkFramebuffer _framebuffer;
};

}