#include "PhysicsRigidBody.h"
#include "Base.h"

namespace gameplay
{

inline float PhysicsRigidBody::getAngularDamping() const
{
    return _body->getAngularDamping();
}

inline const Vector3& PhysicsRigidBody::getAngularVelocity() const
{
    if (!_angularVelocity)
        _angularVelocity = new Vector3();

    const btVector3& v = _body->getAngularVelocity();
    _angularVelocity->set(v.x(), v.y(), v.z());
    return *_angularVelocity;
}

inline const Vector3& PhysicsRigidBody::getAnisotropicFriction() const
{
    if (!_anisotropicFriction)
        _anisotropicFriction = new Vector3();

    const btVector3& af = _body->getAnisotropicFriction();
    _anisotropicFriction->set(af.x(), af.y(), af.z());
    return *_anisotropicFriction;
}

inline float PhysicsRigidBody::getFriction() const
{
    return _body->getFriction();
}

inline const Vector3& PhysicsRigidBody::getGravity() const
{
    if (!_gravity)
        _gravity = new Vector3();

    const btVector3& g = _body->getGravity();
    _gravity->set(g.x(), g.y(), g.z());
    return *_gravity;
}

inline float PhysicsRigidBody::getLinearDamping() const
{
    return _body->getLinearDamping();
}

inline const Vector3& PhysicsRigidBody::getLinearVelocity() const
{
    if (!_linearVelocity)
        _linearVelocity = new Vector3();

    const btVector3& v = _body->getLinearVelocity();
    _linearVelocity->set(v.x(), v.y(), v.z());
    return *_linearVelocity;
}

inline Node* PhysicsRigidBody::getNode()
{
    return _node;
}

inline float PhysicsRigidBody::getRestitution() const
{
    return _body->getRestitution();
}

inline bool PhysicsRigidBody::isKinematic() const
{
    return _body->isKinematicObject();
}

inline bool PhysicsRigidBody::isStatic() const
{
    return _body->isStaticObject();
}

inline bool PhysicsRigidBody::isDynamic() const
{
    return !_body->isStaticOrKinematicObject();
}

inline void PhysicsRigidBody::setAngularVelocity(const Vector3& velocity)
{
    _body->setAngularVelocity(BV(velocity));
}

inline void PhysicsRigidBody::setAnisotropicFriction(const Vector3& friction)
{
    _body->setAnisotropicFriction(BV(friction));
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
    _body->setGravity(BV(gravity));
}

inline void PhysicsRigidBody::setKinematic(bool kinematic)
{
    if (kinematic)
    {
        _body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        _body->setActivationState(DISABLE_DEACTIVATION);
    }
    else
    {
        _body->setCollisionFlags(_body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        _body->setActivationState(ACTIVE_TAG);
    }
}

inline void PhysicsRigidBody::setLinearVelocity(const Vector3& velocity)
{
    _body->setLinearVelocity(BV(velocity));
}

inline void PhysicsRigidBody::setRestitution(float restitution)
{
    _body->setRestitution(restitution);
}

}
