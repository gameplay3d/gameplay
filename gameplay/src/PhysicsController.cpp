/*
 * PhysicsController.cpp
 */

#include "Base.h"
#include "PhysicsController.h"

namespace gameplay
{

// Default gravity is 9.8 along the negative Y axis.
PhysicsController::PhysicsController()
	: _gravity(btScalar(0.0), btScalar(-9.8), btScalar(0.0)), _collisionConfiguration(NULL), _dispatcher(NULL),
	_overlappingPairCache(NULL), _solver(NULL), _world(NULL)
{
}

PhysicsController::~PhysicsController()
{
}

void PhysicsController::setGravity(Vector3 gravity)
{
	_gravity.setX(gravity.x);
	_gravity.setY(gravity.y);
	_gravity.setZ(gravity.z);

	if (_world)
	{
		_world->setGravity(_gravity);
	}
}

void PhysicsController::initialize()
{
	// TODO: Should any of this be configurable?
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_overlappingPairCache = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();

	// Create the world.
	_world = new btDiscreteDynamicsWorld(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
	_world->setGravity(_gravity);
}

void PhysicsController::finalize()
{
	// Remove the rigid bodies from the world and delete them.
	for (int i = _world->getNumCollisionObjects() - 1; i >= 0 ; i--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		_world->removeCollisionObject(obj);
		delete obj;
	}

	// Delete all of the collision shapes.
	for (int i = 0; i < _shapes.size(); i++)
	{
		btCollisionShape* shape = _shapes[i];
		_shapes[i] = 0;
		delete shape;
	}
	_shapes.clear();

	// Clean up the world and its various components.
	delete _world; 
	_world = NULL;

	delete _solver; 
	_solver = NULL;

	delete _overlappingPairCache; 
	_overlappingPairCache = NULL;
	
	delete _dispatcher; 
	_dispatcher = NULL;
	
	delete _collisionConfiguration; 
	_collisionConfiguration = NULL;
}

void PhysicsController::pause()
{
	// DUMMY FUNCTION
}

void PhysicsController::resume()
{
	// DUMMY FUNCTION
}

void PhysicsController::update(long elapsedTime)
{
	// Update the physics simulation, with a maximum
	// of 10 simulation steps being performed in a given frame.
	//
	// Note that stepSimulation takes elapsed time in seconds
	// so we divide by 1000 to convert from milliseconds.
	_world->stepSimulation((float)elapsedTime * 0.001, 10);
}

btCollisionShape* PhysicsController::getBox(const Vector3& min, const Vector3& max, const btVector3& scale)
{
	btVector3 halfExtents(scale.x() * 0.5 * abs(max.x - min.x), scale.y() * 0.5 * abs(max.y - min.y), scale.z() * 0.5 * abs(max.z - min.z));
	btBoxShape* box = new btBoxShape(halfExtents);
	_shapes.push_back(box);

	return box;
}

btCollisionShape* PhysicsController::getSphere(float radius, const btVector3& scale)
{
	// Since sphere shapes depend only on the radius, the best we can do is take
	// the largest dimension and apply that as the uniform scale to the rigid body.
	float uniformScale = scale.x();
	if (uniformScale < scale.y())
		uniformScale = scale.y();
	if (uniformScale < scale.z())
		uniformScale = scale.z();

	btSphereShape* sphere = new btSphereShape(uniformScale * radius);
	_shapes.push_back(sphere);

	return sphere;
}

btCollisionShape* PhysicsController::getTriangleMesh(float* vertexData, int vertexPositionStride, unsigned char* indexData, Mesh::IndexFormat indexFormat)
{
	return NULL;
}

btCollisionShape* PhysicsController::getHeightfield(void* data, int width, int height)
{
	return NULL;
}

}
