#ifndef PHYSICSGHOSTOBJECT_H_
#define PHYSICSGHOSTOBJECT_H_

#include "PhysicsCollisionObject.h"
#include "PhysicsRigidBody.h"
#include "Transform.h"

namespace gameplay
{

/**
 * Defines a physics ghost object.
 *
 * It is a collision volume that does not participate in the physics
 * simulation but can be used the test against other phyics collision objects.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-Collision_Objects
 */
class PhysicsGhostObject : public PhysicsCollisionObject, public Transform::Listener
{
    friend class Node;
    friend class PhysicsController;

public:

    /**
     * @see PhysicsCollisionObject::getType
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
     * @param group Group identifier
     * @param mask Bitmask field for filtering collisions with this object.
     */
    PhysicsGhostObject(Node* node, const PhysicsCollisionShape::Definition& shape, int group = PHYSICS_COLLISION_GROUP_DEFAULT, int mask = PHYSICS_COLLISION_MASK_DEFAULT);

    /**
     * Destructor.
     */
    virtual ~PhysicsGhostObject();

    /**
     * Creates a ghost object from the specified properties object.
     * 
     * @param node The node to create a ghost object for; note that the node must have
     *      a model attached to it prior to creating a ghost object for it.
     * @param properties The properties object defining the ghost object (must have namespace equal to 'ghost').
     * @return The newly created ghost object, or <code>NULL</code> if the ghost object failed to load.
     */
    static PhysicsGhostObject* create(Node* node, Properties* properties);

    /**
     * Pointer to the Bullet ghost collision object.
     */
    btPairCachingGhostObject* _ghostObject;
};

}

#endif
