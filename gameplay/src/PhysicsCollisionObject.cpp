#include "Base.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "Game.h"

namespace gameplay
{

// Internal class used to implement the collidesWith(PhysicsCollisionObject*) function.
struct CollidesWithCallback : public btCollisionWorld::ContactResultCallback
{
    btScalar addSingleResult(btManifoldPoint& cp, 
        const btCollisionObject* a, int partIdA, int indexA, 
        const btCollisionObject* b, int partIdB, int indexB)
    {
        result = true;
        return 0.0f;
    }

    bool result;
};

PhysicsCollisionObject::PhysicsCollisionObject(Node* node)
    : _node(node), _motionState(NULL), _collisionShape(NULL)
{
}

PhysicsCollisionObject::~PhysicsCollisionObject()
{
    SAFE_DELETE(_motionState);

    Game::getInstance()->getPhysicsController()->destroyShape(_collisionShape);
}

PhysicsCollisionShape::Type PhysicsCollisionObject::getShapeType() const
{
    return getCollisionShape()->getType();
}

Node* PhysicsCollisionObject::getNode() const
{
    return _node;
}

PhysicsCollisionShape* PhysicsCollisionObject::getCollisionShape() const
{
    return _collisionShape;
}

PhysicsMotionState* PhysicsCollisionObject::getMotionState() const
{
    return _motionState;
}

bool PhysicsCollisionObject::isKinematic() const
{
    return getCollisionObject()->isKinematicObject();
}

bool PhysicsCollisionObject::isDynamic() const
{
    return !getCollisionObject()->isStaticOrKinematicObject();
}

void PhysicsCollisionObject::addCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    Game::getInstance()->getPhysicsController()->addCollisionListener(listener, this, object);
}

void PhysicsCollisionObject::removeCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    Game::getInstance()->getPhysicsController()->removeCollisionListener(listener, this, object);
}

bool PhysicsCollisionObject::collidesWith(PhysicsCollisionObject* object) const
{
    static CollidesWithCallback callback;

    callback.result = false;
    Game::getInstance()->getPhysicsController()->_world->contactPairTest(getCollisionObject(), object->getCollisionObject(), callback);
    return callback.result;
}

PhysicsCollisionObject::CollisionPair::CollisionPair(PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
    : objectA(objectA), objectB(objectB)
{
    // unused
}

bool PhysicsCollisionObject::CollisionPair::operator < (const CollisionPair& collisionPair) const
{
    // If the pairs are equal, then return false.
    if ((objectA == collisionPair.objectA && objectB == collisionPair.objectB) || (objectA == collisionPair.objectB && objectB == collisionPair.objectA))
        return false;

    // We choose to compare based on objectA arbitrarily.
    if (objectA < collisionPair.objectA)
        return true;

    if (objectA == collisionPair.objectA)
        return objectB < collisionPair.objectB;

    return false;
}

}
