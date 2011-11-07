/**
 * PhysicsGenericConstraint.inl
 */

#include "PhysicsGenericConstraint.h"

namespace gameplay
{

inline Quaternion PhysicsGenericConstraint::getRotationOffsetA()
{
    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getRotation();
    return Quaternion(ro.x(), ro.y(), ro.z(), ro.w());
}

inline Quaternion PhysicsGenericConstraint::getRotationOffsetB()
{
    btQuaternion ro = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getRotation();
    return Quaternion(ro.x(), ro.y(), ro.z(), ro.w());
}

inline Vector3 PhysicsGenericConstraint::getTranslationOffsetA()
{
    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetA().getOrigin();
    return Vector3(to.x(), to.y(), to.z());
}

inline Vector3 PhysicsGenericConstraint::getTranslationOffsetB()
{
    btVector3 to = static_cast<btGeneric6DofConstraint*>(_constraint)->getFrameOffsetB().getOrigin();
    return Vector3(to.x(), to.y(), to.z());
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
