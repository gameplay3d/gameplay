#include "Base.h"
#include "Game.h"
#include "Node.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"

namespace gameplay
{

/**
  * The default vehicle raycaster in Bullet currently does not filter out the vehicle's own
  * rigid body from the ray test which can result in unexpected behavior. These implementations
  * are intended to fix that.
  *
  * @script{ignore}
  */
class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:

    ClosestNotMeRayResultCallback(const btVector3& from, const btVector3& to, btCollisionObject* me)
        : btCollisionWorld::ClosestRayResultCallback(from, to), _me(me)
    {
    }

    btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == _me)
            return 1.0f;

        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

private:

    btCollisionObject* _me;
};

/**
  * @script{ignore}
  */
class VehicleNotMeRaycaster : public btVehicleRaycaster
{

public:

    VehicleNotMeRaycaster(btDynamicsWorld* world, btCollisionObject* me)
        : _dynamicsWorld(world), _me(me)
    {
    }

    void* castRay(const btVector3& from, const btVector3& to, btVehicleRaycasterResult& result)
    {
        ClosestNotMeRayResultCallback rayCallback(from, to, _me);
        _dynamicsWorld->rayTest(from, to, rayCallback);

        if (rayCallback.hasHit())
        {
            const btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
            if (body && body->hasContactResponse())
            {
                result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
                result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
                result.m_hitNormalInWorld.normalize();
                result.m_distFraction = rayCallback.m_closestHitFraction;
                return (void*)body;
            }
        }
        return 0;
    }

private:

    btDynamicsWorld* _dynamicsWorld;
    btCollisionObject* _me;
};

PhysicsVehicle::PhysicsVehicle(Node* node, const PhysicsCollisionShape::Definition& shape, const PhysicsRigidBody::Parameters& parameters)
    : PhysicsCollisionObject(node)
{
    // Note that the constructor for PhysicsRigidBody calls addCollisionObject and so
    // that is where the rigid body gets added to the dynamics world.
    _rigidBody = new PhysicsRigidBody(node, shape, parameters);

    initialize();
}

PhysicsVehicle::PhysicsVehicle(Node* node, PhysicsRigidBody* rigidBody)
    : PhysicsCollisionObject(node)
{
    _rigidBody = rigidBody;

    initialize();
}

PhysicsVehicle* PhysicsVehicle::create(Node* node, Properties* properties)
{
    // Note that the constructor for PhysicsRigidBody calls addCollisionObject and so
    // that is where the rigid body gets added to the dynamics world.
    PhysicsRigidBody* rigidBody = PhysicsRigidBody::create(node, properties, "VEHICLE");
    PhysicsVehicle* vehicle = new PhysicsVehicle(node, rigidBody);

    // Load the defined vehicle parameters.
    properties->rewind();
    const char* name;
    while ((name = properties->getNextProperty()) != NULL)
    {
        if (strcmp(name, "steeringGain") == 0)
        {
            vehicle->setSteeringGain(properties->getFloat());
        }
        else if (strcmp(name, "brakingForce") == 0)
        {
            vehicle->setBrakingForce(properties->getFloat());
        }
        else if (strcmp(name, "drivingForce") == 0)
        {
            vehicle->setDrivingForce(properties->getFloat());
        }
        else
        {
            // Ignore this case (we've already parsed the rigid body parameters).
        }
    }

    return vehicle;
}

void PhysicsVehicle::initialize()
{
    GP_ASSERT(getNode());

    // Safe default values
    setSteeringGain(0.5f);
    setBrakingForce(350.0f);
    setDrivingForce(2000.0f);

    // Create the vehicle and add it to world
    btRigidBody* body = static_cast<btRigidBody*>(_rigidBody->getCollisionObject());
    btDynamicsWorld* dynamicsWorld = Game::getInstance()->getPhysicsController()->_world;
    _vehicleRaycaster = new VehicleNotMeRaycaster(dynamicsWorld, body);
    _vehicle = new btRaycastVehicle(_vehicleTuning, body, _vehicleRaycaster);
    body->setActivationState(DISABLE_DEACTIVATION);
    dynamicsWorld->addVehicle(_vehicle);
    _vehicle->setCoordinateSystem(0, 1, 2);

    // Advertise self among ancestor nodes so that wheels can bind to self.
    // See PhysicsVehicleWheel and Node for more details.
    for (Node* n = getNode()->getParent(); n; n = n->getParent())
    {
        n->addAdvertisedDescendant(getNode());
    }
}

PhysicsVehicle::~PhysicsVehicle()
{
    // Note that the destructor for PhysicsRigidBody calls removeCollisionObject and so
    // that is where the rigid body gets removed from the dynamics world. The vehicle
    // itself is just an action interface in the dynamics world.
    SAFE_DELETE(_vehicle);
    SAFE_DELETE(_vehicleRaycaster);
    SAFE_DELETE(_rigidBody);
}

btCollisionObject* PhysicsVehicle::getCollisionObject() const
{
    GP_ASSERT(_rigidBody);

    return _rigidBody->getCollisionObject();
}

PhysicsCollisionObject::Type PhysicsVehicle::getType() const
{
    return PhysicsCollisionObject::VEHICLE;
}

PhysicsRigidBody* PhysicsVehicle::getRigidBody() const
{
    GP_ASSERT(_rigidBody);

    return _rigidBody;
}

unsigned int PhysicsVehicle::getNumWheels() const
{
    return _wheels.size();
}

PhysicsVehicleWheel* PhysicsVehicle::getWheel(unsigned int i)
{
    return _wheels.at(i);
}

void PhysicsVehicle::addWheel(PhysicsVehicleWheel* wheel)
{
    unsigned int i = _wheels.size();
    _wheels.push_back(wheel);
    wheel->setHost(this, i);
    wheel->addToVehicle(_vehicle);
}

float PhysicsVehicle::getSpeedKph() const
{
    return _vehicle->getCurrentSpeedKmHour();
}

void PhysicsVehicle::update(float elapsedTime, float steering, float braking, float driving)
{
    PhysicsVehicleWheel* wheel;
    for (int i = 0; i < _vehicle->getNumWheels(); i++)
    {
        wheel = getWheel(i);

        if (wheel->isFront())
        {
            _vehicle->setSteeringValue(steering * _steeringGain, i);
        }
        else
        {
            // Allow braking to take precedence over driving.
            if (driving > 0 && braking > 0)
            {
                driving = 0;
            }

            _vehicle->applyEngineForce(driving * _drivingForce, i);
            _vehicle->setBrake(braking * _brakingForce, i);
        }

        wheel->update(elapsedTime);
        wheel->transform(wheel->getNode());
    }
}

float PhysicsVehicle::getSteeringGain() const
{
    return _steeringGain;
}

void PhysicsVehicle::setSteeringGain(float steeringGain)
{
    _steeringGain = steeringGain;
}

float PhysicsVehicle::getBrakingForce() const
{
    return _brakingForce;
}

void PhysicsVehicle::setBrakingForce(float brakingForce)
{
    _brakingForce = brakingForce;
}

float PhysicsVehicle::getDrivingForce() const
{
    return _drivingForce;
}

void PhysicsVehicle::setDrivingForce(float drivingForce)
{
    _drivingForce = drivingForce;
}

}
