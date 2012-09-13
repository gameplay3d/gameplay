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

 @verbatim
    collisionObject <vehicleID>
    {
        type           = VEHICLE

        shape          = BOX        // collision shape for vehicle body
        mass           = <float>    // mass of vehicle body
        friction       = <float>    // friction of vehicle body
        restitution    = <float>    // restitution of vehicle body
        linearDamping  = <float>    // linear damping of vehicle body
        angularDamping = <float>    // angular damping of vehicle body

        // Vehicle steering, braking, and powertrain
        steeringGain   = <float>    // steering at full deflection
        brakingForce   = <float>    // braking force at full braking
        drivingForce   = <float>    // driving force at full throttle
    }
 @endverbatim
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
     * Updates the vehicle state using the specified normalized command
     * inputs, and updates the transform on the visual node for each wheel.
     *
     * @param steering steering command (-1 to 1).
     * @param braking braking command (0 to 1).
     * @param driving net drivetrain command (0 to 1).
     */
    void update(float steering, float braking, float driving);

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

    float _steeringGain;
    float _brakingForce;
    float _drivingForce;
    PhysicsRigidBody* _rigidBody;
    btRaycastVehicle::btVehicleTuning _vehicleTuning;
    btVehicleRaycaster* _vehicleRaycaster;
    btRaycastVehicle* _vehicle;
    std::vector<PhysicsVehicleWheel*> _wheels;
};

}

#endif
