#include "Base.h"
#include "Mesh.h"
#include "FileSystem.h"
#include "Vector3.h"
#include "Vector2.h"

namespace gameplay
{

Mesh::Mesh()
{
}
Mesh::~Mesh()
{
}

void Mesh::load(const std::string& url)
{
}

Component::TypeId Mesh::getTypeId()
{
	return Component::TYPEID_MESH;
}

std::string Mesh::getClassName()
{
	return "gameplay::Mesh";
}

void Mesh::onSerialize(Serializer* serializer)
{
	Renderer::onSerialize(serializer);
}

void Mesh::onDeserialize(Serializer* serializer)
{
	Renderer::onDeserialize(serializer);
}

std::shared_ptr<Serializable> Mesh::createObject()
{
	return std::shared_ptr<Mesh>();
}

}
