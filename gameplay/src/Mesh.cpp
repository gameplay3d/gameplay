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

std::shared_ptr<Serializable> Mesh::createObject()
{
    return std::shared_ptr<Mesh>();
}

Component::ClassType Mesh::getClassType()
{
    return ClassType::eMesh;
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

}
