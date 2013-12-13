#ifndef PHYSICSVEHICLE_H_
#define PHYSICSVEHICLE_H_

#include "PhysicsCollisionObject.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

class Node;
class PhysicsVehicleWheel;

/**
 * Defines a class for vehicle physics.
 *
 * In addition to its own properties defined below, a vehicle has available
 * to it all of the properties of a rigid body such as shape, mass, friction,
 * etc which correspond to the vehicle body:
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Collision_Objects
 */
class PhysicsVehicle : public PhysicsCollisionObject
{
    friend class Node;
    friend class PhysicsVehicleWheel;

public:

    /**
     * @see PhysicsCollisionObject#getType
     */
    PhysicsCollisionObject::Type getType() const;

    /**
     * Returns the rigid body associated with this vehicle.
     */
    PhysicsRigidBody* getRigidBody() const;

    /**
     * Sets whether the associated rigid body is enabled or disabled
     * in the physics world.
     *
     * @param enable true enables the collision object, false disables it.
     */
    void setEnabled(bool enable);

    /**
     * Returns the number of wheels on this vehicle.
     *
     * @return the number of wheels on this vehicle.
     */
    unsigned int getNumWheels() const;

    /**
     * Gets the wheel at the specified index.
     * 
     * @param i index of wheel.
     * @return the wheel at the specified index.
     */
    PhysicsVehicleWheel* getWheel(unsigned int i);

    /**
     * Permanently adds a wheel to this vehicle.
     *
     * @param wheel the wheel to add.
     */
    void addWheel(PhysicsVehicleWheel* wheel);

    /**
     * Returns an indication of vehicle speed in kilometers per hour.
     */
    float getSpeedKph() const;

    /**
     * Returns a lagged version of vehicle speed in kilometers per hour,
     * for example that might be used to control engine sounds.
     */
    float getSpeedSmoothKph() const;

    /**
     * Updates the vehicle state using the specified normalized command
     * inputs, and updates the transform on the visual node for each wheel.
     *
     * @param elapsedTime The elapsed game time.
     * @param steering steering command (-1 to 1).
     * @param braking braking command (0 to 1).
     * @param driving net drivetrain command (0 to 1).
     */
    void update(float elapsedTime, float steering, float braking, float driving);

    /**
     * Resets the vehicle's state, for example in preparation for a reposition.
     */
    void reset();

    /**
     * Gets steering gain at full deflection.
     *
     * @return steering gain at full deflection.
     */
    float getSteeringGain() const;

    /**
     * Sets steering gain at full deflection.
     *
     * @param steeringGain steering gain at full deflection.
     */
    void setSteeringGain(float steeringGain);

    /**
     * Gets braking force at full braking.
     *
     * @return braking force at full braking.
     */
    float getBrakingForce() const;

    /**
     * Sets braking force at full braking.
     *
     * @param brakingForce braking force at full braking.
     */
    void setBrakingForce(float brakingForce);

    /**
     * Gets driving force at full throttle.
     *
     * @return driving force at full throttle.
     */
    float getDrivingForce() const;

    /**
     * Sets driving force at full throttle.
     *
     * @param drivingForce driving force at full throttle.
     */
    void setDrivingForce(float drivingForce);

    /**
     * Returns speed at the point of reduced steering, in km/h.
     * A point of reduced steering is defined by speed and gain.
     * Steering authority will reduce linearly with speed up to
     * this point, and remain constant above that.
     *
     * @return speed at the point of reduced steering, in km/h.
     */
    float getSteerdownSpeed() const;

    /**
     * Returns gain at the point of reduced steering, typically
     * less than 1.
     * A point of reduced steering is defined by speed and gain.
     * Steering authority will reduce linearly with speed up to
     * this point, and remain constant above that.
     *
     * @return gain at the point of reduced steering.
     */
    float getSteerdownGain() const;

    /**
     * Sets the point of reduced steering, defined by speed and
     * gain. Typically the gain value is less than 1.
     * Steering authority will reduce linearly with speed up to
     * this point, and remain constant above that.
     *
     * @param steerdownSpeed speed at the point of reduced steering,
     *     in km/h.
     * @param steerdownGain gain at the point of reduced steering.
     *     A gain of 1 will effectively disable the feature.
     */
    void setSteerdown(float steerdownSpeed, float steerdownGain);

    /**
     * Returns speed where braking starts to fade, in km/h.
     *
     * @return speed where braking starts to fade, in km/h.
     */
    float getBrakedownStart() const;

    /**
     * Returns speed where braking is fully faded, in km/h.
     * This speed is typically greater than the brakedownStart
     * speed.
     *
     * @return speed where braking is fully faded, in km/h.
     */
    float getBrakedownFull() const;

    /**
     * Sets points that control fade of brake force with speed,
     * in km/h.
     *
     * @param brakedownStart braking fades above this speed.
     *     A very large value will effectively disable the feature.
     * @param brakedownFull braking is fully faded at this speed.
     *     This speed is typically greater than the brakedownStart
     *     speed.
     */
    void setBrakedown(float brakedownStart, float brakedownFull);

    /**
     * Returns speed where driving force starts to fade, in km/h.
     *
     * @return speed where driving force starts to fade, in km/h.
     */
    float getDrivedownStart() const;

