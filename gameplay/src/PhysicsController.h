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
#include "HeightField.h"
#include "ScriptTarget.h"

namespace gameplay
{

class ScriptListener;

/**
 * Defines a class for controlling game physics.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Physics
 */
class PhysicsController : public ScriptTarget
{
    friend class Game;
    friend class PhysicsConstraint;
    friend class PhysicsRigidBody;
    friend class PhysicsCharacter;
    friend class PhysicsVehicle;
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

    protected:

        /**
         * Destructor.
         */
        virtual ~Listener();
    };

    /**
     * Structure that stores hit test results for ray and sweep tests.
     */
    struct HitResult
    {
        /**
         * The collision object that was hit.
         */
        PhysicsCollisionObject* object;

        /**
         * The point where the collision occurred, in world space.
         */
        Vector3 point;

        /**
         * The fraction (0-1) of the test distance to the collision point.
         */
        float fraction;

        /**
         * The normal vector of the collision surface, in world space.
         */
        Vector3 normal;
    };

    /**
     * Class that can be overridden to provide custom hit test filters for ray
     * and sweep tests.
     *
     * The default implementation of this class returns only the closest object
     * that intersects a ray of volume.
     */
    class HitFilter
    {
    public:

        /**
         * Constructor.
         */
        HitFilter();

        /**
         * Virtual destructor.
         */
        virtual ~HitFilter();

        /**
         * Called before performing a hit test with an object to determine
         * whether or not the object should be tested.
         *
         * @param object Object to be queried.
         * 
         * @return True if the object should be filtered out, or false to include the object in the test (default).
         */
        virtual bool filter(PhysicsCollisionObject* object);

        /**
         * Called when a ray or sweep test collides with a collision object.
         *
         * Each collision object that is hit during the ray or sweep test is passed
         * to this method, along with details of the hit result. Returning true to
         * this method will continue with normal hit test processing, where only
         * closer objects are returned. Returning false results in this method being
         * called for all objects that intersect the ray or volume.
         *
         * @param result HitResult object containing information about the hit.
         * 
         * @return True (default) to continue with default behavior where closer
         *      objects are processed, false to process all intersecting objects.
         */
        virtual bool hit(const HitResult& result);
    };

    /**
     * Adds a listener to the physics controller.
     * 
     * @param listener The listener to add.
     */
    void addStatusListener(PhysicsController::Listener* listener);

    /**
     * Removes a listener to the physics controller.
     * 
     * @param listener The listener to remove.
     */
    void removeStatusListener(Listener* listener);

    /**
     * Creates a fixed constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     * @return Pointer to the created PhysicsFixedConstraint object.
     */
    PhysicsFixedConstraint* createFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b = NULL);

    /**
     * Creates a generic constraint so that the rigid body (or bodies) is
     * (are) constrained to its (their) current world position(s).
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     * @return Pointer to the created PhysicsGenericConstraint object.
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
     * @return Pointer to the created PhysicsGenericConstraint object.
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
     * @return Pointer to the created PhysicsHingeConstraint object.
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
     * @return Pointer to the created PhysicsSocketConstraint object.
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
     * @return Pointer to the created PhysicsSocketConstraint object.
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
     * @return Pointer to the created PhysicsSpringConstraint object.
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
     * @return Pointer to the created PhysicsSpringConstraint object.
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
     * Performs a ray test on the physics world.
     * 
     * @param ray The ray to test intersection with.
     * @param distance How far along the given ray to test for intersections.
     * @param result Optional pointer to a HitTest structure to store the hit test result information in.
     *      When using a default (or no) filter, this will always be the closest object hit. Otherwise, if 
     *      using a custom filter, it will be the last object passed to the HitFilter::hit method (which
     *      is not necessarily the closest or furthest).
     * @param filter Optional filter pointer used to control which objects are tested.
     *
     * @return True if the ray test collided with a physics object, false otherwise.
     */
    bool rayTest(const Ray& ray, float distance, PhysicsController::HitResult* result = NULL, PhysicsController::HitFilter* filter = NULL);

    /**
     * Performs a sweep test of the given collision object on the physics world.
     *
     * The start position of the sweep test is defined by the current world position
     * of the specified collision object.
     *
     * @param object The collision object to test.
     * @param endPosition The end position of the sweep test, in world space.
     * @param result Optional pointer to a HitTest structure to store the hit test result information in.
     *      When using a default (or no) filter, this will always be the closest object hit. Otherwise, if 
     *      using a custom filter, it will be the last object passed to the HitFilter::hit method (which
     *      is not necessarily the closest or furthest).
     * @param filter Optional filter pointer used to control which objects are tested.
     * 
     * @return True if the object intersects any other physics objects, false otherwise.
     */
    bool sweepTest(PhysicsCollisionObject* object, const Vector3& endPosition, PhysicsController::HitResult* result = NULL, PhysicsController::HitFilter* filter = NULL);

