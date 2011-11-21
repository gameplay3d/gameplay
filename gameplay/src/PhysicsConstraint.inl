/*
 * PhysicsConstraint.inl
 */

#include "PhysicsConstraint.h"

namespace gameplay
{

inline float PhysicsConstraint::getBreakingImpulse() const
{
    return _constraint->getBreakingImpulseThreshold();
}

inline void PhysicsConstraint::setBreakingImpulse(float impulse)
{
    _constraint->setBreakingImpulseThreshold(impulse);
}

inline bool PhysicsConstraint::isEnabled() const
{
    return _constraint->isEnabled();
}

inline void PhysicsConstraint::setEnabled(bool enabled)
{
    _constraint->setEnabled(enabled);
}

}
