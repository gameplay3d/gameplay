#include "Base.h"
#include "Renderer.h"

namespace gameplay
{

Renderer::Renderer() 
	: _url(""), _mesh(nullptr), _materialShared(nullptr), _partCount(0), _loaded(false)
{
}

Renderer::~Renderer()
{
}

std::string Renderer::getUrl() const
{
	return _url;
}

std::shared_ptr<Mesh> Renderer::getMesh() const
{
	return _mesh;
}

void Renderer::setMesh(std::shared_ptr<Mesh> mesh)
{
	_mesh = mesh;
}

size_t Renderer::getMeshPartCount() const
{
	return _mesh->getPartCount();
}

std::shared_ptr<Material> Renderer::getMaterial(int partIndex)
{
	GP_ASSERT(partIndex == -1 || partIndex >= 0);
    if (partIndex < 0)
        return _materialShared;
    if (partIndex >= (int)_partCount)
        return nullptr;
	
	std::shared_ptr<Material> material = nullptr;

    // Look up explicitly specified part material.
    if (_materials.size() > 0)
    {
        material = _materials[partIndex];
    }
    if (material == nullptr)
    {
        // Return the shared material.
         material = _materialShared;
    }
    return material;
}

void Renderer::setMaterial(std::shared_ptr<Material> material, int partIndex)
{
    // TODO
}

bool Renderer::hasMaterial(size_t partIndex) const
{
	return ((partIndex < _partCount) && (_materials.size() > 0) && _materials[partIndex]);
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