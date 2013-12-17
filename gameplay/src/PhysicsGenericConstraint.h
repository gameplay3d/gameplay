#ifndef PHYSICSGENERICCONSTRAINT_H_
#define PHYSICSGENERICCONSTRAINT_H_

#include "PhysicsConstraint.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace gameplay
{
    class PhysicsRigidBody;

/**
 * Defines a completely generic constraint between two
 * rigid bodies (or one rigid body and the world) where the
 * limits for all six degrees of freedom can be set individually.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsGenericConstraint : public PhysicsConstraint
{
    friend class PhysicsController;

public:

    /**
     * Gets the rotation offset for the first rigid body in the constraint.
     * 
     * @return The rotation offset.
     */
    inline const Quaternion& getRotationOffsetA() const;

    /**
     * Gets the rotation offset for the second rigid body in the constraint.
     * 
     * @return The rotation offset.
     */
    inline const Quaternion& getRotationOffsetB() const;

    /**
     * Gets the translation offset for the first rigid body in the constraint.
     * 
     * @return The translation offset.
     */
    inline const Vector3& getTranslationOffsetA() const;

    /**
     * Gets the translation offset for the second rigid body in the constraint.
     * 
     * @return The translation offset.
     */
    inline const Vector3& getTranslationOffsetB() const;

    /**
     * Sets the lower angular limits (as Euler angle limits) along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The lower angular limits (as Euler angle limits) along the local X, Y, and Z axes.
     */
    inline void setAngularLowerLimit(const Vector3& limits);

    /**
     * Sets the upper angular limits (as Euler angle limits) along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The upper angular limits (as Euler angle limits) along the local X, Y, and Z axes.
     */
    inline void setAngularUpperLimit(const Vector3& limits);
    
    /**
     * Sets the lower linear limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The lower linear limits along the local X, Y, and Z axes.
     */
    inline void setLinearLowerLimit(const Vector3& limits);
    
    /**
     * Sets the upper linear limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The upper linear limits along the local X, Y, and Z axes.
     */
    inline void setLinearUpperLimit(const Vector3& limits);

    /**
     * Sets the rotation offset for the first rigid body in the constraint.
     * 
     * @param rotationOffset The rotation offset.
     */
    inline void setRotationOffsetA(const Quaternion& rotationOffset);

    /**
     * Sets the rotation offset for the second rigid body in the constraint.
     * 
     * @param rotationOffset The rotation offset.
     */
    inline void setRotationOffsetB(const Quaternion& rotationOffset);

    /**
     * Sets the translation offset for the first rigid body in the constraint.
     * 
     * @param translationOffset The translation offset.
     */
    inline void setTranslationOffsetA(const Vector3& translationOffset);

    /**
     * Sets the translation offset for the second rigid body in the constraint.
     * 
     * @param translationOffset The translation offset.
     */
    inline void setTranslationOffsetB(const Vector3& translationOffset);

protected:

    /**
     * Constructor.
     *
     * Note: This should only used by subclasses that do not want
     * the _constraint member variable to be initialized.
     */
    PhysicsGenericConstraint();

    /**
     * Creates a generic constraint so that the rigid body (or bodies) is
     * (are) constrained to its (their) current world position(s).
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

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
                             PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

    /**
     * Destructor.
     */
    virtual ~PhysicsGenericConstraint();

private:

    mutable Quaternion* _rotationOffsetA;
    mutable Quaternion* _rotationOffsetB;
    mutable Vector3* _translationOffsetA;
    mutable Vector3* _translationOffsetB;
};

}

#include "PhysicsGenericConstraint.inl"

#endif
