#include "PhysicsSpringConstraint.h"

namespace gameplay
{

inline void PhysicsSpringConstraint::setAngularDampingX(float damping)
{
    setDamping(ANGULAR_X, damping);
}

inline void PhysicsSpringConstraint::setAngularDampingY(float damping)
{
    setDamping(ANGULAR_Y, damping);
}

inline void PhysicsSpringConstraint::setAngularDampingZ(float damping)
{
    setDamping(ANGULAR_Z, damping);
}

inline void PhysicsSpringConstraint::setAngularStrengthX(float strength)
{
    setStrength(ANGULAR_X, strength);
}

inline void PhysicsSpringConstraint::setAngularStrengthY(float strength)
{
    setStrength(ANGULAR_Y, strength);
}

inline void PhysicsSpringConstraint::setAngularStrengthZ(float strength)
{
    setStrength(ANGULAR_Z, strength);
}

inline void PhysicsSpringConstraint::setLinearDampingX(float damping)
{
    setDamping(LINEAR_X, damping);
}

inline void PhysicsSpringConstraint::setLinearDampingY(float damping)
{
    setDamping(LINEAR_Y, damping);
}

inline void PhysicsSpringConstraint::setLinearDampingZ(float damping)
{
    setDamping(LINEAR_Z, damping);
}

inline void PhysicsSpringConstraint::setLinearStrengthX(float strength)
{
    setStrength(LINEAR_X, strength);
}

inline void PhysicsSpringConstraint::setLinearStrengthY(float strength)
{
    setStrength(LINEAR_Y, strength);
}

inline void PhysicsSpringConstraint::setLinearStrengthZ(float strength)
{
    setStrength(LINEAR_Z, strength);
}

}
