#pragma once

#include "PhysicsJoint.h"

namespace gameplay
{

/**
 * Defines a ball-socket joint between two rigid bodies 
 * (or one rigid body and the world). This removes all 
 * linear degrees of freedom from two objects.
 *
 * A spherical joint may have a cone limit, 
 * to restrict the motion to within a certain range.
 */
class PhysicsJointSocket : public PhysicsJoint
{
    friend class Physics;
    friend class SceneObject;
    friend class Activator;

public:
  
    /**
     * Constructor.
     */
    PhysicsJointSocket();
    
    /**
     * Destructor.
     */
    ~PhysicsJointSocket();

    /**
     * @see PhysicsJoint::getType
     */
    PhysicsJoint::Type getType() const;

    /**
     * Sets if a limit is enabled for the joint.
     *
     * @param enabled true if a limit is enabled for the joint, false if no limit is set.
     */
    void setLimitEnabled(bool enabled);

    /**
     * Determines if a limit is enabled for the joint.
     *
     * @return true if a limit is enabled for the joint, false if no limit is set.
     */
    bool isLimitEnabled() const;

   /**
     * Sets the y-angle of the limit.
     *
     * @param angleY The y-angle of the limit.
     */
    void setLimitAngleY(float angleY);

    /**
     * Gets the y-angle of the limit.
     *
     * @return The y-angle of the limit.
     */
    float getLimitAngleY() const;

    /**
     * Sets the z-angle of the limit.
     *
     * @param angleZ The z-angle of the limit.
     */
    void setLimitAngleZ(float angleZ);

    /**
     * Gets the z-angle of the limit.
     *
     * @return The z-angle of the limit.
     */
    float getLimitAngleZ() const;

    /**
     * Sets the distance from the limit at which it becomes active.
     *
     * Default is the lesser of 0.1 radians, and 0.49 the lower of the limit angles
     *
     * @param contactDistance The distance from the limit at which it becomes active.
     */
    void setLimitContactDistance(float contactDistance);

    /**
     * Gets the distance from the limit at which it becomes active.
     *
     * Default is the lesser of 0.1 radians, and 0.49 * (angleMax - angleMin)
     *
     * @return The distance from the limit at which it becomes active.
     */
    float getLimitContactDistance() const;

    /**
     * Sets the amount of bounce when the joint hits a limit.
     *
     * @param bounce The amount of bounce when the joint hits a limit.
     */
    void setLimitBounce(float bounce);

    /**
     * Gets the amount of bounce when the joint hits a limit.
     *
     * @return The amount of bounce when the joint hits a limit.
     */
    float getLimitBounce() const;

    /**
     * Sets the minimum impact velocity which will cause the joint to bounce.
     *
     * @param bounceThreshold The minimum impact velocity which will cause the joint to bounce.
     */
    void setLimitBounceThreshold(float bounceThreshold);

    /**
     * Gets the minimum impact velocity which will cause the joint to bounce.
     *
     * @return The minimum impact velocity which will cause the joint to bounce.
     */
    float getLimitBounceThreshold() const;


    /**
     * Set if a soft spring limit is set on the joint.
     *
     * @param enabled true if a soft spring limit is set on the joint, false if hard limit.
     */
    void setLimitSpringEnabled(bool enabled);

    /**
     * Determines if a soft spring limit is set on the joint.
     *
     * @return true if a soft spring limit is set on the joint, false if hard limit.
     */
    bool isLimitSpringEnabled();

    /**
     * Sets the stiffness on the soft spring limit.
     *
     * If greater than zero, the limit is soft, 
     * Ex. a spring pulls the joint back to the limit.
     *
     * @param stiffness The stiffness on the soft spring limit.
     */
    void setLimitSpringStiffness(float stiffness);

    /**
     * Gets the stiffness on the soft spring limit.
     *
     * If greater than zero, the limit is soft, 
     * Ex. a spring pulls the joint back to the limit.
     *
     * @return The stiffness on the soft spring limit.
     */
    float getLimitSpringStiffness() const;

    /**
     * Sets the damping on the soft spring limit.
     *
     * @param dampening The damping on the soft spring limit.
     */
    void setLimitSpringDampening(float dampening);

    /**
     * Gets the damping on the soft spring limit.
     *
     * @return The damping on the soft spring limit.
     */
    float getLimitSpringDampening() const;

protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();
    
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);
};

}
