#include "Base.h"
#include "PhysicsJoint.h"
#include "Vector3.h"

namespace gameplay
{

PhysicsJoint::PhysicsJoint()
{
}

PhysicsJoint::~PhysicsJoint()
{
}

void PhysicsJoint::setConnectedBody(std::shared_ptr<PhysicsRigidBody> rigidBody)
{
}

std::shared_ptr<PhysicsRigidBody> PhysicsJoint::getConnectedBody() const
{
    return std::shared_ptr<PhysicsRigidBody>();
}

float PhysicsJoint::getBreakingForce() const
{
    return 0.0f;
}

void PhysicsJoint::setBreakingForce(float force)
{
}

float PhysicsJoint::getBreakingTorque() const
{
    return 0.0f;
}

void PhysicsJoint::setBreakingTorque(float force)
{
}

void PhysicsJoint::setAnchorPosition(const Vector3 & anchorPosition)
{
}

Vector3 PhysicsJoint::getAnchorPosition() const
{
    return Vector3();
}

void PhysicsJoint::setCollisionsEnabled(bool collisionsEnabled)
{
}

bool PhysicsJoint::isCollisionsEnabled() const
{
    return false;
}

void PhysicsJoint::setPreprocessingEnabled(bool preprocessingEnabled)
{
}

bool PhysicsJoint::isPreprocessingEnabled() const
{
    return false;
}

void PhysicsJoint::onSerialize(Serializer * serializer)
{
}

void PhysicsJoint::onDeserialize(Serializer * serializer)
{
}

void PhysicsJoint::onConstraintBreak() const
{
}


}