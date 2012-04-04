#ifndef PHYSICSCONTROLLER_H_
#define PHYSICSCONTROLLER_H_

#include "PhysicsConstraint.h"
#include "PhysicsFixedConstraint.h"
#include "PhysicsGenericConstraint.h"
#include "PhysicsHingeConstraint.h"
#include "PhysicsSocketConstraint.h"
#include "PhysicsSpringConstraint.h"
#include "PhysicsCollisionObject.h"
#include "MeshBatch.h"

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
    friend class PhysicsCollisionObject;
    friend class PhysicsGhostObject;

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
    const Vector3& getGravity() const;

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
     * @param distance How far along the given ray to test for intersections.
     * @param hitPoint Optional Vector3 point that is populated with the world-space point of intersection.
     * @param hitFraction Optional float pointer that is populated with the distance along the ray
     *      (as a fraction between 0-1) where the intersection occurred.
     * @return The first rigid body that the ray intersects, or NULL if no intersection was found.
     */
    PhysicsCollisionObject* rayTest(const Ray& ray, float distance, Vector3* hitPoint = NULL, float* hitFraction = NULL);

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
        CollisionInfo() : _status(0) { }

        std::vector<PhysicsCollisionObject::CollisionListener*> _listeners;
        int _status;
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

    // Adds the given collision listener for the two given collision objects.
    void addCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB);

    // Removes the given collision listener.
    void removeCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB);

    // Adds the given collision object to the world.
    void addCollisionObject(PhysicsCollisionObject* object);
    
    // Removes the given collision object from the simulated physics world.
    void removeCollisionObject(PhysicsCollisionObject* object);
    
    // Gets the corresponding GamePlay object for the given Bullet object.
    PhysicsCollisionObject* getCollisionObject(const btCollisionObject* collisionObject) const;

    // Creates a collision shape for the given node and gameplay shape definition.
    // Populates 'centerOfMassOffset' with the correct calculated center of mass offset.
    PhysicsCollisionShape* createShape(Node* node, const PhysicsCollisionShape::Definition& shape, Vector3* centerOfMassOffset);
    
    // Creates a box collision shape.
    PhysicsCollisionShape* createBox(const Vector3& extents, const Vector3& scale);

    // Creates a sphere collision shape.
    PhysicsCollisionShape* createSphere(float radius, const Vector3& scale);

    // Creates a capsule collision shape.
    PhysicsCollisionShape* createCapsule(float radius, float height, const Vector3& scale);

    // Creates a heightfield collision shape.
    PhysicsCollisionShape* createHeightfield(Node* node, Image* image, Vector3* centerOfMassOffset);

    // Creates a triangle mesh collision shape.
    PhysicsCollisionShape* createMesh(Mesh* mesh, const Vector3& scale);

    // Destroys a collision shape created through PhysicsController
    void destroyShape(PhysicsCollisionShape* shape);

    // Helper function for calculating heights from heightmap (image) or heightfield data.
    static float calculateHeight(float* data, unsigned int width, unsigned int height, float x, float y);

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
        int    getDebugMode() const;
        
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
    Vector3 _gravity;
    std::map<PhysicsCollisionObject::CollisionPair, CollisionInfo> _collisionStatus;

};

}

#endif
