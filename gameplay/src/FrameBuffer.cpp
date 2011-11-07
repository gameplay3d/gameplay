#include "Base.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "DepthStencilTarget.h"
#include "Texture.h"

namespace gameplay
{

static unsigned int __maxRenderTargets = 0;
static std::vector<FrameBuffer*> __frameBuffers;

FrameBuffer* FrameBuffer::create(const char* id)
{
    FrameBuffer* frameBuffer = new FrameBuffer(id);

    __frameBuffers.push_back(frameBuffer);
    frameBuffer->_index = __frameBuffers.size() - 1;

    return frameBuffer;
}

FrameBuffer* FrameBuffer::create(const char* id, unsigned int width, unsigned int height)
{
    FrameBuffer* frameBuffer = new FrameBuffer(id);

    // Create RenderTarget with same ID and set as first color attachment.
    RenderTarget* renderTarget = RenderTarget::create(id, width, height);
    frameBuffer->setRenderTarget(renderTarget);
    SAFE_RELEASE(renderTarget);

    __frameBuffers.push_back(frameBuffer);
    frameBuffer->_index = __frameBuffers.size() - 1;

    return frameBuffer;
}

FrameBuffer* FrameBuffer::getFrameBuffer(const char* id)
{
    // Search the vector for a matching ID.
    std::vector<FrameBuffer*>::const_iterator it;
    for (it = __frameBuffers.begin(); it < __frameBuffers.end(); ++it)
    {
        FrameBuffer* fb = *it;
        if (strcmp(id, fb->getID()) == 0)
        {
            return fb;
        }
    }

    return NULL;
}

FrameBuffer::~FrameBuffer()
{
    if (__renderTargets)
    {
        for (unsigned int i = 0; i < __maxRenderTargets; ++i)
        {
            SAFE_RELEASE(__renderTargets[i]);
        }
        SAFE_DELETE_ARRAY(__renderTargets);
    }

    // Release GL resource.
    glDeleteFramebuffers(1, &_handle);

    // Remove self from vector.
    std::vector<FrameBuffer*>::const_iterator it = __frameBuffers.begin() + _index;
    __frameBuffers.erase(it);
}

const char* FrameBuffer::getID() const
{
    return _id.c_str();
}

unsigned int FrameBuffer::getMaxRenderTargets()
{
    if (__maxRenderTargets == 0)
    {
        GLint val;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val);
        __maxRenderTargets = (unsigned int) val;
    }

    return __maxRenderTargets;
}

void FrameBuffer::setRenderTarget(RenderTarget* target, unsigned int index)
{
    assert(index < __maxRenderTargets);

    if (__renderTargets[index] == target)
    {
        return;
    }

    // Release our reference to the current RenderTarget at this index.
    SAFE_RELEASE(__renderTargets[index]);

    __renderTargets[index] = target;
        
    // This FrameBuffer now references the RenderTarget.
    target->addRef();

    // Now set this target as the color attachment corresponding to index.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
    GLenum attachment;
    switch (index)
    {
        case 0:
            attachment = GL_COLOR_ATTACHMENT0;
            break;
        case 1:
            attachment = GL_COLOR_ATTACHMENT1;
            break;
        case 2:
            attachment = GL_COLOR_ATTACHMENT2;
            break;
        case 3:
            attachment = GL_COLOR_ATTACHMENT3;
            break;
        case 4:
            attachment = GL_COLOR_ATTACHMENT4;
            break;
        case 5:
            attachment = GL_COLOR_ATTACHMENT5;
            break;
        case 6:
            attachment = GL_COLOR_ATTACHMENT6;
            break;
        case 7:
            attachment = GL_COLOR_ATTACHMENT7;
            break;
        case 8:
            attachment = GL_COLOR_ATTACHMENT8;
            break;
        case 9:
            attachment = GL_COLOR_ATTACHMENT9;
            break;
        case 10:
            attachment = GL_COLOR_ATTACHMENT10;
            break;
        case 11:
            attachment = GL_COLOR_ATTACHMENT11;
            break;
        case 12:
            attachment = GL_COLOR_ATTACHMENT12;
            break;
        case 13:
            attachment = GL_COLOR_ATTACHMENT13;
            break;
        case 14:
            attachment = GL_COLOR_ATTACHMENT14;
            break;
        case 15:
            attachment = GL_COLOR_ATTACHMENT15;
            break;
    }
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, __renderTargets[index]->getTexture()->getHandle(), 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

RenderTarget* FrameBuffer::getRenderTarget(unsigned int index) const
{
    if (index < __maxRenderTargets)
    {
        return __renderTargets[index];
    }

    return NULL;
}

void FrameBuffer::setDepthStencilTarget(DepthStencilTarget* target)
{
    SAFE_RELEASE(_depthStencilTarget);
    _depthStencilTarget = target;
    target->addRef();
}

DepthStencilTarget* FrameBuffer::getDepthStencilTarget() const
{
    return _depthStencilTarget;
}

void FrameBuffer::bind()
{
    // Check framebuffer completeness.
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE:
            // Success.
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            // Configuration error.
            LOG_ERROR("FrameBuffer unsupported.");
            return;

        default:
            // Programming error; will fail on all hardware.
            LOG_ERROR("Unknown error checking FrameBuffer status.");
            return;
    }

    // Bind this FrameBuffer for rendering.
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
}

void FrameBuffer::bindDefault()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(const char* id) :
    _handle(0), _depthStencilTarget(NULL), _index(0)
{
    if (id)
    {
        _id = id;
    }

    // Create GL FBO resource.
    glGenFramebuffers(1, &_handle);

    // Query MAX_COLOR_ATTACHMENTS the first time a FrameBuffer is created.
    if (__maxRenderTargets == 0)
    {
        GLint val;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val);
        __maxRenderTargets = (unsigned int) val;
    }

    __renderTargets = new RenderTarget*[__maxRenderTargets];
    memset(__renderTargets, 0, sizeof(RenderTarget*) * __maxRenderTargets);
}

}