#ifndef PHYSICSCONTROLLER_H_
#define PHYSICSCONTROLLER_H_

#include "PhysicsConstraint.h"
#include "PhysicsDebugDrawer.h"
#include "PhysicsFixedConstraint.h"
#include "PhysicsGenericConstraint.h"
#include "PhysicsHingeConstraint.h"
#include "PhysicsSocketConstraint.h"
#include "PhysicsSpringConstraint.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{
    
/**
 * Defines a class for controlling game physics.
 */
class PhysicsController
{
    friend class Game;
    friend class PhysicsConstraint;
    friend class PhysicsRigidBody;

public:

    /**
     * Status listener interface.
     */
    class Listener
    {
    public:

        /**
         * The type of physics status event.
         */
        enum EventType 
        {
            /**
             * Event fired when there were no active physics objects and at least one is now active.
             */
            ACTIVATED,

            /**
             * Event fired when there are no more active physics objects in the world.
             */
            DEACTIVATED
        };

        /**
         * Handles when the physics world status changes.
         */
        virtual void statusEvent(EventType type) = 0;
    };

    /**
     * Adds a status listener.
     * 
     * @param listener The listener to add.
     */
    void addStatusListener(Listener* listener);

    /**
     * Creates a fixed constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsFixedConstraint* createFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b = NULL);

    /**
     * Creates a generic constraint so that the rigid body (or bodies) is
     * (are) constrained to its (their) current world position(s).
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsGenericConstraint* createGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b = NULL);

    /**
     * Creates a generic constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param rotationOffsetA The rotation offset for the first rigid body 
     *      (in its local space) with respect to the constraint joint.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param rotationOffsetB The rotation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsGenericConstraint* createGenericConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
                                                     const Vector3& translationOffsetA, PhysicsRigidBody* b = NULL, 
                                                     const Quaternion& rotationOffsetB = Quaternion(), 
                                                     const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a hinge constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param rotationOffsetA The rotation offset for the first rigid body 
     *      (in its local space) with respect to the constraint joint.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param rotationOffsetB The rotation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsHingeConstraint* createHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
                                                  const Vector3& translationOffsetA, PhysicsRigidBody* b = NULL, 
                                                  const Quaternion& rotationOffsetB = Quaternion(), 
                                                  const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a socket constraint so that the rigid body (or bodies) is
     * (are) constrained using its (their) current world position(s) for
     * the translation offset(s) to the constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsSocketConstraint* createSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b = NULL);

    /**
     * Creates a socket constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsSocketConstraint* createSocketConstraint(PhysicsRigidBody* a, const Vector3& translationOffsetA,
                                                    PhysicsRigidBody* b = NULL, const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a spring constraint so that the rigid body (or bodies) is
     * (are) constrained using its (their) current world position(s) for
     * the translation offset(s) to the constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsSpringConstraint* createSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

    /**
     * Creates a spring constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param rotationOffsetA The rotation offset for the first rigid body 
     *      (in its local space) with respect to the constraint joint.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param rotationOffsetB The rotation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsSpringConstraint* createSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
                                                    const Vector3& translationOffsetA, PhysicsRigidBody* b, 
                                                    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

    /**
     * Draws debugging information (rigid body outlines, etc.) using the given view projection matrix.
     * 
     * @param viewProjection The view projection matrix to use when drawing.
     */
    void drawDebug(const Matrix& viewProjection);

    /**
     * Gets the gravity vector for the simulated physics world.
     * 
     * @return The gravity vector.
     */
    const Vector3& getGravity(const Vector3& gravity) const;

    /**
     * Sets the gravity vector for the simulated physics world.
     * 
     * @param gravity The gravity vector.
     */
    void setGravity(const Vector3& gravity);

private:

    /**
     * Constructor.
     */
    PhysicsController();

    /**
     * Destructor.
     */
    ~PhysicsController();

    /**
     * Controller initialize.
     */
    void initialize();

    /**
     * Controller finalize.
     */
    void finalize();

    /**
     * Controller pause.
     */
    void pause();

    /**
     * Controller resume.
     */
    void resume();

    /**
     * Controller update.
     */
    void update(long elapsedTime);

    // Adds the given rigid body to the world.
    void addRigidBody(PhysicsRigidBody* body);

    // Creates a box collision shape to be used in the creation of a rigid body.
    btCollisionShape* getBox(const Vector3& min, const Vector3& max, const btVector3& scale);

    // Creates a triangle mesh collision shape to be used in the creation of a rigid body.
    btCollisionShape* getMesh(PhysicsRigidBody* body);

    // Gets the corresponding GamePlay object for the given Bullet object.
    PhysicsRigidBody* getPhysicsRigidBody(const btCollisionObject* collisionObject);

    // Creates a sphere collision shape to be used in the creation of a rigid body.
    btCollisionShape* getSphere(float radius, const btVector3& scale);

    // Removes the given constraint from the simulated physics world.
    void removeConstraint(PhysicsConstraint* constraint);

    // Removes the given rigid body from the simulated physics world.
    void removeRigidBody(PhysicsRigidBody* rigidBody);

    // Sets up the given constraint for the given two rigid bodies.
    void setupConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint);
    
    Vector3 _gravity;
    btDefaultCollisionConfiguration* _collisionConfiguration;
    btCollisionDispatcher* _dispatcher;
    btBroadphaseInterface* _overlappingPairCache;
    btSequentialImpulseConstraintSolver* _solver;
    btDynamicsWorld* _world;
    btAlignedObjectArray<btCollisionShape*> _shapes;
    PhysicsDebugDrawer* _drawer;
    Listener::EventType _status;
    std::vector<PhysicsRigidBody*> _bodies;
    std::vector<Listener*>* _listeners;
};

}

#endif
