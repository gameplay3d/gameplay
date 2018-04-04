#include "Base.h"
#include "PhysicsCollider.h"
#include "Vector3.h"

namespace gameplay
{

PhysicsCollider::PhysicsCollider()
{
}

PhysicsCollider::~PhysicsCollider()
{
}

void PhysicsCollider::setMaterial(std::shared_ptr<PhysicsMaterial> material)
{
}

std::shared_ptr<PhysicsMaterial> PhysicsCollider::getMaterial() const
{
    return nullptr;
}

void PhysicsCollider::setTrigger(bool trigger)
{
}

bool PhysicsCollider::isTrigger() const
{
    return false;
}

std::shared_ptr<Serializable> PhysicsCollider::createObject()
{
    return std::shared_ptr<PhysicsCollider>();
}

std::string PhysicsCollider::getClassName()
{
    return "gameplay::PhysicsCollider";
}

void PhysicsCollider::onSerialize(Serializer * serializer)
{
}

void PhysicsCollider::onDeserialize(Serializer * serializer)
{
}

}
