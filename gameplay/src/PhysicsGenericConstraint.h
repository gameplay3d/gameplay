/*
 * PhysicsGenericConstraint.h
 */

#ifndef PHYSICSGENERICCONSTRAINT_H_
#define PHYSICSGENERICCONSTRAINT_H_

#include "PhysicsConstraint.h"
#include "PhysicsRigidBody.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace gameplay
{

/**
 * Represents a completely generic constraint between two
 * rigid bodies (or one rigid body and the world) where the
 * limits for all six degrees of freedom can be set individually.
 */
class PhysicsGenericConstraint : public PhysicsConstraint
{
    friend class PhysicsController;

public:
    /**
     * Destructor.
     */
    ~PhysicsGenericConstraint()
    {
        if (_constraint)
        {
            delete _constraint;
            _constraint = NULL;
        }
    }

    /**
     * Sets the lower angular limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The lower angular limits along the local X, Y, and Z axes.
     */
    inline void setAngularLowerLimit(const Vector3& limits)
    {
        ((btGeneric6DofConstraint*)_constraint)->setAngularLowerLimit(btVector3(limits.x, limits.y, limits.z));
    }

    /**
     * Sets the upper angular limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The upper angular limits along the local X, Y, and Z axes.
     */
    inline void setAngularUpperLimit(const Vector3& limits)
    {
        ((btGeneric6DofConstraint*)_constraint)->setAngularUpperLimit(btVector3(limits.x, limits.y, limits.z));
    }
    
    /**
     * Sets the lower linear limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The lower linear limits along the local X, Y, and Z axes.
     */
    inline void setLinearLowerLimit(const Vector3& limits)
    {
        ((btGeneric6DofConstraint*)_constraint)->setLinearLowerLimit(btVector3(limits.x, limits.y, limits.z));
    }
    
    /**
     * Sets the upper linear limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The upper linear limits along the local X, Y, and Z axes.
     */
    inline void setLinearUpperLimit(const Vector3& limits)
    {
        ((btGeneric6DofConstraint*)_constraint)->setLinearUpperLimit(btVector3(limits.x, limits.y, limits.z));
    }

protected:
    /**
     * Creates a generic constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param rotationOffsetA The rotation offset for the first rigid body 
     *      (in its local space) with respect to the constraint joint.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param rotationOffsetB The rotation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsGenericConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
        PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
    {
        if (b)
        {
            btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), 
                btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
            btTransform frameInB(btQuaternion(rotationOffsetB.x, rotationOffsetB.y, rotationOffsetB.z, rotationOffsetB.w), 
                btVector3(translationOffsetB.x, translationOffsetB.y, translationOffsetB.z));
            _constraint = new btGeneric6DofConstraint(*a->_body, *b->_body, frameInA, frameInB, true);
        }
        else
        {
            btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), 
                btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
            _constraint = new btGeneric6DofConstraint(*a->_body, frameInA, true);
        }
    }
};

}

#endif
