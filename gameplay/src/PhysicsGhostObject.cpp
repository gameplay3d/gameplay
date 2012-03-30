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
