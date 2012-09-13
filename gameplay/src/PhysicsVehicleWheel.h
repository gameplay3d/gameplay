#ifndef PHYSICSVEHICLEWHEEL_H_
#define PHYSICSVEHICLEWHEEL_H_

#include "PhysicsCollisionObject.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

class Node;
class PhysicsVehicle;

/**
 * Defines a class for vehicle wheel physics which represents the individual wheel
 * itself as well as the tire and suspension.
 *
 * The following properties are available for wheels:

 @verbatim
    collisionObject <wheelID>
    {
        type                     = VEHICLE_WHEEL

        isFront                  = <bool>                // indicates whether this is a front wheel
        wheelDirection           = <float, float, float> // direction strut extension, in chassis space
        wheelAxle                = <float, float, float> // direction of axle (spin axis), in chassis space
        strutConnectionPoint     = <float, float, float> // strut connection point, in chassis space
        strutRestLength          = <float>               // strut rest length
        strutTravelMax           = <float>               // maximum strut travel
        strutStiffness           = <float>               // strut stiffness, normalized to chassis mass
        strutDampingCompression  = <float>               // strut damping under compression, normalized to chassis mass
        strutDampingRelaxation   = <float>               // strut damping under relaxation, normalized to chassis mass
        strutForceMax            = <float>               // maximum strut force
        frictionBreakout         = <float>               // breakout friction
        wheelRadius              = <float>               // wheel radius
        rollInfluence            = <float>               // how side friction affects chassis roll, normalized
    }
 @endverbatim
 */

class PhysicsVehicleWheel : public PhysicsCollisionObject
{
    friend class Node;
    friend class PhysicsVehicle;

public:

    /**
     * @see PhysicsCollisionObject#getType
     */
    PhysicsCollisionObject::Type getType() const;

    /**
     * Apply this wheel's world transform to the specified node.
     * Useful for updating the specified visual node with the current
     * transform.
     *
     * @param node the node to be transformed; (typically a visual
     * representation of this wheel).
     */
    void transform(Node* node) const;

    /**
     * Returns true if this is a front wheel, false otherwise.
     *
     * @return true if this is a front wheel, false otherwise.
     */
    /*TODO: inline*/ bool isFront() const;

    /**
     * Sets whether this is a front wheel.
     *
     * @param front true if this is a front wheel, false otherwise.
     */
    /*TODO: inline*/ void setFront(bool front);

    /**
     * Gets direction of strut extension, in chassis space.
     *
     * @param wheelDirection address of where to store the result.
     */
    /*TODO: inline*/ void getWheelDirection(Vector3* wheelDirection) const;

    /**
     * Sets direction of strut extension, in chassis space.
     *
     * @param wheelDirection direction of strut extension.
     */
    /*TODO: inline*/ void setWheelDirection(const Vector3& wheelDirection);

    /**
     * Gets direction of axle (the spin axis), in chassis space.
     *
     * @param wheelAxle address of where to store the result.
     */
    /*TODO: inline*/ void getWheelAxle(Vector3* wheelAxle) const;

    /**
     * Sets direction of axle (the spin axis), in chassis space.
     *
     * @param wheelAxle direction of axle (the spin axis).
     */
    /*TODO: inline*/ void setWheelAxle(const Vector3& wheelAxle);

    /**
     * Gets strut connection point, in chassis space.
     *
     * @param strutConnectionPoint address of where to store the result.
     */
    /*TODO: inline*/ void getStrutConnectionPoint(Vector3* strutConnectionPoint) const;

    /**
     * Sets strut connection point, in chassis space.
     *
     * @param strutConnectionPoint strut connection point.
     */
    /*TODO: inline*/ void setStrutConnectionPoint(const Vector3& strutConnectionPoint);

    /**
     * Gets the strut rest length.
     *
     * @return the strut rest length.
     */
    /*TODO: inline*/ float getStrutRestLength() const;

    /**
     * Sets the strut rest length.
     *
     * @param strutRestLength the strut rest length.
     */
    /*TODO: inline*/ void setStrutRestLength(float strutRestLength);

    /**
     * Gets the maximum strut travel.
     *
     * @return the maximum strut travel.
     */
    /*TODO: inline*/ float getStrutTravelMax() const;

    /**
     * Sets the maximum strut travel.
     *
     * @param strutTravelMax the maximum strut travel.
     */
    /*TODO: inline*/ void setStrutTravelMax(float strutTravelMax);

    /**
     * Gets the strut stiffness, normalized to chassis mass.
     *
     * @return the strut stiffness, normalized to chassis mass.
     */
    /*TODO: inline*/ float getStrutStiffness() const;

    /**
     * Sets the strut stiffness, normalized to chassis mass.
     *
     * @param strutStiffness the strut stiffness, normalized to chassis mass.
     */
    /*TODO: inline*/ void setStrutStiffness(float strutStiffness);

    /**
     * Gets strut damping under compression, normalized to chassis mass.
     *
     * @return strut damping under compression, normalized to chassis mass.
     */
    /*TODO: inline*/ float getStrutDampingCompression() const;