    /**
     * Returns speed where driving force is fully faded, in km/h.
     * This speed is typically greater than the drivedownStart
     * speed.
     *
     * @return speed where driving force is fully faded, in km/h.
     */
    float getDrivedownFull() const;

    /**
     * Sets points that control fade of driving force with speed,
     * in km/h.
     *
     * @param drivedownStart driving force fades above this speed.
     *     A very large value will effectively disable the feature.
     * @param drivedownFull driving force is fully faded at this speed.
     *     This speed is typically greater than the drivedownStart
     *     speed.
     */
    void setDrivedown(float drivedownStart, float drivedownFull);

    /**
     * Returns upper limit of low-speed boost effect, in km/h.
     * Driving force is boosted by a specified factor at zero speed,
     * and that factor fades linearly with speed reaching 1 at
     * this speed.
     *
     * @return upper limit of low-speed boost effect, in km/h.
     */
    float getBoostSpeed() const;

    /**
     * Returns boost gain at zero speed, typically greater than 1.
     * Driving force is boosted by this factor at zero speed, and
     * that factor fades linearly with speed reaching 1 at a
     * specified speed.
     *
     * @return boost gain at zero speed.
     */
    float getBoostGain() const;

    /**
     * Sets parameters that define low-speed boost of the driving force.
     * Driving force is boosted by the specified factor at zero speed,
     * and that factor fades linearly with speed reaching 1 at the
     * specified speed.
     *
     * @param boostSpeed upper limit of low-speed boost effect, in km/h.
     * @param boostGain boost gain at zero speed, typically greater than 1.
     *     A gain of 1 will effectively disable the feature.
     */
    void setBoost(float boostSpeed, float boostGain);

    /**
     * Returns the lumped constant that controls aerodynamic downforce.
     * Technically speaking, this constant lumps together the reference
     * area and the down-force coefficient, and is in world-units squared.
     * The actual aerodynamic down-force is calculated as a function of
     * current speed, and is proportional to this constant.
     *
     * @return the lumped constant that controls aerodynamic downforce.
     */
    float getDownforce() const;

    /**
     * Sets the lumped constant that controls aerodynamic downforce.
     * Technically speaking, this constant lumps together the reference
     * area and the down-force coefficient, and is in world-units squared.
     * The actual aerodynamic down-force is calculated as a function of
     * current speed, and is proportional to this constant.
     *
     * @param downforce the lumped constant that controls aerodynamic downforce.
     *     A value of 0 will effectively disable this feature.
     */
    void setDownforce(float downforce);

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

private:

    /**
     * Creates a vehicle based on the specified rigid body parameters and some 'safe' defaults.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param shape The rigid body shape construction information.
     * @param parameters The rigid body construction parameters.
     */
    PhysicsVehicle(Node* node, const PhysicsCollisionShape::Definition& shape, const PhysicsRigidBody::Parameters& parameters);

    /**
     * Creates a vehicle based on the given rigid body and some 'safe' defaults.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param rigidBody The rigid body.
     */
    PhysicsVehicle(Node* node, PhysicsRigidBody* rigidBody);

    /**
     * Private copy constructor to prevent copying.
     */
    PhysicsVehicle(const PhysicsVehicle& src);

    /**
     * Private copy assignment operator.
     */
    PhysicsVehicle& operator=(const PhysicsVehicle&);

    /**
     * Creates a vehicle physics object from the specified properties object.
     * 
     * @param node The node to create a vehicle for; note that the node must have
     *      a model attached to it prior to creating a vehicle for it.
     * @param properties The properties object defining the vehicle (must have type equal to 'VEHICLE').
     * @return The newly created vehicle, or <code>NULL</code> if the vehicle failed to load.
     */
    static PhysicsVehicle* create(Node* node, Properties* properties);

    /**
     * Initializes this vehicle and advertises itself among its ancestor nodes.
     */
    void initialize();

    /**
     * Destructor.
     */
    ~PhysicsVehicle();

    /**
     * Returns adjusted steering value.
     *
     * @param v vehicle speed.
     * @param rawSteering raw steering command.
     */
    float getSteering(float v, float rawSteering) const;

    /**
     * Returns adjusted braking force value.
     *
     * @param v vehicle speed.
     * @param rawBraking raw braking force command.
     */
    float getBraking(float v, float rawBraking) const;

    /**
     * Returns adjusted driving force value.
     *
     * @param v vehicle speed.
     * @param rawDriving raw driving force command.
     * @param rawBraking raw braking force command.
     */
    float getDriving(float v, float rawDriving, float rawBraking) const;

    /**
     * Applies effect of aerodynamic downforce.
     */
    void applyDownforce();

    float _steeringGain;
    float _brakingForce;
    float _drivingForce;
    float _steerdownSpeed;
    float _steerdownGain;
    float _brakedownStart;
    float _brakedownFull;
    float _drivedownStart;
    float _drivedownFull;
    float _boostSpeed;
    float _boostGain;
    float _downforce;
    float _speedSmoothed;
    PhysicsRigidBody* _rigidBody;
    btRaycastVehicle::btVehicleTuning _vehicleTuning;
    btVehicleRaycaster* _vehicleRaycaster;
    btRaycastVehicle* _vehicle;
    std::vector<PhysicsVehicleWheel*> _wheels;
};

}

#endif
