/**
 * PhysicsRigidBody.inl
 */

#include "PhysicsRigidBody.h"

namespace gameplay
{

// TODO: We can't cache these Vector3 values, but should we have member variables
// that are set each time instead of creating a new Vector3 every time?

inline float PhysicsRigidBody::getAngularDamping() const
{
	return _body->getAngularDamping();
}

inline Vector3 PhysicsRigidBody::getAngularVelocity() const
{
	const btVector3& v = _body->getAngularVelocity();
	return Vector3(v.x(), v.y(), v.z());
}

inline Vector3 PhysicsRigidBody::getAnisotropicFriction() const
{
    const btVector3& af = _body->getAnisotropicFriction();
    return Vector3(af.x(), af.y(), af.z());
}

inline float PhysicsRigidBody::getFriction() const
{
    return _body->getFriction();
}

inline Vector3 PhysicsRigidBody::getGravity() const
{
    const btVector3& g = _body->getGravity();
    return Vector3(g.x(), g.y(), g.z());
}

inline float PhysicsRigidBody::getLinearDamping() const
{
	return _body->getLinearDamping();
}

inline Vector3 PhysicsRigidBody::getLinearVelocity() const
{
	const btVector3& v = _body->getLinearVelocity();
	return Vector3(v.x(), v.y(), v.z());
}

inline float PhysicsRigidBody::getRestitution() const
{
    return _body->getRestitution();
}

inline void PhysicsRigidBody::setAngularVelocity(const Vector3& velocity)
{
	_body->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

inline void PhysicsRigidBody::setAnisotropicFriction(const Vector3& friction)
{
    _body->setAnisotropicFriction(btVector3(friction.x, friction.y, friction.z));
}

inline void PhysicsRigidBody::setDamping(float linearDamping, float angularDamping)
{
    _body->setDamping(linearDamping, angularDamping);
}

inline void PhysicsRigidBody::setFriction(float friction)
{
    _body->setFriction(friction);
}

inline void PhysicsRigidBody::setGravity(const Vector3& gravity)
{
    _body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

inline void PhysicsRigidBody::setLinearVelocity(const Vector3& velocity)
{
	_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

inline void PhysicsRigidBody::setRestitution(float restitution)
{
    _body->setRestitution(restitution);
}

inline void PhysicsRigidBody::addConstraint(PhysicsConstraint* constraint)
{
    _constraints.push_back(constraint);
}

}
