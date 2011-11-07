#include "DepthStencilTarget.h"

namespace gameplay
{

static std::vector<DepthStencilTarget*> __depthStencilTargets;

DepthStencilTarget* DepthStencilTarget::create(const char* id, unsigned int width, unsigned int height)
{
    DepthStencilTarget* depthStencilTarget = new DepthStencilTarget(id, width, height);

    __depthStencilTargets.push_back(depthStencilTarget);
    depthStencilTarget->_index = __depthStencilTargets.size() - 1;

    return depthStencilTarget;
}

DepthStencilTarget* DepthStencilTarget::getDepthStencilTarget(const char* id)
{
    // Search the vector for a matching ID.
    std::vector<DepthStencilTarget*>::const_iterator it;
    for (it = __depthStencilTargets.begin(); it < __depthStencilTargets.end(); ++it)
    {
        DepthStencilTarget* dst = *it;
        if (strcmp(id, dst->getID()) == 0)
        {
            return dst;
        }
    }

    return NULL;
}

DepthStencilTarget::~DepthStencilTarget()
{
    // Destroy GL resources.
    glDeleteTextures(1, &_handle);

    // Remove from vector.
    std::vector<DepthStencilTarget*>::const_iterator it = __depthStencilTargets.begin() + _index;
    __depthStencilTargets.erase(it);
}

const char* DepthStencilTarget::getID() const
{
    return _id.c_str();
}

DepthStencilTarget::DepthStencilTarget(const char* id, unsigned int width, unsigned int height)
{
    // Create a backing texture / renderbuffer.
    // Need to experiment to determine whether GL_DEPTH_STENCIL textures are supported.
    // If not, need to use dual renderbuffers instead.
    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
}

}