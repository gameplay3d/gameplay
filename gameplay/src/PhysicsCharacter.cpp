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

std::shared_ptr<Serializable> PhysicsCharacter::createObject()
{
    return std::shared_ptr<PhysicsCharacter>();
}

Component::ClassType PhysicsCharacter::getClassType()
{
    return ClassType::ePhysicsCharacter;
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

}
