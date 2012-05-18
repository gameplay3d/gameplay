#include "Base.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "Game.h"

namespace gameplay
{

/**
 * Internal class used to implement the collidesWith(PhysicsCollisionObject*) function.
 */
struct CollidesWithCallback : public btCollisionWorld::ContactResultCallback
{
    /**
     * Called with each contact. Needed to implement collidesWith(PhysicsCollisionObject*).
     */
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* a, int partIdA, int indexA, const btCollisionObject* b, int partIdB, int indexB)
    {
        result = true;
        return 0.0f;
    }

    /**
     * The result of the callback.
     */
    bool result;
};

PhysicsCollisionObject::PhysicsCollisionObject(Node* node)
    : _node(node), _motionState(NULL), _collisionShape(NULL), _enabled(true)
{
}

PhysicsCollisionObject::~PhysicsCollisionObject()
{
    SAFE_DELETE(_motionState);

    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->destroyShape(_collisionShape);
}

PhysicsCollisionShape::Type PhysicsCollisionObject::getShapeType() const
{
    GP_ASSERT(getCollisionShape());
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
    switch (getType())
    {
    case GHOST_OBJECT:
    case CHARACTER:
        return true;
    default:
        GP_ASSERT(getCollisionObject());
        return getCollisionObject()->isKinematicObject();
    }
}

bool PhysicsCollisionObject::isDynamic() const
{
    GP_ASSERT(getCollisionObject());
    return !getCollisionObject()->isStaticOrKinematicObject();
}

bool PhysicsCollisionObject::isEnabled() const
{
    GP_ASSERT(getCollisionObject());
    return _enabled;
}

void PhysicsCollisionObject::setEnabled(bool enable)
{
    GP_ASSERT(getCollisionObject());
    if (enable)
    {  
        if (!_enabled)
        {
            Game::getInstance()->getPhysicsController()->addCollisionObject(this);
            _enabled = true;
        }
    }
    else
    {
        if (_enabled)
        {
            Game::getInstance()->getPhysicsController()->removeCollisionObject(this);
            _enabled = false;
        }
    }
}

void PhysicsCollisionObject::addCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->addCollisionListener(listener, this, object);
}

void PhysicsCollisionObject::removeCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->removeCollisionListener(listener, this, object);
}

bool PhysicsCollisionObject::collidesWith(PhysicsCollisionObject* object) const
{
    GP_ASSERT(Game::getInstance()->getPhysicsController() && Game::getInstance()->getPhysicsController()->_world);
    GP_ASSERT(object && object->getCollisionObject());
    GP_ASSERT(getCollisionObject());

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
