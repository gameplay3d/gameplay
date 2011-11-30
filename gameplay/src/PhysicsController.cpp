#include "Base.h"
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsMotionState.h"

namespace gameplay
{

// Default gravity is 9.8 along the negative Y axis.
PhysicsController::PhysicsController()
    : _gravity(btScalar(0.0), btScalar(-9.8), btScalar(0.0)), _collisionConfiguration(NULL), _dispatcher(NULL),
    _overlappingPairCache(NULL), _solver(NULL), _world(NULL), _status(Listener::DEACTIVATED), _listeners(NULL)
{
}

void PhysicsController::addStatusListener(Listener* listener)
{
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    _listeners->push_back(listener);
}

PhysicsController::~PhysicsController()
{
    SAFE_DELETE(_listeners);
}

PhysicsFixedConstraint* PhysicsController::createFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsFixedConstraint* constraint = new PhysicsFixedConstraint(a, b);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, b);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b,
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, rotationOffsetA, 
        translationOffsetA, b, rotationOffsetB, translationOffsetB);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsHingeConstraint* PhysicsController::createHingeConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b, 
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsHingeConstraint* constraint = new PhysicsHingeConstraint(a, rotationOffsetA, 
        translationOffsetA, b, rotationOffsetB, translationOffsetB);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a, b);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a,
    const Vector3& translationOffsetA, PhysicsRigidBody* b, const Vector3& translationOffsetB)
{
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a,
        translationOffsetA, b, translationOffsetB);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, b);
    setupConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b, 
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, rotationOffsetA, 
        translationOffsetA, b, rotationOffsetB, translationOffsetB);
    setupConstraint(a, b, constraint);
    return constraint;
}

const Vector3& PhysicsController::getGravity(const Vector3& gravity) const
{
    return _gravity;
}

void PhysicsController::setGravity(const Vector3& gravity)
{
    _gravity = gravity;

    if (_world)
    {
        _world->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
    }
}

void PhysicsController::initialize()
{
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _overlappingPairCache = new btDbvtBroadphase();
    _solver = new btSequentialImpulseConstraintSolver();

    // Create the world.
    _world = new btDiscreteDynamicsWorld(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
    _world->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}

void PhysicsController::finalize()
{
    // Clean up the world and its various components.
    SAFE_DELETE(_world);
    SAFE_DELETE(_solver);
    SAFE_DELETE(_overlappingPairCache);
    SAFE_DELETE(_dispatcher);
    SAFE_DELETE(_collisionConfiguration);
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

    // If we have status listeners, then check if our status has changed.
    if (_listeners)
    {
        Listener::EventType oldStatus = _status;

        if (_status = Listener::DEACTIVATED)
        {
            for (int i = 0; i < _world->getNumCollisionObjects(); i++)
            {
                if (_world->getCollisionObjectArray()[i]->isActive())
                {
                    _status = Listener::ACTIVATED;
                    break;
                }
            }
        }
        else
        {
            bool allInactive = true;
            for (int i = 0; i < _world->getNumCollisionObjects(); i++)
            {
                if (_world->getCollisionObjectArray()[i]->isActive())
                {
                    allInactive = false;
                    break;
                }
            }

            if (allInactive)
                _status = Listener::DEACTIVATED;
        }

        // If the status has changed, notify our listeners.
        if (oldStatus != _status)
        {
            for (unsigned int k = 0; k < _listeners->size(); k++)
            {
                (*_listeners)[k]->statusEvent(_status);
            }
        }
    }

    // All statuses are set with the DIRTY bit before collision processing occurs.
    // During collision processing, if a collision occurs, the status is 
    // set to COLLISION and the DIRTY bit is cleared. Then, after collision processing 
    // is finished, if a given status is still dirty, the COLLISION bit is cleared.

    // Dirty all the collision listeners' collision status caches.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    iter->second |= PhysicsRigidBody::Listener::DIRTY;
                }
            }
        }
    }

    // Go through the physics rigid bodies and update the collision listeners.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    // If this collision pair was one that was registered for listening, then perform the collision test.
                    // (In the case where we register for all collisions with a rigid body, there will be a lot
                    // of collision pairs in the status cache that we did not explicitly register for.)
                    if ((iter->second & PhysicsRigidBody::Listener::REGISTERED) != 0)
                    {
                        if (iter->first._rbB)
                            Game::getInstance()->getPhysicsController()->_world->contactPairTest(iter->first._rbA->_body, iter->first._rbB->_body, *(*_bodies[i]->_listeners)[k]);
                        else
                            Game::getInstance()->getPhysicsController()->_world->contactTest(iter->first._rbA->_body, *(*_bodies[i]->_listeners)[k]);
                    }
                }   
            }
        }
    }

    // Go through all the collision listeners and update their collision status caches.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    if ((iter->second & PhysicsRigidBody::Listener::DIRTY) != 0)
                    {
                        iter->second &= ~PhysicsRigidBody::Listener::COLLISION;
                    }
                }
            }
        }
    }
}

void PhysicsController::addRigidBody(PhysicsRigidBody* body)
{
    _world->addRigidBody(body->_body);
    _bodies.push_back(body);
}

btCollisionShape* PhysicsController::getBox(const Vector3& min, const Vector3& max, const btVector3& scale)
{
    btVector3 halfExtents(scale.x() * 0.5 * abs(max.x - min.x), scale.y() * 0.5 * abs(max.y - min.y), scale.z() * 0.5 * abs(max.z - min.z));
    BULLET_NEW(btBoxShape, box, halfExtents);
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

    BULLET_NEW(btSphereShape, sphere, uniformScale * radius);
    _shapes.push_back(sphere);

    return sphere;
}

PhysicsRigidBody* PhysicsController::getPhysicsRigidBody(const btCollisionObject* collisionObject)
{
    // Find the rigid body and remove it from the world.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_body == collisionObject)
            return _bodies[i];
    }

    return NULL;
}

void PhysicsController::removeConstraint(PhysicsConstraint* constraint)
{
    // Find the constraint and remove it from the physics world.
    for (int i = _world->getNumConstraints() - 1; i >= 0; i--)
    {
        btTypedConstraint* currentConstraint = _world->getConstraint(i);
        if (constraint->_constraint == currentConstraint)
        {
            _world->removeConstraint(currentConstraint);
            break;
        }
    }
}

void PhysicsController::removeRigidBody(PhysicsRigidBody* rigidBody)
{
    // Find the rigid body and remove it from the world.
    for (int i = _world->getNumCollisionObjects() - 1; i >= 0 ; i--)
    {
        btCollisionObject* obj = _world->getCollisionObjectArray()[i];
        if (rigidBody->_body == obj)
        {
            _world->removeCollisionObject(obj);
            break;
        }
    }
}

void PhysicsController::setupConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint)
{
    a->addConstraint(constraint);
    if (b)
    {
        b->addConstraint(constraint);
    }

    _world->addConstraint(constraint->_constraint);
}

}
