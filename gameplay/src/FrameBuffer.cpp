#include "Base.h"
#include "FrameBuffer.h"
#include "Game.h"

#define FRAMEBUFFER_ID_DEFAULT "org.gameplay3d.framebuffer.default"

namespace gameplay
{

unsigned int FrameBuffer::_maxRenderTargets = 0;
std::vector<FrameBuffer*> FrameBuffer::_frameBuffers;
FrameBuffer* FrameBuffer::_defaultFrameBuffer = NULL;
FrameBuffer* FrameBuffer::_currentFrameBuffer = NULL;

FrameBuffer::FrameBuffer(const char* id, unsigned int width, unsigned int height, FrameBufferHandle handle) 
    : _id(id ? id : ""), _handle(handle), _renderTargets(NULL), _renderTargetCount(0), _depthStencilTarget(NULL)
{
}

FrameBuffer::~FrameBuffer()
{
    if (_renderTargets)
    {
        for (unsigned int i = 0; i < _maxRenderTargets; ++i)
        {
            if (_renderTargets[i])
            {
                SAFE_RELEASE(_renderTargets[i]);
            }
        }
        SAFE_DELETE_ARRAY(_renderTargets);
    }

    if (_depthStencilTarget)
        SAFE_RELEASE(_depthStencilTarget);

    // Release GL resource.
    if (_handle)
        GL_ASSERT( glDeleteFramebuffers(1, &_handle) );

    // Remove self from vector.
    std::vector<FrameBuffer*>::iterator it = std::find(_frameBuffers.begin(), _frameBuffers.end(), this);
    if (it != _frameBuffers.end())
    {
        _frameBuffers.erase(it);
    }
}

void FrameBuffer::initialize()
{
    // Query the current/initial FBO handle and store is as out 'default' frame buffer.
    // On many platforms this will simply be the zero (0) handle, but this is not always the case.
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    _defaultFrameBuffer = new FrameBuffer(FRAMEBUFFER_ID_DEFAULT, 0, 0, (FrameBufferHandle)fbo);
    _currentFrameBuffer = _defaultFrameBuffer;

    // Query the max supported color attachments. This glGet operation is not supported
    // on GL ES 2.x, so if the define does not exist, assume a value of 1.
#ifdef GL_MAX_COLOR_ATTACHMENTS
        GLint val;
        GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
        _maxRenderTargets = (unsigned int)std::max(1, val);
#else
        _maxRenderTargets = 1;
#endif
}

void FrameBuffer::finalize()
{
    SAFE_RELEASE(_defaultFrameBuffer);
}

FrameBuffer* FrameBuffer::create(const char* id)
{
    return create(id, 0, 0);
}

FrameBuffer* FrameBuffer::create(const char* id, unsigned int width, unsigned int height, Texture::Format format)
{
    RenderTarget* renderTarget = NULL;
    if (width > 0 && height > 0)
    {
        // Create a default RenderTarget with same ID.
        renderTarget = RenderTarget::create(id, width, height, format);
        if (renderTarget == NULL)
        {
            GP_ERROR("Failed to create render target for frame buffer.");
            return NULL;
        }
    }

    // Create the frame buffer
    GLuint handle = 0;
    GL_ASSERT( glGenFramebuffers(1, &handle) );
    FrameBuffer* frameBuffer = new FrameBuffer(id, width, height, handle);
    
    // Create the render target array for the new frame buffer
    frameBuffer->_renderTargets = new RenderTarget*[_maxRenderTargets];
    memset(frameBuffer->_renderTargets, 0, sizeof(RenderTarget*) * _maxRenderTargets);

    if (renderTarget)
    {
        frameBuffer->setRenderTarget(renderTarget, 0);
        SAFE_RELEASE(renderTarget);
    }
    _frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

FrameBuffer* FrameBuffer::getFrameBuffer(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<FrameBuffer*>::const_iterator it;
    for (it = _frameBuffers.begin(); it < _frameBuffers.end(); ++it)
    {
        FrameBuffer* fb = *it;
        GP_ASSERT(fb);
        if (strcmp(id, fb->getId()) == 0)
        {
            return fb;
        }
    }
    return NULL;
}

const char* FrameBuffer::getId() const
{
    return _id.c_str();
}

unsigned int FrameBuffer::getWidth() const
{
    if (_renderTargetCount > 0 && _renderTargets != NULL && _renderTargets[0] != NULL)
        return _renderTargets[0]->getWidth();

    return 0;
}

unsigned int FrameBuffer::getHeight() const
{
    if (_renderTargetCount > 0 && _renderTargets != NULL && _renderTargets[0] != NULL)
        return _renderTargets[0]->getHeight();

    return 0;
}

unsigned int FrameBuffer::getMaxRenderTargets()
{
    return _maxRenderTargets;
}

void FrameBuffer::setRenderTarget(RenderTarget* target, unsigned int index)
{
    GP_ASSERT(!target || (target->getTexture() && target->getTexture()->getType() == Texture::TEXTURE_2D));

    // No change
    if (_renderTargets[index] == target)
        return;

    setRenderTarget(target, index, GL_TEXTURE_2D);
}

void FrameBuffer::setRenderTarget(RenderTarget* target, Texture::CubeFace face, unsigned int index)
{
    GP_ASSERT(face >= Texture::POSITIVE_X && face <= Texture::NEGATIVE_Z);
    GP_ASSERT(!target || (target->getTexture() && target->getTexture()->getType() == Texture::TEXTURE_CUBE));

    setRenderTarget(target, index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
}

void FrameBuffer::setRenderTarget(RenderTarget* target, unsigned int index, GLenum textureTarget)
{
    GP_ASSERT(index < _maxRenderTargets);
    GP_ASSERT(_renderTargets);

    // Release our reference to the current RenderTarget at this index.
    if (_renderTargets[index])
    {
        SAFE_RELEASE(_renderTargets[index]);
        --_renderTargetCount;
    }

    _renderTargets[index] = target;

    if (target)
    {
        ++_renderTargetCount;

        // This FrameBuffer now references the RenderTarget.
        target->addRef();

        // Now set this target as the color attachment corresponding to index.
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );
        GLenum attachment;
        if (target->getTexture()->getFormat() == Texture::DEPTH)
        {
            attachment = GL_DEPTH_ATTACHMENT;
            GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, _renderTargets[index]->getTexture()->getHandle(), 0));
#ifndef OPENGL_ES            
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
#elif defined(GL_ES_VERSION_3_0) && GL_ES_VERSION_3_0
            glDrawBuffers(0, NULL);
#endif
        }
        else
        {
            attachment = GL_COLOR_ATTACHMENT0 + index;
            GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, _renderTargets[index]->getTexture()->getHandle(), 0) );
        }

        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
        }

        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _currentFrameBuffer->_handle) );
    }
}

