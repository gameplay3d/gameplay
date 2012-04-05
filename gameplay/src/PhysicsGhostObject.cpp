#include "Base.h"
#include "PhysicsGhostObject.h"
#include "Node.h"
#include "Game.h"

namespace gameplay
{

PhysicsGhostObject::PhysicsGhostObject(Node* node, const PhysicsCollisionShape::Definition& shape)
    : PhysicsCollisionObject(node), _ghostObject(NULL)
{
    Vector3 centerOfMassOffset;
    PhysicsController* physicsController = Game::getInstance()->getPhysicsController();

    // Create and set the collision shape for the ghost object.
    _collisionShape = physicsController->createShape(node, shape, &centerOfMassOffset);

    // Create the ghost object.
    _ghostObject = bullet_new<btPairCachingGhostObject>();
	_ghostObject->setCollisionShape(_collisionShape->getShape());
    _ghostObject->setCollisionFlags(_ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Initialize a physics motion state object for syncing the transform.
    _motionState = new PhysicsMotionState(_node, &centerOfMassOffset);
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());

    // Add the ghost object to the physics world.
    physicsController->addCollisionObject(this);

    _node->addListener(this);
}

PhysicsGhostObject::~PhysicsGhostObject()
{
    _node->removeListener(this);

    Game::getInstance()->getPhysicsController()->removeCollisionObject(this);

    SAFE_DELETE(_ghostObject);
}

PhysicsGhostObject* PhysicsGhostObject::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    assert(properties);
    if (!properties || !(strcmp(properties->getNamespace(), "ghost") == 0))
    {
        WARN("Failed to load ghost object from properties object: must be non-null object and have namespace equal to \'ghost\'.");
        return NULL;
    }

    // Load the physics collision shape definition.
    PhysicsCollisionShape::Definition* shape = PhysicsCollisionShape::Definition::create(node, properties);
    if (shape == NULL)
    {
        WARN("Failed to create collision shape during ghost object creation.");
        return NULL;
    }

    // Create the ghost object.
    PhysicsGhostObject* ghost = new PhysicsGhostObject(node, *shape);
    SAFE_DELETE(shape);

    return ghost;
}

PhysicsCollisionObject::Type PhysicsGhostObject::getType() const
{
    return GHOST_OBJECT;
}

btCollisionObject* PhysicsGhostObject::getCollisionObject() const
{
    return _ghostObject;
}

void PhysicsGhostObject::transformChanged(Transform* transform, long cookie)
{
    // Update the motion state with the transform from the node.
    _motionState->updateTransformFromNode();

    // Update the transform on the ghost object.
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());
}

}
