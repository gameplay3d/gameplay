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

Component::TypeId PhysicsVehicle::getTypeId()
{
	return Component::TYPEID_PHYSICS_VEHICLE;
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

std::shared_ptr<Serializable> PhysicsVehicle::createObject()
{
	return std::shared_ptr<PhysicsVehicle>();
}

}