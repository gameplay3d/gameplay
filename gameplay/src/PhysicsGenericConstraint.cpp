#include "PhysicsGenericConstraint.h"

#include "Node.h"
#include "PhysicsMotionState.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

PhysicsGenericConstraint::PhysicsGenericConstraint()
    : PhysicsConstraint(NULL, NULL), _rotationOffsetA(NULL), _rotationOffsetB(NULL),
    _translationOffsetA(NULL), _translationOffsetB(NULL)
{
    // Not used.
}

PhysicsGenericConstraint::PhysicsGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
    : PhysicsConstraint(a, b), _rotationOffsetA(NULL), _rotationOffsetB(NULL),
    _translationOffsetA(NULL), _translationOffsetB(NULL)
{
    if (b)
    {
        Vector3 origin = centerOfMassMidpoint(a->getNode(), b->getNode());
        _constraint = new btGeneric6DofConstraint(*a->_body, *b->_body, getTransformOffset(a->getNode(), origin), getTransformOffset(b->getNode(), origin), true);
    }
    else
    {
        _constraint = new btGeneric6DofConstraint(*a->_body, btTransform::getIdentity(), true);
    }
}

PhysicsGenericConstraint::PhysicsGenericConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
    : PhysicsConstraint(a, b), _rotationOffsetA(NULL), _rotationOffsetB(NULL), _translationOffsetA(NULL), _translationOffsetB(NULL)
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

        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), btVector3(tA.x, tA.y, tA.z));
        btTransform frameInB(btQuaternion(rotationOffsetB.x, rotationOffsetB.y, rotationOffsetB.z, rotationOffsetB.w), btVector3(tB.x, tB.y, tB.z));
        _constraint = new btGeneric6DofConstraint(*a->_body, *b->_body, frameInA, frameInB, true);
    }
    else
    {
        btTransform frameInA(btQuaternion(rotationOffsetA.x, rotationOffsetA.y, rotationOffsetA.z, rotationOffsetA.w), btVector3(tA.x, tA.y, tA.z));
        _constraint = new btGeneric6DofConstraint(*a->_body, frameInA, true);
    }
}

PhysicsGenericConstraint::~PhysicsGenericConstraint()
{
    SAFE_DELETE(_rotationOffsetA);
    SAFE_DELETE(_rotationOffsetB);
    SAFE_DELETE(_translationOffsetA);
    SAFE_DELETE(_translationOffsetB);
}

}
