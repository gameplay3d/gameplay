/*
 * PhysicsConstraint.h
 */

#ifndef PHYSICSCONSTRAINT_H_
#define PHYSICSCONSTRAINT_H_

#include "PhysicsController.h"

namespace gameplay
{

/**
 * Base class for physics constraints.
 */
class PhysicsConstraint
{
    friend class PhysicsController;

public:
    /**
     * Constructor.
     */
    PhysicsConstraint() : _constraint(NULL) {}

protected:
    btTypedConstraint* _constraint;
};

}

#endif
