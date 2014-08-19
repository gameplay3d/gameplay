#include "Base.h"
#include "RenderTarget.h"

namespace gameplay
{

static std::vector<RenderTarget*> __renderTargets;

RenderTarget::RenderTarget(const char* id)
    : _id(id ? id : ""), _texture(NULL)
{
}

RenderTarget::~RenderTarget()
{
    SAFE_RELEASE(_texture);

    // Remove ourself from the cache.
    std::vector<RenderTarget*>::iterator it = std::find(__renderTargets.begin(), __renderTargets.end(), this);
    if (it != __renderTargets.end())
    {
        __renderTargets.erase(it);
    }
}

RenderTarget* RenderTarget::create(const char* id, unsigned int width, unsigned int height)
{
    // Create a new texture with the given width.
    Texture* texture = Texture::create(Texture::RGBA, width, height, NULL, false);
    if (texture == NULL)
    {
        GP_ERROR("Failed to create texture for render target.");
        return NULL;
    }

    RenderTarget* rt = create(id, texture);
    texture->release();

    return rt;
}

RenderTarget* RenderTarget::create(const char* id, Texture* texture)
{
    RenderTarget* renderTarget = new RenderTarget(id);
    renderTarget->_texture = texture;
    renderTarget->_texture->addRef();

    __renderTargets.push_back(renderTarget);

    return renderTarget;
}

RenderTarget* RenderTarget::getRenderTarget(const char* id)
{
    GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<RenderTarget*>::const_iterator it;
    for (it = __renderTargets.begin(); it < __renderTargets.end(); ++it)
    {
        RenderTarget* dst = *it;
        GP_ASSERT(dst);
        if (strcmp(id, dst->getId()) == 0)
        {
            return dst;
        }
    }

    return NULL;
}

const char* RenderTarget::getId() const
{
    return _id.c_str();
}

Texture* RenderTarget::getTexture() const
{
    return _texture;
}

unsigned int RenderTarget::getWidth() const
{
    return _texture->getWidth();
}

unsigned int RenderTarget::getHeight() const
{
    return _texture->getHeight();
}

}
