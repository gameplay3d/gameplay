/**
 * PhysicsSocketConstraint.cpp
 */

#include "PhysicsSocketConstraint.h"

#include "Node.h"

namespace gameplay
{

PhysicsSocketConstraint::PhysicsSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    if (b)
    {
        // Create a translation matrix that translates to the midpoint
        // between the two physics rigid bodies.
        Matrix m;
        Matrix::createTranslation(midpoint(a->_node, b->_node), &m);

        // Calculate the translation offsets to the rigid bodies
        // by transforming the translation matrix above into each rigid body's
        // local space (multiply by the inverse world matrix and extract translation).
        Matrix mAi;
        a->_node->getWorldMatrix().invert(&mAi);
        mAi.multiply(m);

        Matrix mBi;
        b->_node->getWorldMatrix().invert(&mBi);
        mBi.multiply(m);
    
        Vector3 tA, tB;
        mAi.getTranslation(&tA);
        mBi.getTranslation(&tB);

        _constraint = new btPoint2PointConstraint(*a->_body, *b->_body, btVector3(tA.x, tA.y, tA.z), btVector3(tB.x, tB.y, tB.z));
    }
    else
    {
        _constraint = new btPoint2PointConstraint(*a->_body, btVector3(0.0f, 0.0f, 0.0f));
    }
}

PhysicsSocketConstraint::PhysicsSocketConstraint(PhysicsRigidBody* a, const Vector3& translationOffsetA, 
    PhysicsRigidBody* b, const Vector3& translationOffsetB)
{
    if (b)
    {
        _constraint = new btPoint2PointConstraint(*a->_body, *b->_body, 
            btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z),
            btVector3(translationOffsetB.x, translationOffsetB.y, translationOffsetB.z));
    }
    else
    {
        _constraint = new btPoint2PointConstraint(*a->_body, 
            btVector3(translationOffsetA.x, translationOffsetA.y, translationOffsetA.z));
    }
}

PhysicsSocketConstraint::~PhysicsSocketConstraint()
{
    // DUMMY FUNCTION
}

}
