/**
 * PhysicsSocketConstraint.cpp
 */

#include "PhysicsSocketConstraint.h"

#include "Node.h"

namespace gameplay
{

PhysicsSocketConstraint::PhysicsSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
    : PhysicsConstraint(a, b)
{
    if (b)
    {
        Vector3 origin = centerOfMassMidpoint(a->_node, b->_node);
        btTransform frameInA = getTransformOffset(a->_node, origin);
        btTransform frameInB = getTransformOffset(b->_node, origin);

        _constraint = new btPoint2PointConstraint(*a->_body, *b->_body, frameInA.getOrigin(), frameInB.getOrigin());
    }
    else
    {
        _constraint = new btPoint2PointConstraint(*a->_body, btVector3(0.0f, 0.0f, 0.0f));
    }
}

PhysicsSocketConstraint::PhysicsSocketConstraint(PhysicsRigidBody* a, const Vector3& translationOffsetA, 
    PhysicsRigidBody* b, const Vector3& translationOffsetB)
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

        _constraint = new btPoint2PointConstraint(*a->_body, *b->_body, btVector3(tA.x, tA.y, tA.z), btVector3(tB.x, tB.y, tB.z));
    }
    else
    {
        _constraint = new btPoint2PointConstraint(*a->_body, btVector3(tA.x, tA.y, tA.z));
    }
}

PhysicsSocketConstraint::~PhysicsSocketConstraint()
{
    // DUMMY FUNCTION
}

}
