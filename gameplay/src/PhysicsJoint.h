#pragma once

#include "Component.h"
#include "Physics.h"
#include "Vector3.h"
#include "PhysicsRigidBody.h" 

namespace gameplay
{

/**
 * Defines a physics joint which constraints the
 * way two rigid bodies can moved relative to 
 * one another.
 */
class PhysicsJoint : public Component
{
public:

    friend class Physics;
    friend class SceneObject;

    /**
     * Defines the type of joint.
     */
    enum class Type
    {
        eFixed,
        eHinge,
        eSpring,
        eSocket,
        eGeneric
    };

    /**
     * Constructor.
     */
    PhysicsJoint();
    
    /**
     * Destructor.
     */
    ~PhysicsJoint();

     /**
      * Gets the type of joint this is.
      */
    virtual PhysicsJoint::Type getType() const = 0;

    /**
     * Sets the connected rigid bodies for the joint.
     *
     * The connected rigid bodies can be set to nullptr.
     * In this case it uses the world position as the origin.
     * 
     * @param rigidBody The rigid body connected to the joint.
     */
    void setConnectedBody(std::shared_ptr<PhysicsRigidBody> rigidBody);

    /**
     * Gets the connected rigid body for the joint.
     *
     * The connected rigid bodies can be set to nullptr.
     * In this case it uses the world position as the origin. 
     *
     * @return The connected rigid body for the joint.
     */
    std::shared_ptr<PhysicsRigidBody> getConnectedBody() const;

    /**
     * Gets the force needed to break the joint.
     * 
     * @return The force needed to break the joint.
     */
    float getBreakingForce() const;

    /**
     * Sets the force needed to break the joint.
     * 
     * @param force The force needed to break the joint.
     */
    void setBreakingForce(float force);

    /**
     * Gets the torque needed to break the joint.
     * 
     * @return The torque needed to break the joint.
     */
    float getBreakingTorque() const;

    /**
     * Sets the torque needed to break the joint.
     * 
     * @param torque The torque needed to break the joint.
     */
    void setBreakingTorque(float torque);

    /**
     * Sets the anchor position of the joint around which motion is constraint.
     *
     * @param anchorPosition The anchor position of the joint around which motion is constraint.
     */
    void setAnchorPosition(const Vector3& anchorPosition);

    /**
     * Gets the anchor position of the joint around which motion is constraint.
     *
     * return The anchor position of the joint around which motion is constraint.
     */
    Vector3 getAnchorPosition() const;

    /**
     * Sets if collisions between rigid bodies connected 
     * to the joint are enabled.
     *
     * @param collisionsEnabled true if collisions between rigid bodies connected 
     *      to the joint are enabled.
     */
    void setCollisionsEnabled(bool collisionsEnabled);

    /**
     * Determines if collisions between rigid bodies connected 
     * to the joint are enabled.
     *
     * @return true if collisions between rigid bodies connected 
     *      to the joint are enabled.
     */
    bool isCollisionsEnabled() const;

    /**
     * Sets if preproccessing is enabled for this joint.
     *
     *  @param preprocessingEnabled true if preproccessing is enabled for this joint.
     */
    void setPreprocessingEnabled(bool preprocessingEnabled);

    /**
     * Determines if preproccessing is enabled for this joint.
     *
     *  @return true if preproccessing is enabled for this joint.
     */
    bool isPreprocessingEnabled() const;
    
protected:

    /**
     * @see Serializable::onSerialize
     */
    virtual void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    virtual void onDeserialize(Serializer* serializer);

    /**
     * Event occurs when the force or torque constraints are broken.
     */
    void onConstraintBreak() const;
};

}
