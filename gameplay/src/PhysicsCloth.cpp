#include "Base.h"
#include "PhysicsCloth.h"

namespace gameplay
{

PhysicsCloth::PhysicsCloth()
{
}

PhysicsCloth::~PhysicsCloth()
{
}

Component::TypeId PhysicsCloth::getTypeId()
{
	return Component::TYPEID_PHYSICS_CLOTH;
}

std::string PhysicsCloth::getClassName()
{
	return "gameplay::PhysicsCloth";
}

void PhysicsCloth::onSerialize(Serializer * serializer)
{
}

void PhysicsCloth::onDeserialize(Serializer * serializer)
{
}

std::shared_ptr<Serializable> PhysicsCloth::createObject()
{
	return std::shared_ptr<PhysicsCloth>();
}

}