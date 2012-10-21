#include "Base.h"
#include "DepthStencilTarget.h"

namespace gameplay
{

static std::vector<DepthStencilTarget*> __depthStencilTargets;

DepthStencilTarget::DepthStencilTarget(const char* id, Format format, unsigned int width, unsigned int height)
    : _id(id ? id : ""), _format(format), _renderBuffer(0), _width(width), _height(height)
{
}

DepthStencilTarget::~DepthStencilTarget()
{
    // Destroy GL resources.
    if (_renderBuffer)
        GL_ASSERT( glDeleteTextures(1, &_renderBuffer) );

    // Remove from vector.
    std::vector<DepthStencilTarget*>::iterator it = std::find(__depthStencilTargets.begin(), __depthStencilTargets.end(), this);
    if (it != __depthStencilTargets.end())
    {
        __depthStencilTargets.erase(it);
    }
}

DepthStencilTarget* DepthStencilTarget::create(const char* id, Format format, unsigned int width, unsigned int height)
{
    // Create the depth stencil target.
    DepthStencilTarget* depthStencilTarget = new DepthStencilTarget(id, format, width, height);

    // Create a render buffer for this new depth stencil target
    GL_ASSERT( glGenRenderbuffers(1, &depthStencilTarget->_renderBuffer) );
    GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, depthStencilTarget->_renderBuffer) );
    GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height) );

    // Add it to the cache.
    __depthStencilTargets.push_back(depthStencilTarget);

    return depthStencilTarget;
}

DepthStencilTarget* DepthStencilTarget::getDepthStencilTarget(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<DepthStencilTarget*>::const_iterator it;
    for (it = __depthStencilTargets.begin(); it < __depthStencilTargets.end(); it++)
    {
        DepthStencilTarget* dst = *it;
        GP_ASSERT(dst);
        if (strcmp(id, dst->getId()) == 0)
        {
            return dst;
        }
    }
    return NULL;
}

const char* DepthStencilTarget::getId() const
{
    return _id.c_str();
}

DepthStencilTarget::Format DepthStencilTarget::getFormat() const
{
    return _format;
}

unsigned int DepthStencilTarget::getWidth() const
{
    return _width;
}

unsigned int DepthStencilTarget::getHeight() const
{
    return _height;
}

}
