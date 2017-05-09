#include "Base.h"
#include "Renderer.h"

namespace gameplay
{

Renderer::Renderer() : Component()
{
}

Renderer::~Renderer()
{
}

const Geometry& Renderer::getGeometry() const
{
    return _geometry;
}

size_t Renderer::getMaterialCount() const
{
    return _materials.size();
}

const Material& Renderer::getMaterial(size_t index) const
{
    return _materials[index];
}

}
