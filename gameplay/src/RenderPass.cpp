#include "Base.h"
#include "RenderPass.h"

namespace gameplay
{

RenderPass::RenderPass() :
    _width(0),
    _height(0),
    _colorAttachmentCount(0),
    _depthStencilFormat(Format::eUndefined),
    _sampleCount(Texture::SampleCount::e1X),
    _renderPass(VK_NULL_HANDLE),
    _framebuffer(VK_NULL_HANDLE)
{
}

RenderPass::~RenderPass()
{
    _colorFormats.clear();
    _colorAttachments.clear();
}

size_t RenderPass::getWidth() const
{
    return _width;
}

size_t RenderPass::getHeight() const
{
    return _height;
}

size_t RenderPass::getColorAttachmentCount() const
{
    return _colorAttachmentCount;
}

Format RenderPass::getColorFormat(size_t index) const
{
    return _colorFormats[index];
}

Format RenderPass::getDepthStencilFormat() const
{
    return _depthStencilFormat;
}

Texture::SampleCount RenderPass::getSampleCount() const
{
    return _sampleCount;
}

std::shared_ptr<Texture> RenderPass::getColorAttachment(size_t index)
{
    return _colorAttachments[index];
}

std::shared_ptr<Texture> RenderPass::getColorMultisampleAttachment(size_t index)
{
    return _colorMultisampleAttachments[index];
}

std::shared_ptr<Texture> RenderPass::getDepthStencilAttachment()
{
    return _depthStencilAttachment;
}

}
