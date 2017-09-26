#include "Base.h"
#include "Model.h"
#include "FileSystem.h"
#include "Vector3.h"
#include "Vector2.h"

namespace gameplay
{

Model::Model()
{
}
Model::~Model()
{
}

void Model::load(const std::string& url)
{
}

Component::TypeId Model::getTypeId()
{
	return Component::TYPEID_MODEL;
}

std::string Model::getClassName()
{
	return "gameplay::Model";
}

void Model::onSerialize(Serializer* serializer)
{
	Renderer::onSerialize(serializer);
}

void Model::onDeserialize(Serializer* serializer)
{
	Renderer::onDeserialize(serializer);
}

std::shared_ptr<Serializable> Model::createObject()
{
	return std::shared_ptr<Model>();
}

}