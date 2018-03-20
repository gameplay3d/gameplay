#include "Base.h"
#include "Renderer.h"

namespace gameplay
{

Renderer::Renderer() : 
    _url(""),
    _materialShared(nullptr),
    _loaded(false)
{
}

Renderer::~Renderer()
{
}

std::string Renderer::getUrl() const
{
    return _url;
}

std::shared_ptr<Material> Renderer::getMaterial(int submeshIndex)
{
    GP_ASSERT(submeshIndex == -1 || submeshIndex >= 0);
    if (submeshIndex < 0)
        return _materialShared;
    //if (submeshIndex >= (int)_submesh.size())
    //    return nullptr;
    
    std::shared_ptr<Material> material = nullptr;

    // Look up explicitly specified part material.
    if (_materials.size() > 0)
    {
        material = _materials[submeshIndex];
    }
    if (material == nullptr)
    {
        // Return the shared material.
         material = _materialShared;
    }
    return material;
}

void Renderer::setMaterial(std::shared_ptr<Material> material, int submeshIndex)
{
    // TODO
}

bool Renderer::hasMaterial(size_t submeshIndex) const
{
    return false;
    //return ((submeshIndex < _submesh.size()) && (_materials.size() > 0) && _materials[submeshIndex]);
}

void Renderer::onSerialize(Serializer * serializer)
{
}

void Renderer::onDeserialize(Serializer * serializer)
{
}

bool Renderer::isLoaded() const
{
    return _loaded;
}

void Renderer::onLoaded()
{
    _loaded = true;
}

}