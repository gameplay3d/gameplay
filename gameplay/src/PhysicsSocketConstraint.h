#ifndef PHYSICSSOCKETCONSTRAINT_H_
#define PHYSICSSOCKETCONSTRAINT_H_

#include "PhysicsConstraint.h"
#include "PhysicsRigidBody.h"
#include "Vector3.h"

namespace gameplay
{

/**
 * Defines a ball-socket or point-to-point constraint
 * between two rigid bodies (or one rigid body and the world)
 * where rotation is unrestricted about the constraint joint (pivot point).
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsSocketConstraint : public PhysicsConstraint
{
    friend class PhysicsController;

private:

    /**
     * Creates a socket constraint so that the rigid body (or bodies) is
     * (are) constrained using its (their) current world position(s) for
     * the translation offset(s) to the constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

    /**
     * Creates a socket constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsSocketConstraint(PhysicsRigidBody* a, const Vector3& translationOffsetA, 
                            PhysicsRigidBody* b, const Vector3& translationOffsetB);

    /**
     * Destructor.
     */
    ~PhysicsSocketConstraint();
};

}

#endif
