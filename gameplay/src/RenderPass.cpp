#include "Base.h"
#include "RenderPass.h"

namespace gameplay
{

RenderPass::RenderPass() :
	_width(0), _height(0),
	_colorFormat(Format::FORMAT_UNDEFINED),
	_depthStencilFormat(Format::FORMAT_UNDEFINED),
	_sampleCount(Texture::SAMPLE_COUNT_1X)
{
}

RenderPass::RenderPass(size_t width, size_t height, 
					   size_t colorAttachmentCount, 
					   Format colorFormat, 
					   Format depthStancilFormat, 
					   Texture::SampleCount sampleCount,
					   std::vector<std::shared_ptr<Texture>> colorAttachments,
					   std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
			           std::shared_ptr<Texture> depthStencilAttachment) :
	_width(width), _height(height),
	_colorAttachmentCount(colorAttachmentCount),
	_colorFormat(colorFormat),
	_depthStencilFormat(depthStancilFormat),
	_sampleCount(sampleCount),
	_colorAttachments(colorAttachments),
	_colorMultisampleAttachments(colorMultisampleAttachments),
	_depthStencilAttachment(depthStencilAttachment)
{
}

RenderPass::~RenderPass()
{
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

Format RenderPass::getColorFormat() const
{
	return _colorFormat;
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