    /**
     * Sets strut damping under compression, normalized to chassis mass.
     *
     * @param strutDampingCompression strut damping under compression, normalized to chassis mass.
     */
    /*TODO: inline*/ void setStrutDampingCompression(float strutDampingCompression);

    /**
     * Gets strut damping under relaxation, normalized to chassis mass.
     *
     * @return strut damping under relaxation, normalized to chassis mass.
     */
    /*TODO: inline*/ float getStrutDampingRelaxation() const;

    /**
     * Sets strut damping under relaxation, normalized to chassis mass.
     *
     * @param strutDampingRelaxation strut damping under relaxation, normalized to chassis mass.
     */
    /*TODO: inline*/ void setStrutDampingRelaxation(float strutDampingRelaxation);

    /**
     * Gets the maximum strut force.
     *
     * @return the maximum strut force.
     */
    /*TODO: inline*/ float getStrutForceMax() const;

    /**
     * Sets the maximum strut force.
     *
     * @param strutForceMax the maximum strut force.
     */
    /*TODO: inline*/ void setStrutForceMax(float strutForceMax);

    /**
     * Gets the breakout friction.
     *
     * @return the breakout friction.
     */
    /*TODO: inline*/ float getFrictionBreakout() const;

    /**
     * Sets the breakout friction.
     *
     * @param frictionBreakout the breakout friction.
     */
    /*TODO: inline*/ void setFrictionBreakout(float frictionBreakout);

    /**
     * Gets the wheel radius.
     *
     * @return the wheel radius.
     */
    /*TODO: inline*/ float getWheelRadius() const;

    /**
     * Sets the wheel radius.
     *
     * @param wheelRadius the wheel radius.
     */
    /*TODO: inline*/ void setWheelRadius(float wheelRadius);

    /**
     * Gets roll influence which determines how side friction affects chassis roll.
     *
     * @return roll influence, normalized factor.
     */
    /*TODO: inline*/ float getRollInfluence() const;

    /**
     * Sets roll influence which determines how side friction affects chassis roll.
     *
     * @param rollInfluence roll influence, normalized factor.
     */
    /*TODO: inline*/ void setRollInfluence(float rollInfluence);

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

private:

    /**
     * Creates a vehicle wheel based on the specified rigid body parameters and some 'safe' defaults.
     * Also, traverse up the scene graph until we find the first common ancestor with another node
     * of collision type VEHICLE and add ourself as a wheel onto that vehicle. This assumes that the
     * VEHICLE comes before the VEHICLE_WHEEL in the ".scene" (properties) file.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param shape The rigid body shape construction information.
     * @param parameters The rigid body construction parameters.
     */
    PhysicsVehicleWheel(Node* node, const PhysicsCollisionShape::Definition& shape, const PhysicsRigidBody::Parameters& parameters);

    /**
     * Creates a vehicle wheel based on the given rigid body and some 'safe' defaults.
     * Also, traverse up the scene graph until we find the first common ancestor with another node
     * of collision type VEHICLE and add ourself as a wheel onto that vehicle. This assumes that the
     * VEHICLE comes before the VEHICLE_WHEEL in the ".scene" (properties) file.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param rigidBody The rigid body.
     */
    PhysicsVehicleWheel(Node* node, PhysicsRigidBody* rigidBody);

    /**
     * Private copy constructor to prevent copying.
     */
    PhysicsVehicleWheel(const PhysicsVehicleWheel& src);

    /**
     * Private copy assignment operator.
     */
    PhysicsVehicleWheel& operator=(const PhysicsVehicleWheel&);

    /**
     * Creates a vehicle wheel physics object from the specified properties object.
     * Also, traverse up the scene graph until we find the first common ancestor with another node
     * of collision type VEHICLE and add ourself as a wheel onto that vehicle. This assumes that the
     * VEHICLE comes before the VEHICLE_WHEEL in the ".scene" (properties) file.
     * 
     * @param node The node to create a wheel for; note that the node must have
     *      a model attached to it prior to creating a vehicle wheel for it.
     * @param properties The properties object defining the vehicle wheel (must have type equal to 'VEHICLE_WHEEL').
     * @return The newly created wheel, or <code>NULL</code> if the vehicle wheel failed to load.
     */
    static PhysicsVehicleWheel* create(Node* node, Properties* properties);

    /**
     * Destructor.
     */
    ~PhysicsVehicleWheel();

    /**
     * Traverse up the visual scene graph. Upon finding the first ancestor node with an
     * advertised descendant of collsion type VEHICLE, add this wheel onto the vehicle.
     */
    // Note: Currently this method is silent on failure to find a host.
    void findAncestorAndBind();

    /**
     * Sets the host vehicle for this wheel.
     *
     * @param host the host vehicle.
     * @param indexInHost the index of this wheel within the host vehicle.
     */
    /*TODO: inline*/ void setHost(PhysicsVehicle* host, unsigned int indexInHost);

    /**
     * Adds this wheel to the specified Bullet vehicle.
     *
     * @param vehicle Bullet vehicle to add self to.
     */
    void addToVehicle(btRaycastVehicle* vehicle);

    PhysicsRigidBody* _rigidBody;
    PhysicsVehicle* _host;
    unsigned int _indexInHost;
};

}

#endif /* PHYSICSVEHICLEWHEEL_H_ */
