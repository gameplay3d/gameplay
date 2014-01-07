#ifndef PHYSICSFIXEDCONSTRAINT_H_
#define PHYSICSFIXEDCONSTRAINT_H_

#include "PhysicsGenericConstraint.h"

namespace gameplay
{

/**
 * Defines a constraint where two rigid bodies 
 * (or one rigid body and the world) are bound together.
 *
 * This is similar in concept to parenting one node to another,
 * but can be used in specific situations for a more appropriate effect
 * Ex. for implementing sticky projectiles, etc.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsFixedConstraint : public PhysicsGenericConstraint
{
    friend class PhysicsController;

protected:

    /**
     * @see PhysicsGenericConstraint
     */
    PhysicsFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

    /**
     * Destructor.
     */
    ~PhysicsFixedConstraint();

    // Note: We make these functions protected to prevent usage
    // (these are public in the base class, PhysicsGenericConstraint).

    /**
     * Protected to prevent usage.
     */
    inline void setAngularLowerLimit(const Vector3& limit);
    
    /**
     * Protected to prevent usage.
     */
    inline void setAngularUpperLimit(const Vector3& limit);
    
    /**
     * Protected to prevent usage.
     */
    inline void setLinearLowerLimit(const Vector3& limit);
    
    /**
     * Protected to prevent usage.
     */
    inline void setLinearUpperLimit(const Vector3& limit);
};

}

#include "PhysicsFixedConstraint.inl"

#endif
