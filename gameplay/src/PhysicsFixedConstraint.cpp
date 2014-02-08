#include "Base.h"
#include "PhysicsFixedConstraint.h"

namespace gameplay
{

PhysicsFixedConstraint::PhysicsFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
    : PhysicsGenericConstraint(a, b)
{
    PhysicsGenericConstraint::setAngularLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
    PhysicsGenericConstraint::setAngularUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
    PhysicsGenericConstraint::setLinearLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
    PhysicsGenericConstraint::setLinearUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
}

PhysicsFixedConstraint::~PhysicsFixedConstraint()
{
    // Not used.
}

}
