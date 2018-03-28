#include "Base.h"
#include "PhysicsVehicle.h"

namespace gameplay
{

PhysicsVehicle::PhysicsVehicle()
{
}

PhysicsVehicle::~PhysicsVehicle()
{
}

std::shared_ptr<Serializable> PhysicsVehicle::createObject()
{
    return std::shared_ptr<PhysicsVehicle>();
}

Component::ClassType PhysicsVehicle::getClassType()
{
    return ClassType::ePhysicsVehicle;
}

std::string PhysicsVehicle::getClassName()
{
    return "gameplay::PhysicsVehicle";
}

void PhysicsVehicle::onSerialize(Serializer * serializer)
{
}

void PhysicsVehicle::onDeserialize(Serializer * serializer)
{
}

}
