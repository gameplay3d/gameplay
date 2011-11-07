/*
 * PhysicsConstraint.h
 */

#ifndef PHYSICSCONSTRAINT_H_
#define PHYSICSCONSTRAINT_H_

#include "Base.h"
#include "Ref.h"
#include "Vector3.h"

namespace gameplay
{
    class Node;

/**
 * Base class for physics constraints.
 */
class PhysicsConstraint : public Ref
{
    friend class PhysicsController;

public:
    /**
     * Gets the impulse needed to break the constraint.
     * 
     * @return The impulse needed to break the constraint.
     */
    inline float getBreakingImpulse() const;

    /**
     * Sets the impulse needed to break the constraint
     * (if an impulse greater than or equal to the given
     * value is applied to the constraint, the constraint
     * will be broken).
     * 
     * @param impulse The impulse needed to break the constraint.
     */
    inline void setBreakingImpulse(float impulse);

    /**
     * Gets whether the constraint is enabled or not.
     * 
     * @return Whether the constraint is enabled or not.
     */
    inline bool isEnabled() const;

    /**
     * Sets whether the constraint is enabled or not.
     * 
     * @param enabled Whether the constraint is enabled or not.
     */
    inline void setEnabled(bool enabled);

protected:
    /**
     * Constructor.
     */
    PhysicsConstraint();

    /**
     * Destructor.
     */
    virtual ~PhysicsConstraint();

    /**
     * Calculates the midpoint between the two nodes.
     */
    static Vector3 midpoint(Node* a, Node* b);

    btTypedConstraint* _constraint;
};

}

#include "PhysicsConstraint.inl"

#endif
