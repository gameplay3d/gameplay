#include "PhysicsConstraint.h"

namespace gameplay
{

inline float PhysicsConstraint::getBreakingImpulse() const
{
    GP_ASSERT(_constraint);
    return _constraint->getBreakingImpulseThreshold();
}

inline void PhysicsConstraint::setBreakingImpulse(float impulse)
{
    GP_ASSERT(_constraint);
    _constraint->setBreakingImpulseThreshold(impulse);
}

inline bool PhysicsConstraint::isEnabled() const
{
    GP_ASSERT(_constraint);
    return _constraint->isEnabled();
}

inline void PhysicsConstraint::setEnabled(bool enabled)
{
    GP_ASSERT(_constraint);
    _constraint->setEnabled(enabled);
}

}
