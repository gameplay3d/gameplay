#include "Base.h"
#include "MeshInstance.h"
#include "FileSystem.h"

namespace gameplay
{

MeshInstance::MeshInstance()
{
}

MeshInstance::~MeshInstance()
{
}

std::shared_ptr<Serializable> MeshInstance::createObject()
{
    return std::make_shared<MeshInstance>();
}

std::string MeshInstance::getClassName()
{
    return "gameplay::MeshInstance";
}

void MeshInstance::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void MeshInstance::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

}