private:

    /**
     * Internal class used to integrate with Bullet collision callbacks.
     */
    class CollisionCallback : public btCollisionWorld::ContactResultCallback
    {
    public:
        /**
         * Constructor.
         * 
         * @param pc The physics controller that owns the callback.
         */
        CollisionCallback(PhysicsController* pc) : _pc(pc) {}

    protected:
        /**
            * Internal function used for Bullet integration (do not use or override).
            */
        btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* a, int partIdA, int indexA, const btCollisionObjectWrapper* b, int partIdB, int indexB);    

    private:
        PhysicsController* _pc;
    };

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
    void update(float elapsedTime);

    // Adds the given collision listener for the two given collision objects.
    void addCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB);

    // Removes the given collision listener.
    void removeCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB);

    // Adds the given collision object to the world.
    void addCollisionObject(PhysicsCollisionObject* object);
    
    // Removes the given collision object from the simulated physics world.
    void removeCollisionObject(PhysicsCollisionObject* object, bool removeListeners);
    
    // Gets the corresponding GamePlay object for the given Bullet object.
    PhysicsCollisionObject* getCollisionObject(const btCollisionObject* collisionObject) const;

    // Creates a collision shape for the given node and gameplay shape definition.
    // Populates 'centerOfMassOffset' with the correct calculated center of mass offset.
    PhysicsCollisionShape* createShape(Node* node, const PhysicsCollisionShape::Definition& shape, Vector3* centerOfMassOffset, bool dynamic);
    
    // Creates a box collision shape.
    PhysicsCollisionShape* createBox(const Vector3& extents, const Vector3& scale);

    // Creates a sphere collision shape.
    PhysicsCollisionShape* createSphere(float radius, const Vector3& scale);

    // Creates a capsule collision shape.
    PhysicsCollisionShape* createCapsule(float radius, float height, const Vector3& scale);

    // Creates a heightfield collision shape.
    PhysicsCollisionShape* createHeightfield(Node* node, HeightField* heightfield, Vector3* centerOfMassOffset);

    // Creates a triangle mesh collision shape.
    PhysicsCollisionShape* createMesh(Mesh* mesh, const Vector3& scale, bool dynamic);

    // Destroys a collision shape created through PhysicsController
    void destroyShape(PhysicsCollisionShape* shape);

    // Legacy method for grayscale heightmaps: r + g + b, normalized.
    static float normalizedHeightGrayscale(float r, float g, float b);

    // Higher precision method for heightmaps: 65536*r + 256*g + b, normalized.
    static float normalizedHeightPacked(float r, float g, float b);

    // Sets up the given constraint for the given two rigid bodies.
    void addConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint);

    // Checks whether constraints are supported for the given rigid bodies and emits a warning if they are not.
    bool checkConstraintRigidBodies(PhysicsRigidBody* a, PhysicsRigidBody* b);
    
    // Removes the given constraint from the simulated physics world.
    void removeConstraint(PhysicsConstraint* constraint);
    
    /**
     * Draws Bullet debug information.
     * @script{ignore}
     */
    class DebugDrawer : public btIDebugDraw
    {
    public:

        /** 
         * DebugVertex.
         * @script{ignore}
         */
        struct DebugVertex
        {
            /**
             * The x coordinate of the vertex.
             */
            float x;
    
            /**
             * The y coordinate of the vertex.
             */
            float y;
    
            /**
             * The z coordinate of the vertex.
             */
            float z;

            /**
             * The red color component of the vertex.
             */
            float r;
    
            /**
             * The green color component of the vertex.
             */
            float g;
    
            /**
             * The blue color component of the vertex.
             */
            float b;
    
            /**
             * The alpha component of the vertex.
             */
            float a;
        };

        /**
         * Constructor.
         */
        DebugDrawer(); 

        /** 
         * Destructor.
         */
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
        MeshBatch* _meshBatch;
        int _lineCount;
    };

    bool _isUpdating;
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
    CollisionCallback* _collisionCallback;
};

}

#endif
