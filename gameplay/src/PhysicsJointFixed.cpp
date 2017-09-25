#include "Base.h"
#include "PhysicsJointFixed.h"

namespace gameplay
{

PhysicsJointFixed::PhysicsJointFixed() 
{
}

PhysicsJointFixed::~PhysicsJointFixed()
{
}

PhysicsJoint::Type PhysicsJointFixed::getType() const
{
	return PhysicsJoint::TYPE_FIXED;
}

Component::TypeId PhysicsJointFixed::getTypeId()
{
	return Component::TYPEID_PHYSICS_JOINT_FIXED;
}

std::string PhysicsJointFixed::getClassName()
{
	return "gameplay::PhysicsJointFixed";
}

void PhysicsJointFixed::onSerialize(Serializer * serializer)
{
}

void PhysicsJointFixed::onDeserialize(Serializer * serializer)
{
}

std::shared_ptr<Serializable> PhysicsJointFixed::createObject()
{
	return std::shared_ptr<PhysicsJointFixed>();
}

}