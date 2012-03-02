#ifndef PHYSICSCOLLISIONOBJECT_H_
#define PHYSICSCOLLISIONOBJECT_H_

#include "Vector3.h"

namespace gameplay
{

class Node;

/**
 * Base class for all gameplay physics objects that support collision events.
 */
class PhysicsCollisionObject
{
    friend class PhysicsController;

public:

    /**
     * Enumeration of all possible collision object types.
     */
    enum Type
    {
        /**
         * PhysicsRigidBody type.
         */
        RIGID_BODY,

        /**
         * PhysicsCharacter type.
         */
        CHARACTER
    };

    /** 
     * Defines a pair of rigid bodies that collided (or may collide).
     */
    class CollisionPair
    {
    public:

        /**
         * Constructor.
         */
        CollisionPair(PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB);

        /**
         * Less than operator (needed for use as a key in map).
         * 
         * @param collisionPair The collision pair to compare.
         * @return True if this pair is "less than" the given pair; false otherwise.
         */
        bool operator < (const CollisionPair& collisionPair) const;

        /**
         * The first object in the collision.
         */
        PhysicsCollisionObject* objectA;

        /**
         * The second object in the collision.
         */
        PhysicsCollisionObject* objectB;
    };

    /**
     * Collision listener interface.
     */
    class CollisionListener
    {
        friend class PhysicsCollisionObject;
        friend class PhysicsController;

    public:

        /**
         * The type of collision event.
         */
        enum EventType
        {
            /**
             * Event fired when the two rigid bodies start colliding.
             */
            COLLIDING,

            /**
             * Event fired when the two rigid bodies no longer collide.
             */
            NOT_COLLIDING
        };

        /**
         * Virtual destructor.
         */
        virtual ~CollisionListener() { }

        /**
         * Called when a collision occurs between two objects in the physics world.
         * 
         * @param type The type of collision event.
         * @param collisionPair The two collision objects involved in the collision.
         * @param contactPointA The contact point with the first object (in world space).
         * @param contactPointB The contact point with the second object (in world space).
         */
        virtual void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
                                    const PhysicsCollisionObject::CollisionPair& collisionPair,
                                    const Vector3& contactPointA = Vector3::zero(),
                                    const Vector3& contactPointB = Vector3::zero()) = 0;
    };

    /**
     * Returns the type of the collision object.
     */
    virtual PhysicsCollisionObject::Type getType() const = 0;

    /**
     * Returns the node associated with this collision object.
     */
    virtual Node* getNode() const = 0;

    /**
     * Adds a collision listener for this collision object.
     * 
     * @param listener The listener to add.
     * @param object Optional collision object used to filter the collision event.
     */
    void addCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object = NULL);

    /**
     * Removes a collision listener.
     *
     * @param listener The listener to remove.
     */
    void removeCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object = NULL);

    /**
     * Checks if this collision object collides with the given object.
     * 
     * @param object The collision object to test for collision with.
     * 
     * @return True if this object collides with the specified one; false otherwise.
     */
    bool collidesWith(PhysicsCollisionObject* object) const;

protected:

    /**
     * Constructor.
     */
    PhysicsCollisionObject();

    /**
     * Virtual destructor.
     */
    virtual ~PhysicsCollisionObject();

    /**
     * Returns the Bullet Physics collision object.
     *
     * @return The Bullet collision object.
     */
    virtual btCollisionObject* getCollisionObject() const = 0;

    /**
     * Returns the Bullet Physics collision shape.
     *
     * @return The Bullet collision shape.
     */
    virtual btCollisionShape* getCollisionShape() const = 0;

private:

    // Internal class used to implement the collidesWith(PhysicsRigidBody*) function.
    struct CollidesWithCallback : public btCollisionWorld::ContactResultCallback
    {
        btScalar addSingleResult(btManifoldPoint& cp, 
                                 const btCollisionObject* a, int partIdA, int indexA, 
                                 const btCollisionObject* b, int partIdB, int indexB);

        bool result;
    };

};

}

#endif
