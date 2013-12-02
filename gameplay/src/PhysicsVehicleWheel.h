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

        steerable                = <bool>                // indicates whether wheel is steerable
        wheelDirection           = <float, float, float> // direction of strut extension, in chassis space
        wheelAxle                = <float, float, float> // direction of axle (spin axis), in chassis space
        strutConnectionOffset    = <float, float, float> // offset from default strut connection point
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
     * Operation not supported. Use host vehicle's setEnabled() instead.
     */
    void setEnabled(bool enable);

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
     * Returns true if this wheel is steerable, false otherwise.
     *
     * @return true if this wheel is steerable, false otherwise.
     */
    bool isSteerable() const;

    /**
     * Sets whether this wheel is steerable.
     *
     * @param steerable true if this wheel is steerable, false otherwise.
     */
    void setSteerable(bool steerable);

    /**
     * Gets direction of strut extension, in chassis space.
     *
     * @param wheelDirection address of where to store the result.
     */
    void getWheelDirection(Vector3* wheelDirection) const;

    /**
     * Sets direction of strut extension, in chassis space.
     *
     * @param wheelDirection direction of strut extension.
     */
    void setWheelDirection(const Vector3& wheelDirection);

    /**
     * Gets direction of axle (the spin axis), in chassis space.
     *
     * @param wheelAxle address of where to store the result.
     */
    void getWheelAxle(Vector3* wheelAxle) const;

    /**
     * Sets direction of axle (the spin axis), in chassis space.
     *
     * @param wheelAxle direction of axle (the spin axis).
     */
    void setWheelAxle(const Vector3& wheelAxle);

    /**
     * Gets offset from the default strut connection point.
     * The default strut connection point is determined from the position
     * of the wheel node relative to the chassis node, and uses the
     * specified value for strut rest length to locate the connection
     * point above it (i.e., in the specified direction of strut
     * compression).
     * Any non-zero strut connection offset acts as a delta from the
     * computed default.
     *
     * @param strutConnectionOffset address of where to store the result.
     */
    void getStrutConnectionOffset(Vector3* strutConnectionOffset) const;

    /**
     * Sets offset from the default strut connection point.
     * The default strut connection point is determined from the position
     * of the wheel node relative to the chassis node, and uses the
     * specified value for strut rest length to locate the connection
     * point above it (i.e., in the specified direction of strut
     * compression).
     * Any non-zero strutConnectionOffset acts as a delta from the
     * computed default.
     *
     * @param strutConnectionOffset offset from the default strut connection
     *     point.
     */
    void setStrutConnectionOffset(const Vector3& strutConnectionOffset);

    /**
     * Gets the strut rest length.
     *
     * @return the strut rest length.
     */
    float getStrutRestLength() const;

    /**
     * Sets the strut rest length.
     *
     * @param strutRestLength the strut rest length.
     */
    void setStrutRestLength(float strutRestLength);

    /**
     * Gets the maximum strut travel.
     *
     * @return the maximum strut travel.
     */
    float getStrutTravelMax() const;

    /**
     * Sets the maximum strut travel.
     *
     * @param strutTravelMax the maximum strut travel.
     */
    void setStrutTravelMax(float strutTravelMax);

    /**
     * Gets the strut stiffness, normalized to chassis mass.
     *
     * @return the strut stiffness, normalized to chassis mass.
     */
    float getStrutStiffness() const;

    /**
     * Sets the strut stiffness, normalized to chassis mass.
     *
     * @param strutStiffness the strut stiffness, normalized to chassis mass.
     */
    void setStrutStiffness(float strutStiffness);

    /**
     * Gets strut damping under compression, normalized to chassis mass.
     *
     * @return strut damping under compression, normalized to chassis mass.
     */
    float getStrutDampingCompression() const;

    /**
     * Sets strut damping under compression, normalized to chassis mass.
     *
     * @param strutDampingCompression strut damping under compression, normalized to chassis mass.
     */
    void setStrutDampingCompression(float strutDampingCompression);

    /**
     * Gets strut damping under relaxation, normalized to chassis mass.
     *
     * @return strut damping under relaxation, normalized to chassis mass.
     */
    float getStrutDampingRelaxation() const;

    /**
     * Sets strut damping under relaxation, normalized to chassis mass.
     *
     * @param strutDampingRelaxation strut damping under relaxation, normalized to chassis mass.
     */
    void setStrutDampingRelaxation(float strutDampingRelaxation);

    /**
     * Gets the maximum strut force.
     *
     * @return the maximum strut force.
     */
    float getStrutForceMax() const;

    /**
     * Sets the maximum strut force.
     *
     * @param strutForceMax the maximum strut force.
     */
    void setStrutForceMax(float strutForceMax);

    /**
     * Gets the breakout friction.
     *
     * @return the breakout friction.
     */
    float getFrictionBreakout() const;

    /**
     * Sets the breakout friction.
     *
     * @param frictionBreakout the breakout friction.
     */
    void setFrictionBreakout(float frictionBreakout);

    /**
     * Gets the wheel radius.
     *
     * @return the wheel radius.
     */
    float getWheelRadius() const;

    /**
     * Sets the wheel radius.
     *
     * @param wheelRadius the wheel radius.
     */
    void setWheelRadius(float wheelRadius);

    /**
     * Gets roll influence which determines how side friction affects chassis roll.
     *
     * @return roll influence, normalized factor.
     */
    float getRollInfluence() const;

    /**
     * Sets roll influence which determines how side friction affects chassis roll.
     *
     * @param rollInfluence roll influence, normalized factor.
     */
    void setRollInfluence(float rollInfluence);

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
     * Creates a vehicle wheel based on some 'safe' defaults.
     * Also, traverse up the scene graph until we find the first common ancestor with another node
     * of collision type VEHICLE and add ourself as a wheel onto that vehicle. This assumes that the
     * VEHICLE comes before the VEHICLE_WHEEL in the ".scene" (properties) file.
     * 
     * @param node The node to create a vehicle wheel for.
     */
    PhysicsVehicleWheel(Node* node);

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
     * advertised descendant of collision type VEHICLE, add this wheel onto the vehicle.
     */
    // Note: Currently this method is silent on failure to find a host.
    void findAncestorAndBind();

    /**
     * Depth-first search for the first vehicle starting from the specified node.
     *
     * @param node the starting node for the recursive search.
     */
    PhysicsVehicle* findVehicle(Node* node);

    /**
     * Sets the host vehicle for this wheel.
     *
     * @param host the host vehicle.
     * @param indexInHost the index of this wheel within the host vehicle.
     */
    void setHost(PhysicsVehicle* host, unsigned int indexInHost);

    /**
     * Adds this wheel to the specified Bullet vehicle.
     *
     * @param vehicle Bullet vehicle to add self to.
     */
    void addToVehicle(btRaycastVehicle* vehicle);

    /**
     * Update state of this wheel, per frame.
     *
     * @param elapsedTime The elapsed game time.
     */
    void update(float elapsedTime);

    /**
     * Computes the default strut connection point for
     * this wheel.
     * The default strut connection point is determined from the position
     * of the wheel node relative to the chassis node, and uses the
     * specified value for maximum strut travel to locate the connection
     * point above it (i.e., in the specified direction of strut
     * compression).
     *
     * @param result where to store the result.
     */
    void getConnectionDefault(Vector3* result) const;

    /**
     * Get wheel position at bind time relative to chassis.
     *
     * @param result where to store the result.
     */
    void getWheelPos(Vector3* result) const;

    PhysicsVehicle* _host;
    unsigned int _indexInHost;
    Vector3 _initialOffset;
    Vector3 _positionDelta;
    Quaternion _orientation;
};

}

#endif
