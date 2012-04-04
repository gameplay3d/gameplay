#include "PhysicsRigidBody.h"
#include "Base.h"

namespace gameplay
{

inline float PhysicsRigidBody::getMass() const
{
    return _mass;
}

inline float PhysicsRigidBody::getFriction() const
{
    return _body->getFriction();
}

inline void PhysicsRigidBody::setFriction(float friction)
{
    _body->setFriction(friction);
}

inline float PhysicsRigidBody::getRestitution() const
{
    return _body->getRestitution();
}

inline void PhysicsRigidBody::setRestitution(float restitution)
{
    _body->setRestitution(restitution);
}

inline float PhysicsRigidBody::getLinearDamping() const
{
    return _body->getLinearDamping();
}

inline float PhysicsRigidBody::getAngularDamping() const
{
    return _body->getAngularDamping();
}

inline void PhysicsRigidBody::setDamping(float linearDamping, float angularDamping)
{
    _body->setDamping(linearDamping, angularDamping);
}

inline Vector3 PhysicsRigidBody::getLinearVelocity() const
{
    const btVector3& v = _body->getLinearVelocity();
    return Vector3(v.x(), v.y(), v.z());
}

inline void PhysicsRigidBody::setLinearVelocity(const Vector3& velocity)
{
    _body->setLinearVelocity(BV(velocity));
}

inline Vector3 PhysicsRigidBody::getAngularVelocity() const
{
    const btVector3& v = _body->getAngularVelocity();
    return Vector3(v.x(), v.y(), v.z());
}

inline void PhysicsRigidBody::setAngularVelocity(const Vector3& velocity)
{
    _body->setAngularVelocity(BV(velocity));
}

inline Vector3 PhysicsRigidBody::getAnisotropicFriction() const
{
    const btVector3& af = _body->getAnisotropicFriction();
    return Vector3(af.x(), af.y(), af.z());
}

inline void PhysicsRigidBody::setAnisotropicFriction(const Vector3& friction)
{
    _body->setAnisotropicFriction(BV(friction));
}

inline Vector3 PhysicsRigidBody::getGravity() const
{
    const btVector3& g = _body->getGravity();
    return Vector3(g.x(), g.y(), g.z());
}

inline void PhysicsRigidBody::setGravity(const Vector3& gravity)
{
    _body->setGravity(BV(gravity));
}

inline bool PhysicsRigidBody::isStatic() const
{
    return _body->isStaticObject();
}

}
