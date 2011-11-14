/**
 * DepthStencilTarget.cpp
 */

#include "Base.h"
#include "DepthStencilTarget.h"

namespace gameplay
{

static std::vector<DepthStencilTarget*> __depthStencilTargets;

DepthStencilTarget::DepthStencilTarget(const char* id, Format format)
    : _id(id), _format(format), _depthTexture(NULL), _stencilBuffer(0)
{
}

DepthStencilTarget::~DepthStencilTarget()
{
    SAFE_RELEASE(_depthTexture);

    // Destroy GL resources.
    if (_stencilBuffer)
    {
        GL_ASSERT( glDeleteTextures(1, &_stencilBuffer) );
    }

    // Remove from vector.
    std::vector<DepthStencilTarget*>::iterator it = std::find(__depthStencilTargets.begin(), __depthStencilTargets.end(), this);
    if (it != __depthStencilTargets.end())
    {
        __depthStencilTargets.erase(it);
    }
}

DepthStencilTarget* DepthStencilTarget::create(const char* id, Format format, unsigned int width, unsigned int height)
{
    // Create a backing texture buffer.
    Texture* depthTexture = Texture::create(Texture::DEPTH, width, height, NULL, false);
    if (depthTexture == NULL)
    {
        return NULL;
    }

    // Create stencil renderbuffer if format is DEPTH24_STENCIL8
    RenderBufferHandle stencilBuffer = 0;
    if (format == DEPTH24_STENCIL8)
    {
        // Backup the existing render buffer
        GLint currentRbo = 0;
        GL_ASSERT( glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRbo) );

        // Create the new render buffer
        GL_ASSERT( glGenRenderbuffers(1, &stencilBuffer) );
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer) );
        GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height) );

        // Restore the old render buffer
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, currentRbo) );
    }

    // Create the depth stencil target
    DepthStencilTarget* depthStencilTarget = new DepthStencilTarget(id, format);
    depthStencilTarget->_depthTexture = depthTexture;
    depthStencilTarget->_stencilBuffer = stencilBuffer;

    // Add it to the cache
    __depthStencilTargets.push_back(depthStencilTarget);

    return depthStencilTarget;
}

DepthStencilTarget* DepthStencilTarget::getDepthStencilTarget(const char* id)
{
    // Search the vector for a matching ID.
    std::vector<DepthStencilTarget*>::const_iterator it;
    for (it = __depthStencilTargets.begin(); it < __depthStencilTargets.end(); it++)
    {
        DepthStencilTarget* dst = *it;
        if (strcmp(id, dst->getID()) == 0)
        {
            return dst;
        }
    }

    return NULL;
}

const char* DepthStencilTarget::getID() const
{
    return _id.c_str();
}

DepthStencilTarget::Format DepthStencilTarget::getFormat() const
{
    return _format;
}

Texture* DepthStencilTarget::getTexture() const
{
    return _depthTexture;
}

}
