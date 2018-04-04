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

std::shared_ptr<Serializable> PhysicsCloth::createObject()
{
    return std::shared_ptr<PhysicsCloth>();
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

}
