#include "Base.h"
#include "PhysicsConstraint.h"
#include "Game.h"
#include "Node.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

PhysicsConstraint::PhysicsConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
    : _a(a), _b(b), _constraint(NULL)
{
}

PhysicsConstraint::~PhysicsConstraint()
{
    // Remove the physics rigid bodies' references to this constraint.
    if (_a)
        _a->removeConstraint(this);
    if (_b)
        _b->removeConstraint(this);

    // Remove the constraint from the physics world and delete the Bullet object.
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->removeConstraint(this);
    SAFE_DELETE(_constraint);
}

Vector3 PhysicsConstraint::centerOfMassMidpoint(const Node* a, const Node* b)
{
    GP_ASSERT(a);
    GP_ASSERT(b);

    Vector3 tA, tB;
    a->getWorldMatrix().getTranslation(&tA);
    b->getWorldMatrix().getTranslation(&tB);

    tA = getWorldCenterOfMass(a);
    tB = getWorldCenterOfMass(b);
    
    Vector3 d(tA, tB);
    d.scale(0.5f);
    Vector3 c(tA);
    c.add(d);

    return c;
}

Quaternion PhysicsConstraint::getRotationOffset(const Node* node, const Vector3& point)
{
    GP_ASSERT(node);

    // Create a translation matrix that translates to the given origin.
    Matrix m;
    Matrix::createTranslation(point, &m);

    // Calculate the rotation offset to the rigid body by transforming 
    // the translation matrix above into the rigid body's local space 
    // (multiply by the inverse world matrix) and extracting the rotation.
    Matrix mi;
    node->getWorldMatrix().invert(&mi);
    mi.multiply(m);
    
    Quaternion r;
    mi.getRotation(&r);

    return r;
}

Vector3 PhysicsConstraint::getTranslationOffset(const Node* node, const Vector3& point)
{
    GP_ASSERT(node);

    // Create a translation matrix that translates to the given origin.
    Matrix m;
    Matrix::createTranslation(point, &m);

    // Calculate the translation offset to the rigid body by transforming 
    // the translation matrix above into the rigid body's local space 
    // (multiply by the inverse world matrix) and extracting the translation.
    Matrix mi;
    node->getWorldMatrix().invert(&mi);
    mi.multiply(m);
    
    Vector3 t;
    mi.getTranslation(&t);

    Vector3 s;
    node->getWorldMatrix().getScale(&s);

    t.x *= s.x;
    t.y *= s.y;
    t.z *= s.z;
    
    t = offsetByCenterOfMass(node, t);

    return t;
}

btTransform PhysicsConstraint::getTransformOffset(const Node* node, const Vector3& origin)
{
    GP_ASSERT(node);

    // Create a translation matrix that translates to the given origin.
    Matrix m;
    Matrix::createTranslation(origin, &m);

    // Calculate the translation and rotation offset to the rigid body
    // by transforming the translation matrix above into the rigid body's
    // local space (multiply by the inverse world matrix and extract components).
    Matrix mi;
    node->getWorldMatrix().invert(&mi);
    mi.multiply(m);

    Quaternion r;
    mi.getRotation(&r);
    
    Vector3 t;
    mi.getTranslation(&t);

    Vector3 s;
    node->getWorldMatrix().getScale(&s);

    t.x *= s.x;
    t.y *= s.y;
    t.z *= s.z;
    
    t = offsetByCenterOfMass(node, t);

    return btTransform(BQ(r), BV(t));
}

Vector3 PhysicsConstraint::getWorldCenterOfMass(const Node* node)
{
    GP_ASSERT(node);

    const BoundingSphere& sphere = node->getBoundingSphere();
    if (!(sphere.center.isZero() && sphere.radius == 0))
    {
        // The world-space center of mass is the sphere's center.
        return sphere.center;
    }

    // Warn the user that the node has no bounding volume.
    GP_WARN("Node %s' has no bounding volume - center of mass is defaulting to local coordinate origin.", node->getId());

    Vector3 center;
    node->getWorldMatrix().transformPoint(&center);
    return center;
}

Vector3 PhysicsConstraint::offsetByCenterOfMass(const Node* node, const Vector3& v)
{
    GP_ASSERT(node && node->getCollisionObject() && node->getCollisionObject()->_motionState);
    btVector3 centerOfMassOffset = node->getCollisionObject()->_motionState->_centerOfMassOffset.getOrigin();
    return Vector3(v.x + centerOfMassOffset.x(), v.y + centerOfMassOffset.y(), v.z + centerOfMassOffset.z());
}

}
