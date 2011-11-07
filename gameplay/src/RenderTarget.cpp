#include "Base.h"
#include "RenderTarget.h"
#include "Texture.h"

namespace gameplay
{

static std::vector<RenderTarget*> __renderTargets;

RenderTarget* RenderTarget::create(const char* id, unsigned int width, unsigned int height)
{
    RenderTarget* renderTarget = new RenderTarget(id, width, height);

    __renderTargets.push_back(renderTarget);
    renderTarget->_index = __renderTargets.size() - 1;

    return renderTarget;
}

RenderTarget* RenderTarget::getRenderTarget(const char* id)
{
    // Search the vector for a matching ID.
    std::vector<RenderTarget*>::const_iterator it;
    for (it = __renderTargets.begin(); it < __renderTargets.end(); ++it)
    {
        RenderTarget* rt = *it;
        if (strcmp(id, rt->getID()) == 0)
        {
            return rt;
        }
    }

    return NULL;
}
 
RenderTarget::~RenderTarget()
{
    SAFE_RELEASE(_texture);

    // Erase this RenderTarget from the vector.
    std::vector<RenderTarget*>::const_iterator it = __renderTargets.begin() + _index;
    __renderTargets.erase(it);
}

const char* RenderTarget::getID() const
{
    return _id.c_str();
}
     
Texture* RenderTarget::getTexture() const
{
    return _texture;
}
 
RenderTarget::RenderTarget(const char* id, unsigned int width, unsigned int height)
{
    if (id)
    {
        _id = id;
    }

    _texture = Texture::create(Texture::RGBA8888, width, height, NULL, true);
}

}