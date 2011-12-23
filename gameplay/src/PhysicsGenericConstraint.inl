#include "PhysicsGenericConstraint.h"

namespace gameplay
{

inline const Quaternion& PhysicsGenericConstraint::getRotationOffsetA() const
{
    if (!_rotationOffsetA)
        _rotationOffsetA = new Quaternion();

    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getRotation();
    _rotationOffsetA->set(ro.x(), ro.y(), ro.z(), ro.w());
    return *_rotationOffsetA;
}

inline const Quaternion& PhysicsGenericConstraint::getRotationOffsetB() const
{
    if (!_rotationOffsetB)
        _rotationOffsetB = new Quaternion();

    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getRotation();
    _rotationOffsetB->set(ro.x(), ro.y(), ro.z(), ro.w());
    return *_rotationOffsetB;
}

inline const Vector3& PhysicsGenericConstraint::getTranslationOffsetA() const
{
    if (!_translationOffsetA)
        _translationOffsetA = new Vector3();

    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getOrigin();
    _translationOffsetA->set(to.x(), to.y(), to.z());
    return *_translationOffsetA;
}

inline const Vector3& PhysicsGenericConstraint::getTranslationOffsetB() const
{
    if (!_translationOffsetB)
        _translationOffsetB = new Vector3();

    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getOrigin();
    _translationOffsetB->set(to.x(), to.y(), to.z());
    return *_translationOffsetB;
}

inline void PhysicsGenericConstraint::setAngularLowerLimit(const Vector3& limits)
{
    ((btGeneric6DofConstraint*)_constraint)->setAngularLowerLimit(btVector3(limits.x, limits.y, limits.z));
}

inline void PhysicsGenericConstraint::setAngularUpperLimit(const Vector3& limits)
{
    ((btGeneric6DofConstraint*)_constraint)->setAngularUpperLimit(btVector3(limits.x, limits.y, limits.z));
}

inline void PhysicsGenericConstraint::setLinearLowerLimit(const Vector3& limits)
{
    ((btGeneric6DofConstraint*)_constraint)->setLinearLowerLimit(btVector3(limits.x, limits.y, limits.z));
}
    
inline void PhysicsGenericConstraint::setLinearUpperLimit(const Vector3& limits)
{
    ((btGeneric6DofConstraint*)_constraint)->setLinearUpperLimit(btVector3(limits.x, limits.y, limits.z));
}

inline void PhysicsGenericConstraint::setRotationOffsetA(const Quaternion& rotationOffset)
{
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().setRotation(btQuaternion(rotationOffset.x, rotationOffset.y, rotationOffset.z, rotationOffset.w));
}

inline void PhysicsGenericConstraint::setRotationOffsetB(const Quaternion& rotationOffset)
{
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().setRotation(btQuaternion(rotationOffset.x, rotationOffset.y, rotationOffset.z, rotationOffset.w));
}

inline void PhysicsGenericConstraint::setTranslationOffsetA(const Vector3& translationOffset)
{
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().setOrigin(btVector3(translationOffset.x, translationOffset.y, translationOffset.z));
}

inline void PhysicsGenericConstraint::setTranslationOffsetB(const Vector3& translationOffset)
{
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().setOrigin(btVector3(translationOffset.x, translationOffset.y, translationOffset.z));
}

}
