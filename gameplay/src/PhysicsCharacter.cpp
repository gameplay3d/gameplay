#include "Base.h"
#include "PhysicsCharacter.h"

namespace gameplay
{

PhysicsCharacter::PhysicsCharacter()
{
}

PhysicsCharacter::~PhysicsCharacter()
{
}

Component::TypeId PhysicsCharacter::getTypeId()
{
	return Component::TYPEID_PHYSICS_CHARACTER;
}

std::string PhysicsCharacter::getClassName()
{
	return "gameplay::PhysicsCharacter";
}

void PhysicsCharacter::onSerialize(Serializer * serializer)
{
}

void PhysicsCharacter::onDeserialize(Serializer * serializer)
{
}

std::shared_ptr<Serializable> PhysicsCharacter::createObject()
{
	return std::shared_ptr<PhysicsCharacter>();
}

}