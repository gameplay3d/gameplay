#include "Base.h"
#include "PhysicsJointHinge.h"

namespace gameplay
{

PhysicsJointHinge::PhysicsJointHinge() 
{
}

PhysicsJointHinge::~PhysicsJointHinge()
{
}

PhysicsJoint::Type PhysicsJointHinge::getType() const
{
    return PhysicsJoint::TYPE_HINGE;
}

float PhysicsJointHinge::getAngle() const
{
    return 0.0f;
}

float PhysicsJointHinge::getVelocity() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitEnabled(bool enabled)
{
}

bool PhysicsJointHinge::isLimitEnabled() const
{
    return false;
}

void PhysicsJointHinge::setLimitAngleMin(float angleMin)
{
}

float PhysicsJointHinge::getLimitAngleMin() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitAngleMax(float angleMax)
{
}

float PhysicsJointHinge::getLimitAngleMax() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitContactDistance(float contactDistance)
{
}

float PhysicsJointHinge::getLimitContactDistance() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitBounce(float bounce)
{
}

float PhysicsJointHinge::getLimitBounce() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitBounceThreshold(float bounceThreshold)
{
}

float PhysicsJointHinge::getLimitBounceThreshold() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitSpringEnabled(bool enabled)
{
}

bool PhysicsJointHinge::isLimitSpringEnabled()
{
    return false;
}

void PhysicsJointHinge::setLimitSpringStiffness(float stiffness)
{
}

float PhysicsJointHinge::getLimitSpringStiffness() const
{
    return 0.0f;
}

void PhysicsJointHinge::setLimitSpringDampening(float dampening)
{
}

float PhysicsJointHinge::getLimitSpringDampening() const
{
    return 0.0f;
}

void PhysicsJointHinge::setDriveEnabled(bool enabled)
{
}

bool PhysicsJointHinge::isDriveEnabled() const
{
    return false;
}

void PhysicsJointHinge::setDriveTargetVelocity(float targetVelocity)
{
}

float PhysicsJointHinge::getDriveTargetVelocity() const
{
    return 0.0f;
}

void PhysicsJointHinge::setDriveForce(float force)
{
}

float PhysicsJointHinge::getDriveForce() const
{
    return 0.0f;
}

void PhysicsJointHinge::setDriveGearRatio(float gearRatio)
{
}

float PhysicsJointHinge::getDriveGearRatio() const
{
    return 0.0f;
}

Component::TypeId PhysicsJointHinge::getTypeId()
{
    return Component::TYPEID_PHYSICS_JOINT_HINGE;
}

std::string PhysicsJointHinge::getClassName()
{
    return "gameplay::PhysicsJointHinge";
}

void PhysicsJointHinge::onSerialize(Serializer * serializer)
{
    PhysicsJoint::onSerialize(serializer);
}

void PhysicsJointHinge::onDeserialize(Serializer * serializer)
{
    PhysicsJoint::onDeserialize(serializer);
}

std::shared_ptr<Serializable> PhysicsJointHinge::createObject()
{
    return std::shared_ptr<PhysicsJointHinge>();
}

}