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

public:

    /**
     * @see PhysicsCollisionObject#getType
     */
    PhysicsCollisionObject::Type getType() const;

	/**
     * Used to synchronize the transform between GamePlay and Bullet.
     */
    void transformChanged(Transform* transform, long cookie);

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

protected:

    /**
     * Constructor.
     * 
     * @param node The node to attach the ghost object to.
     * @param shape The collision shape definition for the ghost object.
     */
	PhysicsGhostObject(Node* node, const PhysicsCollisionShape::Definition& shape);

    /**
     * Destructor.
     */
    virtual ~PhysicsGhostObject();

    btPairCachingGhostObject* _ghostObject;
};

}

#endif
