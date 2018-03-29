#include "Base.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

PhysicsRigidBody::PhysicsRigidBody()
{
}

PhysicsRigidBody::~PhysicsRigidBody()
{
}

void PhysicsRigidBody::setMass(float mass) const
{
}

float PhysicsRigidBody::getMass() const
{
    return 0.0f;
}

void PhysicsRigidBody::setLinearVelocity(float linearVelocity) const
{
}

float PhysicsRigidBody::getLinearVelocity() const
{
    return 0.0f;
}

void PhysicsRigidBody::setAngularVelocity(float angularVelocity) const
{
}

float PhysicsRigidBody::getAngularVelocity() const
{
    return 0.0f;
}

void PhysicsRigidBody::setLinearDrag(float linearDrag) const
{
}

float PhysicsRigidBody::getLinearDrag() const
{
    return 0.0f;
}

void PhysicsRigidBody::setAngularDrag(float angularDrag) const
{
}

float PhysicsRigidBody::getAngularDrag() const
{
    return 0.0f;
}

void PhysicsRigidBody::setGravityEnabled(bool gravityEnabled)
{
}

bool PhysicsRigidBody::isGravityEnabled() const
{
    return false;
}

void PhysicsRigidBody::setKinematic(bool kinematic)
{
}

bool PhysicsRigidBody::isKinematic() const
{
    return false;
}

void PhysicsRigidBody::setFreezeConstraint(uint32_t freezeConstraints)
{
}

uint32_t PhysicsRigidBody::getFreezeConstraint() const
{
    return 0;
}

float PhysicsRigidBody::getMassCenter() const
{
    return 0.0f;
}

void PhysicsRigidBody::addForce(const Vector3 & force, PhysicsRigidBody::ForceMode mode)
{
}

void PhysicsRigidBody::clearForce(PhysicsRigidBody::ForceMode mode)
{
}

void PhysicsRigidBody::addTorque(const Vector3 & force, PhysicsRigidBody::ForceMode mode)
{
}

void PhysicsRigidBody::clearTorque(PhysicsRigidBody::ForceMode mode)
{
}

std::shared_ptr<Serializable> PhysicsRigidBody::createObject()
{
    return std::shared_ptr<PhysicsRigidBody>();
}

Component::ClassType PhysicsRigidBody::getClassType()
{
    return ClassType::ePhysicsRigidBody;
}

std::string PhysicsRigidBody::getClassName()
{
    return "gameplay::PhysicsRigidBody";
}

void PhysicsRigidBody::onSerialize(Serializer * serializer)
{
}

void PhysicsRigidBody::onDeserialize(Serializer * serializer)
{
}

}
