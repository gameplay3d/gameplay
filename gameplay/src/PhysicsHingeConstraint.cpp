/*
 * PhysicsHingeConstraint.cpp
 */

#include "PhysicsHingeConstraint.h"

namespace gameplay
{

void PhysicsHingeConstraint::setLimits(float minAngle, float maxAngle, float softness, float biasFactor, float relaxationFactor)
{
    ((btHingeConstraint*)_constraint)->setLimit(minAngle, maxAngle, softness, biasFactor, relaxationFactor);
}

PhysicsHingeConstraint::PhysicsHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    if (b)
    {
        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), 
            btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
        btTransform frameInB(btQuaternion(rotationOffsetB.x, rotationOffsetB.y, rotationOffsetB.z, rotationOffsetB.w), 
            btVector3(translationOffsetB.x, translationOffsetB.y, translationOffsetB.z));
        _constraint = new btHingeConstraint(*a->_body, *b->_body, frameInA, frameInB);
    }
    else
    {
        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), 
            btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
        _constraint = new btHingeConstraint(*a->_body, frameInA);
    }
}

    
PhysicsHingeConstraint::~PhysicsHingeConstraint()
{
    // DUMMY FUNCTION
}

}
