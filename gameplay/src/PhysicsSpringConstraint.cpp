/**
 * PhysicsSpringConstraint.cpp
 */

#include "PhysicsSpringConstraint.h"

#include "Node.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

PhysicsSpringConstraint::PhysicsSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    // Create a translation matrix that translates to the midpoint
    // between the two physics rigid bodies.
    Matrix m;
    Matrix::createTranslation(midpoint(a->_node, b->_node), &m);

    // Calculate the translation and rotation offsets to the rigid bodies
    // by transforming the translation matrix above into each rigid body's
    // local space (multiply by the inverse world matrix and extract components).
    Matrix mAi;
    a->_node->getWorldMatrix().invert(&mAi);
    mAi.multiply(m);

    Matrix mBi;
    b->_node->getWorldMatrix().invert(&mBi);
    mBi.multiply(m);

    Quaternion rA, rB;
    mAi.getRotation(&rA);
    mBi.getRotation(&rB);
    
    Vector3 tA, tB;
    mAi.getTranslation(&tA);
    mBi.getTranslation(&tB);

    btTransform frameInA(btQuaternion(rA.x, rA.y, rA.z, rA.w), btVector3(tA.x, tA.y, tA.z));
    btTransform frameInB(btQuaternion(rB.x, rB.y, rB.z, rB.w), btVector3(tB.x, tB.y, tB.z));
            
    _constraint = new btGeneric6DofSpringConstraint(*a->_body, *b->_body, frameInA, frameInB, true);
}

PhysicsSpringConstraint::PhysicsSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), 
        btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
    btTransform frameInB(btQuaternion(rotationOffsetB.x, rotationOffsetB.y, rotationOffsetB.z, rotationOffsetB.w), 
        btVector3(translationOffsetB.x, translationOffsetB.y, translationOffsetB.z));
    _constraint = new btGeneric6DofSpringConstraint(*a->_body, *b->_body, frameInA, frameInB, true);
}

void PhysicsSpringConstraint::setStrength(SpringProperty property, float strength)
{
    if (strength < MATH_EPSILON)
        ((btGeneric6DofSpringConstraint*)_constraint)->enableSpring(property, false);
    else
    {
        ((btGeneric6DofSpringConstraint*)_constraint)->enableSpring(property, true);
        ((btGeneric6DofSpringConstraint*)_constraint)->setStiffness(property, strength);
        ((btGeneric6DofSpringConstraint*)_constraint)->setEquilibriumPoint(property);
    }
}

void PhysicsSpringConstraint::setDamping(SpringProperty property, float damping)
{
    ((btGeneric6DofSpringConstraint*)_constraint)->setDamping(property, damping);
    ((btGeneric6DofSpringConstraint*)_constraint)->setEquilibriumPoint(property);
}

}
