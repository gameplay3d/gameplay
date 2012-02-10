#include "PhysicsHingeConstraint.h"

#include "Node.h"

namespace gameplay
{

void PhysicsHingeConstraint::setLimits(float minAngle, float maxAngle, float bounciness)
{
    // Use the defaults for softness (0.9) and biasFactor (0.3).
    ((btHingeConstraint*)_constraint)->setLimit(minAngle, maxAngle, 0.9f, 0.3f, bounciness);
}

PhysicsHingeConstraint::PhysicsHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
    : PhysicsConstraint(a, b)
{
    // Take scale into account for the first node's translation offset.
    Vector3 sA;
    a->getNode()->getWorldMatrix().getScale(&sA);
    Vector3 tA(translationOffsetA.x * sA.x, translationOffsetA.y * sA.y, translationOffsetA.z * sA.z);

    if (b)
    {
        // Take scale into account for the second node's translation offset.
        Vector3 sB;
        b->getNode()->getWorldMatrix().getScale(&sB);
        Vector3 tB(translationOffsetB.x * sB.x, translationOffsetB.y * sB.y, translationOffsetB.z * sB.z);

        btTransform frameInA(BQ(rotationOffsetA), BV(tA));
        btTransform frameInB(BQ(rotationOffsetB), BV(tB));
        _constraint = new btHingeConstraint(*a->_body, *b->_body, frameInA, frameInB);
    }
    else
    {
        btTransform frameInA(BQ(rotationOffsetA), BV(tA));
        _constraint = new btHingeConstraint(*a->_body, frameInA);
    }
}

    
PhysicsHingeConstraint::~PhysicsHingeConstraint()
{
    // Unused
}

}
