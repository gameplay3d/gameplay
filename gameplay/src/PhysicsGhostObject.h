#ifndef PHYSICSGHOSTOBJECT_H_
#define PHYSICSGHOSTOBJECT_H_

#include "PhysicsCollisionObject.h"
#include "PhysicsRigidBody.h"
#include "Transform.h"

namespace gameplay
{

class PhysicsMotionState;

/**
 * Defines a class for physics ghost objects.
 */
class PhysicsGhostObject : public PhysicsCollisionObject, public Transform::Listener
{
    friend class Node;
    friend class PhysicsController;

public:

    /**
     * @see PhysicsCollisionObject#getType
     */
    PhysicsCollisionObject::Type getType() const;

    /**
     * @see PhysicsCollisionObject#getNode
     */
    Node* getNode() const;

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

    /**
     * @see PhysicsCollisionObject::getCollisionShape
     */
    btCollisionShape* getCollisionShape() const;

private:

    /**
     * Constructor.
     * 
     * @param node The node to attach the ghost object to.
     * @param type The type of ghost object (collision shape type).
     */
    PhysicsGhostObject(Node* node, PhysicsRigidBody::ShapeType type);
    
    /**
     * Destructor.
     */
    ~PhysicsGhostObject();

    /**
     * Used to synchronize the transform between GamePlay and Bullet.
     */
    void transformChanged(Transform* transform, long cookie);

    Node* _node;
    PhysicsMotionState* _motionState;
    btCollisionShape* _shape;
    btGhostObject* _ghostObject;
};

}

#endif