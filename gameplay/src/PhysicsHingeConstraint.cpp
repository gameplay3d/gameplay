/*
 * PhysicsHingeConstraint.cpp
 */

#include "PhysicsHingeConstraint.h"

#include "Node.h"

namespace gameplay
{

void PhysicsHingeConstraint::setLimits(float minAngle, float maxAngle, float softness, float biasFactor, float relaxationFactor)
{
    ((btHingeConstraint*)_constraint)->setLimit(minAngle, maxAngle, softness, biasFactor, relaxationFactor);
}

PhysicsHingeConstraint::PhysicsHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
    : PhysicsConstraint(a, b)
{
    // Take scale into account for the first node's translation offset.
    Vector3 sA;
    a->_node->getWorldMatrix().getScale(&sA);
    Vector3 tA(translationOffsetA.x * sA.x, translationOffsetA.y * sA.y, translationOffsetA.z * sA.z);

    if (b)
    {
        // Take scale into account for the second node's translation offset.
        Vector3 sB;
        b->_node->getWorldMatrix().getScale(&sB);
        Vector3 tB(translationOffsetB.x * sB.x, translationOffsetB.y * sB.y, translationOffsetB.z * sB.z);

        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), btVector3(tA.x, tA.y, tA.z));
        btTransform frameInB(btQuaternion(rotationOffsetB.x, rotationOffsetB.y, rotationOffsetB.z, rotationOffsetB.w), btVector3(tB.x, tB.y, tB.z));
        _constraint = new btHingeConstraint(*a->_body, *b->_body, frameInA, frameInB);
    }
    else
    {
        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), btVector3(tA.x, tA.y, tA.z));
        _constraint = new btHingeConstraint(*a->_body, frameInA);
    }
}

    
PhysicsHingeConstraint::~PhysicsHingeConstraint()
{
    // DUMMY FUNCTION
}

}
