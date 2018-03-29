#include "Base.h"
#include "PhysicsJointSpring.h"

namespace gameplay
{

PhysicsJointSpring::PhysicsJointSpring() 
{
}

PhysicsJointSpring::~PhysicsJointSpring()
{
}

PhysicsJoint::Type PhysicsJointSpring::getType() const
{
    return Type::eSpring;
}

float PhysicsJointSpring::getDistance() const
{
    return 0.0f;
}

void PhysicsJointSpring::setDistanceMin(float distanceMin)
{
}

float PhysicsJointSpring::getDistanceMin() const
{
    return 0.0f;
}

void PhysicsJointSpring::setDistanceMax(float distanceMax)
{
}

float PhysicsJointSpring::getDistanceMax() const
{
    return 0.0f;
}

void PhysicsJointSpring::setTolerance(float tolerance)
{
}

float PhysicsJointSpring::getTolerance() const
{
    return 0.0f;
}

void PhysicsJointSpring::setStiffness(float stiffness)
{
}

float PhysicsJointSpring::getStiffness() const
{
    return 0.0f;
}

void PhysicsJointSpring::setDampening(float dampening)
{
}

float PhysicsJointSpring::getDampening() const
{
    return 0.0f;
}

std::shared_ptr<Serializable> PhysicsJointSpring::createObject()
{
    return std::shared_ptr<PhysicsJointSpring>();
}

Component::ClassType PhysicsJointSpring::getClassType()
{
    return ClassType::ePhysicsJointSpring;
}

std::string PhysicsJointSpring::getClassName()
{
    return "gameplay::PhysicsJointSpring";
}

void PhysicsJointSpring::onSerialize(Serializer * serializer)
{
    PhysicsJoint::onSerialize(serializer);
}

void PhysicsJointSpring::onDeserialize(Serializer * serializer)
{
    PhysicsJoint::onDeserialize(serializer);
}

}