RenderTarget* FrameBuffer::getRenderTarget(unsigned int index) const
{
    GP_ASSERT(_renderTargets);
    if (index < _maxRenderTargets)
    {
        return _renderTargets[index];
    }
    return NULL;
}

unsigned int FrameBuffer::getRenderTargetCount() const
{
    return _renderTargetCount;
}

void FrameBuffer::setDepthStencilTarget(DepthStencilTarget* target)
{
    if (_depthStencilTarget == target)
        return;

    // Release our existing depth stencil target.
    SAFE_RELEASE(_depthStencilTarget);

    _depthStencilTarget = target;

    if (target)
    {
        // The FrameBuffer now owns this DepthStencilTarget.
        target->addRef();

        // Now set this target as the color attachment corresponding to index.
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );

        // Attach the render buffer to the framebuffer
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_depthBuffer) );
        if (target->isPacked())
        {
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_depthBuffer) );
        }
        else if (target->getFormat() == DepthStencilTarget::DEPTH_STENCIL)
        {
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_stencilBuffer) );
        }

        // Check the framebuffer is good to go.
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
        }

        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _currentFrameBuffer->_handle) );
    }
}

DepthStencilTarget* FrameBuffer::getDepthStencilTarget() const
{
    return _depthStencilTarget;
}

bool FrameBuffer::isDefault() const
{
    return (this == _defaultFrameBuffer);
}

FrameBuffer* FrameBuffer::bind(GLenum type)
{
    GL_ASSERT( glBindFramebuffer(type, _handle) );
    FrameBuffer* previousFrameBuffer = _currentFrameBuffer;
    _currentFrameBuffer = this;
    return previousFrameBuffer;
}

void FrameBuffer::getScreenshot(Image* image)
{
    GP_ASSERT( image );

    unsigned int width = _currentFrameBuffer->getWidth();
    unsigned int height = _currentFrameBuffer->getHeight();

	if (image->getWidth() == width && image->getHeight() == height) {
		GLenum format = image->getFormat() == Image::RGB ? GL_RGB : GL_RGBA;
        GL_ASSERT( glReadPixels(0, 0, width, height, format, GL_UNSIGNED_BYTE, image->getData()) );
	}
}

Image* FrameBuffer::createScreenshot(Image::Format format)
{
    Image* screenshot = Image::create(_currentFrameBuffer->getWidth(), _currentFrameBuffer->getHeight(), format, NULL);
    getScreenshot(screenshot);

    return screenshot;
}

FrameBuffer* FrameBuffer::bindDefault(GLenum type)
{
    GL_ASSERT( glBindFramebuffer(type, _defaultFrameBuffer->_handle) );
    _currentFrameBuffer = _defaultFrameBuffer;
    return _defaultFrameBuffer;
}

FrameBuffer* FrameBuffer::getCurrent()
{
    return _currentFrameBuffer;
}

}
