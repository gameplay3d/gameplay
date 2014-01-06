#include "PhysicsFixedConstraint.h"

namespace gameplay
{

inline void PhysicsFixedConstraint::setAngularLowerLimit(const Vector3& limit)
{
    PhysicsGenericConstraint::setAngularLowerLimit(limit);
}

inline void PhysicsFixedConstraint::setAngularUpperLimit(const Vector3& limit)
{
    PhysicsGenericConstraint::setAngularUpperLimit(limit);
}

inline void PhysicsFixedConstraint::setLinearLowerLimit(const Vector3& limit)
{
    PhysicsGenericConstraint::setLinearLowerLimit(limit);
}

inline void PhysicsFixedConstraint::setLinearUpperLimit(const Vector3& limit)
{
    PhysicsGenericConstraint::setLinearUpperLimit(limit);
}

}
