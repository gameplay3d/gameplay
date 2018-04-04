#include "Base.h"
#include "PhysicsJointGeneric.h"

namespace gameplay
{

PhysicsJointGeneric::PhysicsJointGeneric() 
{
}

PhysicsJointGeneric::~PhysicsJointGeneric()
{
}

PhysicsJoint::Type PhysicsJointGeneric::getType() const
{
    return Type::eGeneric;
}

float PhysicsJointGeneric::getTwistX() const
{
    return 0.0f;
}

float PhysicsJointGeneric::getSwingY() const
{
    return 0.0f;
}

float PhysicsJointGeneric::getSwingZ() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setMotionRange(MotionAxis axis, MotionRange range)
{
}

PhysicsJointGeneric::MotionRange PhysicsJointGeneric::getMotionRange(MotionAxis axis) const
{
    return MotionRange();
}

void PhysicsJointGeneric::setLinearLimitExtent(float extent)
{
}

float PhysicsJointGeneric::getLinearLimitExtent() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setLinearLimitBounce(float bounce)
{
}

float PhysicsJointGeneric::getLinearLimitBounce() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setLinearLimitContactDistance(float contactDistance)
{
}

float PhysicsJointGeneric::getLinearLimitContactDistance() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setLinearLimitSpringStiffness(float stiffness)
{
}

float PhysicsJointGeneric::getLinearLimitSpringStiffness() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setLinearLimitSpringDampening(float dampening)
{
}

float PhysicsJointGeneric::getLinearLimitSpringDampening() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setTwistLimitExtent(float extent)
{
}

float PhysicsJointGeneric::getTwistLimitExtent() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setTwistLimitBounce(float bounce)
{
}

float PhysicsJointGeneric::getTwistLimitBounce() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setTwistLimitContactDistance(float contactDistance)
{
}

float PhysicsJointGeneric::getTwistLimitContactDistance() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setTwistLimitSpringStiffness(float stiffness)
{
}

float PhysicsJointGeneric::getTwistLimitSpringStiffness() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setTwistLimitSpringDampening(float dampening)
{
}

float PhysicsJointGeneric::getTwistLimitSpringDampening() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setSwingLimitExtent(float extent)
{
}

float PhysicsJointGeneric::getSwingLimitExtent() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setSwingLimitBounce(float bounce)
{
}

float PhysicsJointGeneric::getSwingLimitBounce() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setSwingLimitContactDistance(float contactDistance)
{
}

float PhysicsJointGeneric::getSwingLimitContactDistance() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setSwingLimitSpringStiffness(float stiffness)
{
}

float PhysicsJointGeneric::getSwingLimitSpringStiffness() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setSwingLimitSpringDampening(float dampening)
{
}

float PhysicsJointGeneric::getSwingLimitSpringDampening() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setDriveTargetPosition(const Vector3 & targetPosition)
{
}

Vector3 PhysicsJointGeneric::getDriveTargetPosition() const
{
    return Vector3::zero();
}

void PhysicsJointGeneric::setDriveTargetVelocity(float targetVelocity)
{
}

float PhysicsJointGeneric::getDriveTargetVelocity() const
{
    return 0.0f;
}

void PhysicsJointGeneric::setDriveStiffness(MotionAxis axis, float stiffness)
{
}

float PhysicsJointGeneric::getDriveStiffness(MotionAxis axis)
{
    return 0.0f;
}

void PhysicsJointGeneric::setDriveDampening(MotionAxis axis, float dampening)
{
}

float PhysicsJointGeneric::getDriveDampening(MotionAxis axis)
{
    return 0.0f;
}

void PhysicsJointGeneric::setDriveForce(MotionAxis axis, float force)
{
}

float PhysicsJointGeneric::getDriveForce(MotionAxis axis)
{
    return 0.0f;
}

std::shared_ptr<Serializable> PhysicsJointGeneric::createObject()
{
    return std::shared_ptr<PhysicsJointGeneric>();
}

std::string PhysicsJointGeneric::getClassName()
{
    return "gameplay::PhysicsJointGeneric";
}

void PhysicsJointGeneric::onSerialize(Serializer * serializer)
{
    PhysicsJoint::onSerialize(serializer);
}

void PhysicsJointGeneric::onDeserialize(Serializer * serializer)
{
    PhysicsJoint::onDeserialize(serializer);
}

}
