/**
 * FrameBuffer.cpp
 */

#include "Base.h"
#include "FrameBuffer.h"

namespace gameplay
{

static unsigned int __maxRenderTargets = 0;
static std::vector<FrameBuffer*> __frameBuffers;
static FrameBufferHandle __defaultHandle = 0;
static FrameBufferHandle __currentHandle = 0;

FrameBuffer::FrameBuffer(const char* id, unsigned int width, unsigned int height) :
    _id(id ? id : ""), _width(width), _height(height), _handle(0), 
    _renderTargets(NULL), _depthStencilTarget(NULL)
{
}

FrameBuffer::~FrameBuffer()
{
    if (_renderTargets)
    {
        for (unsigned int i = 0; i < __maxRenderTargets; ++i)
        {
            if (_renderTargets[i])
            {
                SAFE_RELEASE(_renderTargets[i]);
            }
        }
        SAFE_DELETE_ARRAY(_renderTargets);
    }
    if (_depthStencilTarget)
    {
        SAFE_RELEASE(_depthStencilTarget);
    }

    // Release GL resource.
    if (_handle)
    {
        GL_ASSERT( glDeleteFramebuffers(1, &_handle) );
    }

    // Remove self from vector.
    std::vector<FrameBuffer*>::iterator it = std::find(__frameBuffers.begin(), __frameBuffers.end(), this);
    if (it != __frameBuffers.end())
    {
        __frameBuffers.erase(it);
    }
}

void FrameBuffer::initialize()
{
    // Query the current/initial FBO handle and store is as out 'default' frame buffer.
    // On many platforms this will simply be the zero (0) handle, but this is not always the case.
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    __defaultHandle = (FrameBufferHandle)fbo;
    __currentHandle = __defaultHandle;

    // Query the max supported color attachments. This glGet operation is not supported
    // on GL ES 2.x, so if the define does not exist, assume a value of 1.
#ifdef GL_MAX_COLOR_ATTACHMENTS
        GLint val;
        GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
        __maxRenderTargets = (unsigned int)std::max(1, val);
#else
        __maxRenderTargets = 1;
#endif
}

FrameBuffer* FrameBuffer::create(const char* id)
{
    return create(id, 0, 0);
}

FrameBuffer* FrameBuffer::create(const char* id, unsigned int width, unsigned int height)
{
    // Call getMaxRenderTargets() to force __maxRenderTargets to be set
    getMaxRenderTargets();

    RenderTarget* renderTarget = NULL;
    if (width > 0 && height > 0)
    {
        // Create a default RenderTarget with same ID.
        renderTarget = RenderTarget::create(id, width, height);
        if (renderTarget == NULL)
        {
            GP_ERROR("Failed to create render target for frame buffer.");
            return NULL;
        }
    }

    // Create the frame buffer
    GLuint handle = 0;
    GL_ASSERT( glGenFramebuffers(1, &handle) );
    FrameBuffer* frameBuffer = new FrameBuffer(id, width, height);
    frameBuffer->_handle = handle;
    
    // Create the render target array for the new frame buffer
    frameBuffer->_renderTargets = new RenderTarget*[__maxRenderTargets];
    memset(frameBuffer->_renderTargets, 0, sizeof(RenderTarget*) * __maxRenderTargets);

    if (renderTarget)
    {
        frameBuffer->setRenderTarget(renderTarget, 0);
        SAFE_RELEASE(renderTarget);
    }

    __frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

FrameBuffer* FrameBuffer::getFrameBuffer(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<FrameBuffer*>::const_iterator it;
    for (it = __frameBuffers.begin(); it < __frameBuffers.end(); ++it)
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
    return _width;
}

unsigned int FrameBuffer::getHeight() const
{
    return _height;
}

unsigned int FrameBuffer::getMaxRenderTargets()
{
    return __maxRenderTargets;
}

void FrameBuffer::setRenderTarget(RenderTarget* target, unsigned int index)
{
    GP_ASSERT(index < __maxRenderTargets);
    GP_ASSERT(_renderTargets);

    if (_renderTargets[index] == target)
    {
        // No change.
        return;
    }

    // Release our reference to the current RenderTarget at this index.
    SAFE_RELEASE(_renderTargets[index]);

    _renderTargets[index] = target;

    if (target)
    {
        GP_ASSERT( _renderTargets[index]->getTexture() );

        // This FrameBuffer now references the RenderTarget.
        target->addRef();

        // Now set this target as the color attachment corresponding to index.
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );
        GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
        GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _renderTargets[index]->getTexture()->getHandle(), 0) );
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
        }

        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, __currentHandle) );
    }
}

RenderTarget* FrameBuffer::getRenderTarget(unsigned int index) const
{
    GP_ASSERT(_renderTargets);
    if (index < __maxRenderTargets)
    {
        return _renderTargets[index];
    }

    return NULL;
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
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_renderBuffer) );
        if (target->getFormat() == DepthStencilTarget::DEPTH_STENCIL)
        {
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_renderBuffer) );
        }

        // Check the framebuffer is good to go.
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
        }

        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, __currentHandle) );
    }
}

DepthStencilTarget* FrameBuffer::getDepthStencilTarget() const
{
    return _depthStencilTarget;
}

void FrameBuffer::bind()
{
    // Bind this FrameBuffer for rendering.
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );

    // Update the current FBO handle
    __currentHandle = _handle;
}

void FrameBuffer::bindDefault()
{
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, __defaultHandle) );

    // Update the current FBO handle
    __currentHandle = __defaultHandle;
}


}
