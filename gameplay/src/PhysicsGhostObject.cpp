#include "Base.h"
#include "PhysicsGhostObject.h"
#include "Node.h"
#include "Game.h"

namespace gameplay
{

PhysicsGhostObject::PhysicsGhostObject(Node* node, const PhysicsCollisionShape::Definition& shape, int group, int mask)
    : PhysicsCollisionObject(node, group, mask), _ghostObject(NULL)
{
    Vector3 centerOfMassOffset;
    PhysicsController* physicsController = Game::getInstance()->getPhysicsController();
    GP_ASSERT(physicsController);

    // Create and set the collision shape for the ghost object.
    _collisionShape = physicsController->createShape(node, shape, &centerOfMassOffset, false);
    GP_ASSERT(_collisionShape);

    // Create the ghost object.
    _ghostObject = bullet_new<btPairCachingGhostObject>();
    _ghostObject->setCollisionShape(_collisionShape->getShape());
    _ghostObject->setCollisionFlags(_ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Initialize a physics motion state object for syncing the transform.
    _motionState = new PhysicsMotionState(_node, this, &centerOfMassOffset);
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());

    // Add the ghost object to the physics world.
    physicsController->addCollisionObject(this);

    GP_ASSERT(_node);
    _node->addListener(this);
}

PhysicsGhostObject::~PhysicsGhostObject()
{
    GP_ASSERT(_node);
    _node->removeListener(this);

    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->removeCollisionObject(this, true);

    SAFE_DELETE(_ghostObject);
}

PhysicsGhostObject* PhysicsGhostObject::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    if (!properties || !(strcmp(properties->getNamespace(), "collisionObject") == 0))
    {
        GP_ERROR("Failed to load ghost object from properties object: must be non-null object and have namespace equal to 'collisionObject'.");
        return NULL;
    }

    // Check that the type is specified and correct.
    const char* type = properties->getString("type");
    if (!type)
    {
        GP_ERROR("Failed to load ghost object from properties object; required attribute 'type' is missing.");
        return NULL;
    }
    if (strcmp(type, "GHOST_OBJECT") != 0)
    {
        GP_ERROR("Failed to load ghost object from properties object; attribute 'type' must be equal to 'GHOST_OBJECT'.");
        return NULL;
    }

    // Load the physics collision shape definition.
    PhysicsCollisionShape::Definition shape = PhysicsCollisionShape::Definition::create(node, properties);
    if (shape.isEmpty())
    {
        GP_ERROR("Failed to create collision shape during ghost object creation.");
        return NULL;
    }

    // Create the ghost object.
    PhysicsGhostObject* ghost = new PhysicsGhostObject(node, shape);

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
    GP_ASSERT(_motionState);
    GP_ASSERT(_ghostObject);

    // Update the motion state with the transform from the node.
    _motionState->updateTransformFromNode();

    // Update the transform on the ghost object.
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());
}

}
