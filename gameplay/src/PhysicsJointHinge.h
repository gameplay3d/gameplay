#pragma once

#include "PhysicsJoint.h"

namespace gameplay
{

/**
 * Defines a hinge joint between two rigid bodies
 * (or one rigid body and the world) where movement is
 * restricted to rotation about one axis.
 *
 */
class PhysicsJointHinge : public PhysicsJoint
{
    friend class Physics;
    friend class Activator;
    friend class SceneObject;

public:
  
    /**
     * Constructor.
     */
    PhysicsJointHinge();
    
    /**
     * Destructor.
     */
    ~PhysicsJointHinge();

    /**
     * @see PhysicsJoint::getType
     */
    PhysicsJoint::Type getType() const;

    /**
     * Gets the current angle of the joint.
     *
     * @return The current angle of the joint.
     */
    float getAngle() const;

    /**
     * Gets the current velocity of the joint.
     *
     * @return The current velocity of the joint.
     */
    float getVelocity() const;

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
     * Sets the minimum angle of the limit.
     *
     * @param angleMin The minimum angle of the limit.
     */
    void setLimitAngleMin(float angleMin);

    /**
     * Gets the minimum angle of the limit.
     *
     * @return The minimum angle of the limit.
     */
    float getLimitAngleMin() const;

    /**
     * Sets the maximum angle of the limit.
     *
     * @param angleMax The maximum angle of the limit.
     */
    void setLimitAngleMax(float angleMax);

    /**
     * Gets the maximum angle of the limit.
     *
     * @return The maximum angle of the limit.
     */
    float getLimitAngleMax() const;

    /**
     * Sets the distance from the limit at which it becomes active.
     *
     * Default is the lesser of 0.1 radians, and 0.49 * (angleMax - angleMin)
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

    /**
     * Sets if the drive is enabled.
     * 
     * @param enabled true if the drive is enabled, false if not enabled.
     */
    void setDriveEnabled(bool enabled);

    /**
     * Gets if the drive is enabled.
     * 
     * @return true if the drive is enabled, false if not enabled.
     */
    bool isDriveEnabled() const;

    /**
     * Sets the target velocity for the drive.
     *
     * @param targetVelocity The target velocity for the drive.
     */
    void setDriveTargetVelocity(float targetVelocity);

    /**
     * Gets the target velocity for the drive.
     *
     * @return The target velocity for the drive.
     */
    float getDriveTargetVelocity() const;

    /**
     * Sets the maximum force the drive can exert. 
     *
     * @param force The maximum force the drive can exert. 
     */
    void setDriveForce(float force);

    /**
     * Gets the maximum force the drive can exert. 
     *
     * @return The maximum force the drive can exert. 
     */
    float getDriveForce() const;
    
    /**
     * Sets the gear ratio for the drive. 
     *
     * @param gearRatio The gear ratio for the drive.
     */
    void setDriveGearRatio(float gearRatio);

    /**
     * Gets the gear ratio for the drive. 
     *
     * @return The gear ratio for the drive.
     */
    float getDriveGearRatio() const;

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
