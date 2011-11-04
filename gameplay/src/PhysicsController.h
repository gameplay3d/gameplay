/*
 * PhysicsController.h
 */

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
class PhysicsController
{
	friend class Game;
	friend class PhysicsRigidBody;

public:
    
	/**
	 * Destructor
	 */
	virtual ~PhysicsController();

	/**
	 * Sets the gravity vector for the simulated physics world.
	 * 
	 * @param gravity The gravity vector.
	 */
	void setGravity(Vector3 gravity);

    /**
     * Creates a fixed constraint.
     */
    PhysicsFixedConstraint* createFixedConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
        const Vector3& translationOffsetA, PhysicsRigidBody* b = NULL, 
        const Quaternion& rotationOffsetB = Quaternion(), const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a generic constraint.
     */
    PhysicsGenericConstraint* createGenericConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
        const Vector3& translationOffsetA, PhysicsRigidBody* b = NULL, 
        const Quaternion& rotationOffsetB = Quaternion(), const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a hinge constraint.
     */
    PhysicsHingeConstraint* createHingeConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
        const Vector3& translationOffsetA, PhysicsRigidBody* b = NULL, 
        const Quaternion& rotationOffsetB = Quaternion(), const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a socket constraint.
     */
    PhysicsSocketConstraint* createSocketConstraint(PhysicsRigidBody* a, const Vector3& translationOffsetA,
        PhysicsRigidBody* b = NULL, const Vector3& translationOffsetB = Vector3());

    /**
     * Creates a spring constraint.
     */
    PhysicsSpringConstraint* createSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, 
        const Vector3& translationOffsetA, PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

private:
	
	/**
	 * Constructor.
	 */
	PhysicsController();

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

	btCollisionShape* getBox(const Vector3& min, const Vector3& max, const btVector3& scale);
	btCollisionShape* getSphere(float radius, const btVector3& scale);
	btCollisionShape* getTriangleMesh(float* vertexData, int vertexPositionStride, unsigned char* indexData, Mesh::IndexFormat indexFormat);
	btCollisionShape* getHeightfield(void* data, int width, int height);

    void addConstraint(PhysicsConstraint* constraint);

	btVector3 _gravity;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _overlappingPairCache;
	btSequentialImpulseConstraintSolver* _solver;
	btDynamicsWorld* _world;
	btAlignedObjectArray<btCollisionShape*> _shapes;

    std::vector<PhysicsConstraint*> _constraints;
};

}

#endif
