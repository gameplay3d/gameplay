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

std::shared_ptr<Serializable> PhysicsJointFixed::createObject()
{
    return std::shared_ptr<PhysicsJointFixed>();
}

PhysicsJoint::Type PhysicsJointFixed::getType() const
{
    return Type::eFixed;
}

Component::ClassType PhysicsJointFixed::getClassType()
{
    return ClassType::ePhysicsJointFixed;
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

}