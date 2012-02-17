#ifndef PHYSICSCONTROLLER_H_
#define PHYSICSCONTROLLER_H_

#include "PhysicsConstraint.h"
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
class PhysicsController : public btCollisionWorld::ContactResultCallback
{
    friend class Game;
    friend class PhysicsConstraint;
    friend class PhysicsRigidBody;
    friend class PhysicsCharacter;

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
         * Handles when a physics world status event occurs.
         */
        virtual void statusEvent(EventType type) = 0;
    };

    /**
     * Adds a listener to the physics controller.
     * 
     * @param listener The listener to add.
     */
    void addStatusListener(PhysicsController::Listener* listener);

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
    PhysicsGenericConstraint* createGenericConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, 
                                                      PhysicsRigidBody* b = NULL, const Quaternion& rotationOffsetB = Quaternion(), const Vector3& translationOffsetB = Vector3());

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
    PhysicsHingeConstraint* createHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
                                                  PhysicsRigidBody* b = NULL, const Quaternion& rotationOffsetB = Quaternion(), const Vector3& translationOffsetB = Vector3());

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
    PhysicsSpringConstraint* createSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,          
                                                    PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

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
   
    /**
     * Draws debugging information (rigid body outlines, etc.) using the given view projection matrix.
     * 
     * @param viewProjection The view projection matrix to use when drawing.
     */
    void drawDebug(const Matrix& viewProjection);

    /**
     * Gets the first rigid body that the given ray intersects.
     * 
     * @param ray The ray to test intersection with.
     * @param hitPoint Optional Vector3 point that is populated with the world-space point of intersection.
     * @param hitDistance Optional float pointer that is populated with the distance along the ray
     *      (as a fraction between 0-1) where the intersection occurred.
     *
     * @return The first rigid body that the ray intersects, or NULL if no intersection was found.
     */
    PhysicsRigidBody* rayTest(const Ray& ray, Vector3* hitPoint = NULL, float* hitFraction = NULL);

protected:

    /**
     * Internal function used for Bullet integration (do not use or override).
     */
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* a, int partIdA, int indexA, const btCollisionObject* b, int partIdB, int indexB);    

private:

    // Internal constants for the collision status cache.
    static const int DIRTY;
    static const int COLLISION;
    static const int REGISTERED;
    static const int REMOVE;

    // Represents the collision listeners and status for a given collision pair (used by the collision status cache).
    struct CollisionInfo
    {
        std::vector<PhysicsRigidBody::Listener*> _listeners;
        int _status;
    };

    // Wraps Bullet collision shapes (used for implementing shape caching).
    struct PhysicsCollisionShape : public Ref
    {
        PhysicsCollisionShape(btCollisionShape* shape) : _shape(shape) {}
        ~PhysicsCollisionShape() { SAFE_DELETE(_shape); }

        btCollisionShape* _shape;
    };

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

    // Adds the given collision listener for the two given rigid bodies.
    void addCollisionListener(PhysicsRigidBody::Listener* listener, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);

    // Adds the given rigid body to the world.
    void addRigidBody(PhysicsRigidBody* body);
    
    // Removes the given rigid body from the simulated physics world.
    void removeRigidBody(PhysicsRigidBody* rigidBody);
    
    // Gets the corresponding GamePlay object for the given Bullet object.
    PhysicsRigidBody* getRigidBody(const btCollisionObject* collisionObject);
    
    // Creates a box collision shape to be used in the creation of a rigid body.
    btCollisionShape* createBox(const Vector3& min, const Vector3& max, const Vector3& scale);

    // Creates a capsule collision shape to be used in the creation of a rigid body.
    btCollisionShape* createCapsule(float radius, float height);

    // Creates a sphere collision shape to be used in the creation of a rigid body.
    btCollisionShape* createSphere(float radius, const Vector3& scale);

    // Creates a triangle mesh collision shape to be used in the creation of a rigid body.
    btCollisionShape* createMesh(PhysicsRigidBody* body, const Vector3& scale);

    // Sets up the given constraint for the given two rigid bodies.
    void addConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint);

    // Checks whether constraints are supported for the given rigid bodies and emits a warning if they are not.
    bool checkConstraintRigidBodies(PhysicsRigidBody* a, PhysicsRigidBody* b);
    
    // Removes the given constraint from the simulated physics world.
    void removeConstraint(PhysicsConstraint* constraint);
    
    // Draws Bullet debug information.
    class DebugDrawer : public btIDebugDraw
    {
    public:

        struct DebugVertex
        {
            float x, y, z;
            float r, g, b, a;
        };

        DebugDrawer();        
        ~DebugDrawer();
        
        void begin(const Matrix& viewProjection);
        void end();

        // Overridden Bullet functions from btIDebugDraw.
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);        
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);        
        void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);        
        void reportErrorWarning(const char* warningString);
        void draw3dText(const btVector3& location, const char* textString);        
        void setDebugMode(int mode);        
        int	getDebugMode() const;
        
    private:
        
        int _mode;
        const Matrix* _viewProjection;
        MeshBatch* _meshBatch;
    };
    
    btDefaultCollisionConfiguration* _collisionConfiguration;
    btCollisionDispatcher* _dispatcher;
    btBroadphaseInterface* _overlappingPairCache;
    btSequentialImpulseConstraintSolver* _solver;
    btDynamicsWorld* _world;
    btGhostPairCallback* _ghostPairCallback;
    std::vector<PhysicsCollisionShape*> _shapes;
    DebugDrawer* _debugDrawer;
    Listener::EventType _status;
    std::vector<Listener*>* _listeners;
    std::vector<PhysicsRigidBody*> _bodies;
    Vector3 _gravity;
    std::map<PhysicsRigidBody::CollisionPair, CollisionInfo> _collisionStatus;  
};

}

#endif
