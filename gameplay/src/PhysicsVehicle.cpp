#include "Base.h"
#include "Game.h"
#include "MathUtil.h"
#include "Node.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"

#define AIR_DENSITY (1.2f)
#define KPH_TO_MPS (1.0f / 3.6f)

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
    : PhysicsCollisionObject(node), _speedSmoothed(0)
{
    // Note that the constructor for PhysicsRigidBody calls addCollisionObject and so
    // that is where the rigid body gets added to the dynamics world.
    _rigidBody = new PhysicsRigidBody(node, shape, parameters);

    initialize();
}

PhysicsVehicle::PhysicsVehicle(Node* node, PhysicsRigidBody* rigidBody)
    : PhysicsCollisionObject(node), _speedSmoothed(0)
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
        else if (strcmp(name, "steerdownSpeed") == 0)
        {
            vehicle->_steerdownSpeed = properties->getFloat();
        }
        else if (strcmp(name, "steerdownGain") == 0)
        {
            vehicle->_steerdownGain = properties->getFloat();
        }
        else if (strcmp(name, "brakedownStart") == 0)
        {
            vehicle->_brakedownStart = properties->getFloat();
        }
        else if (strcmp(name, "brakedownFull") == 0)
        {
            vehicle->_brakedownFull = properties->getFloat();
        }
        else if (strcmp(name, "drivedownStart") == 0)
        {
            vehicle->_drivedownStart = properties->getFloat();
        }
        else if (strcmp(name, "drivedownFull") == 0)
        {
            vehicle->_drivedownFull = properties->getFloat();
        }
        else if (strcmp(name, "boostSpeed") == 0)
        {
            vehicle->_boostSpeed = properties->getFloat();
        }
        else if (strcmp(name, "boostGain") == 0)
        {
            vehicle->_boostGain = properties->getFloat();
        }
        else if (strcmp(name, "downforce") == 0)
        {
            vehicle->_downforce = properties->getFloat();
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
    setSteerdown(0, 1);
    setBrakedown(1000, 0);
    setDrivedown(1000, 0);
    setBoost(0, 1);
    setDownforce(0);

    // Create the vehicle and add it to world
    btRigidBody* body = static_cast<btRigidBody*>(_rigidBody->getCollisionObject());
    btDynamicsWorld* dynamicsWorld = Game::getInstance()->getPhysicsController()->_world;
    _vehicleRaycaster = new VehicleNotMeRaycaster(dynamicsWorld, body);
    _vehicle = bullet_new<btRaycastVehicle>(_vehicleTuning, body, _vehicleRaycaster);
    body->setActivationState(DISABLE_DEACTIVATION);
    dynamicsWorld->addVehicle(_vehicle);
    _vehicle->setCoordinateSystem(0, 1, 2);
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

void PhysicsVehicle::setEnabled(bool enable)
{
    getRigidBody()->setEnabled(enable);
}

unsigned int PhysicsVehicle::getNumWheels() const
{
    return (unsigned int)_wheels.size();
}

PhysicsVehicleWheel* PhysicsVehicle::getWheel(unsigned int i)
{
    return _wheels.at(i);
}

void PhysicsVehicle::addWheel(PhysicsVehicleWheel* wheel)
{
    unsigned i = (unsigned int)_wheels.size();
    _wheels.push_back(wheel);
    wheel->setHost(this, i);
    wheel->addToVehicle(_vehicle);
}

float PhysicsVehicle::getSpeedKph() const
{
    return _vehicle->getCurrentSpeedKmHour();
}

float PhysicsVehicle::getSpeedSmoothKph() const
{
    return _speedSmoothed;
}

void PhysicsVehicle::update(float elapsedTime, float steering, float braking, float driving)
{
    float v = getSpeedKph();
    MathUtil::smooth(&_speedSmoothed, v, elapsedTime, 0, 1200);
    if (elapsedTime > 0)
    {
        // Avoid accumulation of downforce while paused (zero elapsedTime)
        applyDownforce();
    }

    // Adjust control inputs based on vehicle speed.
    steering = getSteering(v, steering);
    driving = getDriving(v, driving, braking);
    braking = getBraking(v, braking);

    // Allow braking to take precedence over driving.
    if (driving > 0 && braking > 0)
    {
        driving = 0;
    }

    PhysicsVehicleWheel* wheel;
    for (int i = 0; i < _vehicle->getNumWheels(); i++)
    {
        wheel = getWheel(i);

        if (wheel->isSteerable())
        {
            _vehicle->setSteeringValue(steering * _steeringGain, i);
        }
        else
        {
            _vehicle->applyEngineForce(driving * _drivingForce, i);
            _vehicle->setBrake(braking * _brakingForce, i);
        }

        wheel->update(elapsedTime);
        wheel->transform(wheel->getNode());
    }
}

void PhysicsVehicle::reset()
{
    _rigidBody->setLinearVelocity(Vector3::zero());
    _rigidBody->setAngularVelocity(Vector3::zero());
    _speedSmoothed = 0;
}

float PhysicsVehicle::getSteering(float v, float rawSteering) const
{
    float gain = 1;
    if (_steerdownSpeed > MATH_FLOAT_SMALL)
    {
        gain = max(_steerdownGain, 1 - (1 - _steerdownGain) * fabs(v) / _steerdownSpeed);
    }

    return rawSteering * gain;
}

float PhysicsVehicle::getBraking(float v, float rawBraking) const
{
    float reduc = 0;
    float delta = _brakedownFull - _brakedownStart;
    if (delta > MATH_FLOAT_SMALL)
    {
        reduc = max(0.0f, (v - _brakedownStart) / delta);
        reduc *= reduc;
    }

    return max(0.0f, rawBraking - reduc);
}

float PhysicsVehicle::getDriving(float v, float rawDriving, float rawBraking) const
{
    float reduc = 0;
    float delta = _drivedownFull - _drivedownStart;
    if (rawBraking == 0 && delta > MATH_FLOAT_SMALL)
    {
        reduc = max(0.0f, (v - _drivedownStart) / delta);
        reduc *= reduc;
    }

    float gain = 1;
    if (_boostSpeed > MATH_FLOAT_SMALL)
    {
        gain = max(1.0f, _boostGain - (_boostGain - 1) * fabs(v) / _boostSpeed);
    }

    return gain * rawDriving - reduc;
}

void PhysicsVehicle::applyDownforce()
{
    float v = _speedSmoothed * KPH_TO_MPS;

    // dynamic pressure
    float q = 0.5f * AIR_DENSITY * v * v;

    // _downforce is the product of reference area and the aerodynamic coefficient
    _rigidBody->applyForce(Vector3(0, -_downforce * q, 0));
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

float PhysicsVehicle::getSteerdownSpeed() const
{
    return _steerdownSpeed;
}

float PhysicsVehicle::getSteerdownGain() const
{
    return _steerdownGain;
}

void PhysicsVehicle::setSteerdown(float steerdownSpeed, float steerdownGain)
{
    _steerdownSpeed = steerdownSpeed;
    _steerdownGain = steerdownGain;
}

float PhysicsVehicle::getBrakedownStart() const
{
    return _brakedownStart;
}

float PhysicsVehicle::getBrakedownFull() const
{
    return _brakedownFull;
}

void PhysicsVehicle::setBrakedown(float brakedownStart, float brakedownFull)
{
    _brakedownStart = brakedownStart;
    _brakedownFull = brakedownFull;
}

float PhysicsVehicle::getDrivedownStart() const
{
    return _drivedownStart;
}

float PhysicsVehicle::getDrivedownFull() const
{
    return _drivedownFull;
}

void PhysicsVehicle::setDrivedown(float drivedownStart, float drivedownFull)
{
    _drivedownStart = drivedownStart;
    _drivedownFull = drivedownFull;
}

float PhysicsVehicle::getBoostSpeed() const
{
    return _boostSpeed;
}

float PhysicsVehicle::getBoostGain() const
{
    return _boostGain;
}

void PhysicsVehicle::setBoost(float boostSpeed, float boostGain)
{
    _boostSpeed = boostSpeed;
    _boostGain = boostGain;
}

float PhysicsVehicle::getDownforce() const
{
    return _downforce;
}

void PhysicsVehicle::setDownforce(float downforce)
{
    _downforce = downforce;
}

}
