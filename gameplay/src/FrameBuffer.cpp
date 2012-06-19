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
            SAFE_RELEASE(_renderTargets[i]);
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
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    __defaultHandle = (FrameBufferHandle)fbo;
}

FrameBuffer* FrameBuffer::create(const char* id, unsigned int width, unsigned int height)
{
    if (!isPowerOfTwo(width) | !isPowerOfTwo(height))
    {
        GP_ERROR("Failed to create render target for frame buffer. Width and Height must be a power of 2.");
        return NULL;
    }

    // Call getMaxRenderTargets() to force __maxRenderTargets to be set
    getMaxRenderTargets();

    // Create RenderTarget with same ID.
    RenderTarget* renderTarget = NULL;
    renderTarget = RenderTarget::create(id, width, height);
    if (renderTarget == NULL)
    {
        GP_ERROR("Failed to create render target for frame buffer.");
        return NULL;
    }

    // Create the frame buffer
    GLuint handle = 0;
    GL_ASSERT( glGenFramebuffers(1, &handle) );
    FrameBuffer* frameBuffer = new FrameBuffer(id, width, height);
    frameBuffer->_handle = handle;
    
    // Create the render target array for the new frame buffer
    RenderTarget** renderTargets = new RenderTarget*[__maxRenderTargets];
    memset(renderTargets, 0, sizeof(RenderTarget*) * __maxRenderTargets);
    frameBuffer->_renderTargets = renderTargets;

    frameBuffer->setRenderTarget(renderTarget, 0);

    __frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

FrameBuffer* FrameBuffer::getFrameBuffer(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<FrameBuffer*>::const_iterator it;
    for (it = __frameBuffers.begin(); it < __frameBuffers.end(); it++)
    {
        FrameBuffer* fb = *it;
        GP_ASSERT(fb);
        if (strcmp(id, fb->getID()) == 0)
        {
            return fb;
        }
    }
    return NULL;
}

const char* FrameBuffer::getID() const
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
    if (__maxRenderTargets == 0)
    {
#ifdef GL_MAX_COLOR_ATTACHMENTS
        GLint val;
        GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
        __maxRenderTargets = (unsigned int) val;
#else
        __maxRenderTargets = 1;
#endif
    }
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
        // This FrameBuffer now references the RenderTarget.
        target->addRef();

        // Store the current FBO binding so we can restore it
        GLint currentFbo;
        GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo) );

        // Now set this target as the color attachment corresponding to index.
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _renderTargets[index]->getTexture()->getHandle()) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
        GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL) );
        GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
        GP_ASSERT( _renderTargets[index]->getTexture() );
        GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _renderTargets[index]->getTexture()->getHandle(), 0) );
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incompleted: 0x%x", fboStatus);
        }
        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, currentFbo) );
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

        // Store the current FBO binding so we can restore it.
        GLint currentFbo;
        GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo) );

        // Now set this target as the color attachment corresponding to index.
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );
        
        // Create a render buffer 
        RenderBufferHandle renderBuffer = 0;
        GL_ASSERT( glGenRenderbuffers(1, &renderBuffer) );
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer) );
        GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height) );
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer) );
        // Attach the 
        if (target->getFormat() == DepthStencilTarget::DEPTH_STENCIL)
        {
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer) );
        }

        // Check the framebuffer is good to go.
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            GP_ERROR("Framebuffer status incompleted: 0x%x", fboStatus);
        }
        _depthStencilTarget->_renderBuffer = renderBuffer;


        // Restore the FBO binding
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, currentFbo) );
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
}

void FrameBuffer::bindDefault()
{
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, __defaultHandle) );
}

bool FrameBuffer::isPowerOfTwo(unsigned int value)
{
    return (value != 0) && ((value & (value - 1)) == 0);
}

}
