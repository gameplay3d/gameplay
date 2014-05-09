#include "PhysicsGenericConstraint.h"

namespace gameplay
{

inline const Quaternion& PhysicsGenericConstraint::getRotationOffsetA() const
{
    if (!_rotationOffsetA)
        _rotationOffsetA = new Quaternion();

    GP_ASSERT(_constraint);
    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getRotation();
    _rotationOffsetA->set(ro.x(), ro.y(), ro.z(), ro.w());
    return *_rotationOffsetA;
}

inline const Quaternion& PhysicsGenericConstraint::getRotationOffsetB() const
{
    if (!_rotationOffsetB)
        _rotationOffsetB = new Quaternion();

    GP_ASSERT(_constraint);
    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getRotation();
    _rotationOffsetB->set(ro.x(), ro.y(), ro.z(), ro.w());
    return *_rotationOffsetB;
}

inline const Vector3& PhysicsGenericConstraint::getTranslationOffsetA() const
{
    if (!_translationOffsetA)
        _translationOffsetA = new Vector3();

    GP_ASSERT(_constraint);
    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getOrigin();
    _translationOffsetA->set(to.x(), to.y(), to.z());
    return *_translationOffsetA;
}

inline const Vector3& PhysicsGenericConstraint::getTranslationOffsetB() const
{
    if (!_translationOffsetB)
        _translationOffsetB = new Vector3();

    GP_ASSERT(_constraint);
    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getOrigin();
    _translationOffsetB->set(to.x(), to.y(), to.z());
    return *_translationOffsetB;
}

inline void PhysicsGenericConstraint::setAngularLowerLimit(const Vector3& limits)
{
    GP_ASSERT(_constraint);
    ((btGeneric6DofConstraint*)_constraint)->setAngularLowerLimit(BV(limits));
}

inline void PhysicsGenericConstraint::setAngularUpperLimit(const Vector3& limits)
{
    GP_ASSERT(_constraint);
    ((btGeneric6DofConstraint*)_constraint)->setAngularUpperLimit(BV(limits));
}

inline void PhysicsGenericConstraint::setLinearLowerLimit(const Vector3& limits)
{
    GP_ASSERT(_constraint);
    ((btGeneric6DofConstraint*)_constraint)->setLinearLowerLimit(BV(limits));
}
    
inline void PhysicsGenericConstraint::setLinearUpperLimit(const Vector3& limits)
{
    GP_ASSERT(_constraint);
    ((btGeneric6DofConstraint*)_constraint)->setLinearUpperLimit(BV(limits));
}

inline void PhysicsGenericConstraint::setRotationOffsetA(const Quaternion& rotationOffset)
{
    GP_ASSERT(_constraint);
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().setRotation(BQ(rotationOffset));
}

inline void PhysicsGenericConstraint::setRotationOffsetB(const Quaternion& rotationOffset)
{
    GP_ASSERT(_constraint);
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().setRotation(BQ(rotationOffset));
}

inline void PhysicsGenericConstraint::setTranslationOffsetA(const Vector3& translationOffset)
{
    GP_ASSERT(_constraint);
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().setOrigin(BV(translationOffset));
}

inline void PhysicsGenericConstraint::setTranslationOffsetB(const Vector3& translationOffset)
{
    GP_ASSERT(_constraint);
    static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().setOrigin(BV(translationOffset));
}

}
