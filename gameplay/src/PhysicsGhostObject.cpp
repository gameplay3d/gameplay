#include "Base.h"
#include "Game.h"
#include "PhysicsGhostObject.h"
#include "Vector3.h"

namespace gameplay
{

PhysicsGhostObject::PhysicsGhostObject(Node* node, PhysicsRigidBody::ShapeType type)
    : _node(node), _motionState(NULL), _shape(NULL), _ghostObject(NULL)
{
    _node->addListener(this);

    // Create the ghost object.
    _ghostObject = bullet_new<btPairCachingGhostObject>();

    // Get the node's world scale (we need to apply this during creation since ghost objects don't scale dynamically).
    Vector3 scale;
    _node->getWorldMatrix().getScale(&scale);

    // Use the center of the bounding sphere as the center of mass offset.
    Vector3 c(_node->getModel()->getMesh()->getBoundingSphere().center);
    c.x *= scale.x;
    c.y *= scale.y;
    c.z *= scale.z;
        
    // Initialize a physics motion state object for syncing the transform.
    Vector3 centerOfMassOffset(-c);
    _motionState = new PhysicsMotionState(_node, &centerOfMassOffset);
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());

    // Create and set the collision shape for the ghost object.
    PhysicsController* physicsController = Game::getInstance()->getPhysicsController();
    switch (type)
    {
        case PhysicsRigidBody::SHAPE_BOX:
        {
            const BoundingBox& box = node->getModel()->getMesh()->getBoundingBox();
            _shape = physicsController->createBox(box.min, box.max, scale);
            break;
        }
        case PhysicsRigidBody::SHAPE_SPHERE:
        {
            const BoundingSphere& sphere = node->getModel()->getMesh()->getBoundingSphere();
            _shape = physicsController->createSphere(sphere.radius, scale);
            break;
        }
    }
    _ghostObject->setCollisionShape(_shape);

    // Add the ghost object to the physics world.
    physicsController->addCollisionObject(this);
}

PhysicsGhostObject::~PhysicsGhostObject()
{
    Game::getInstance()->getPhysicsController()->removeCollisionObject(this);

    _shape = NULL;
    SAFE_DELETE(_ghostObject);
    SAFE_DELETE(_motionState);
}

PhysicsCollisionObject::Type PhysicsGhostObject::getType() const
{
    return GHOST_OBJECT;
}

Node* PhysicsGhostObject::getNode() const
{
    return _node;
}

btCollisionObject* PhysicsGhostObject::getCollisionObject() const
{
    return _ghostObject;
}

btCollisionShape* PhysicsGhostObject::getCollisionShape() const
{
    return _shape;
}

void PhysicsGhostObject::transformChanged(Transform* transform, long cookie)
{
    // Update the motion state with the transform from the node.
    _motionState->updateTransformFromNode();

    // Update the transform on the ghost object.
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());
}

}
