#ifndef PHYSICSHINGECONSTRAINT_H_
#define PHYSICSHINGECONSTRAINT_H_

#include "PhysicsConstraint.h"
#include "PhysicsRigidBody.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace gameplay
{

/**
 * Defines a hinge constraint between two rigid bodies
 * (or one rigid body and the world) where movement is
 * restricted to rotation about one axis.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsHingeConstraint : public PhysicsConstraint
{
    friend class PhysicsController;

public:

    /**
     * Sets the limits (and optionally, some properties) for the hinge.
     * 
     * @param minAngle The minimum angle for the hinge.
     * @param maxAngle The maximum angle for the hinge.
     * @param bounciness The bounciness of the hinge (this is applied as
     *      a factor to the incoming velocity when a hinge limit is met in
     *      order to calculate the outgoing velocity-for example, 0.0 corresponds
     *      to no bounce and 1.0 corresponds to an outgoing velocity that is equal
     *      in magnitude to the incoming velocity).
     */
    void setLimits(float minAngle, float maxAngle, float bounciness = 1.0f);

private:
    /**
     * Creates a hinge constraint.
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
    PhysicsHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
                           PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

    /**
     * Destructor.
     */
    ~PhysicsHingeConstraint();
};

}

#endif
