#pragma once

#include "PhysicsJoint.h"

namespace gameplay
{

/**
 * Defines a completely generic joint between two
 * rigid bodies (or one rigid body and the world) where the
 * limits for all six degrees of freedom can be set individually.
 */
class PhysicsJointGeneric : public PhysicsJoint
{
    friend class Physics;
    friend class Activator;
    friend class SceneObject;

public:
  
    /**
     * Defines the 6 degrees of freedom motion axis.
     */
    enum class MotionAxis
    {
        eX,
        eY,
        eZ,
        eTwistX,
        eSwingY,
        eSwingZ
    };

    /**
     * Defines he range of motions allowed for a single degree of freedom.
     */
    enum class MotionRange
    {
        eLocked,
        eLimited,
        eFree
    };

    /**
     * Constructor.
     */
    PhysicsJointGeneric();
    
    /**
     * Destructor.
     */
    ~PhysicsJointGeneric();

    /**
     * @see PhysicsJoint::getType
     */
    PhysicsJoint::Type getType() const;

    /**
     * Gets the twist angle of the joint.
     *
     * @return The twist angle of the joint 
     */
    float getTwistX() const;

    /**
     * Gets the swing angle of the joint from the y-axis.
     *
     * @return The swing angle of the joint from the y-axis.
     */
    float getSwingY() const;

    /**
     * Gets the swing angle of the joint from the z-axis.
     *
     * @return The swing angle of the joint from the z-axis.
     */
    float getSwingZ() const;

    /**
     * Sets the motion range for the specified motion axis.
     *
     * @param axis The motion axis to set the range on.
     * @param range The motion range to be set.
     */
    void setMotionRange(MotionAxis axis, MotionRange range);

    /**
     * Gets the motion range for the specified motion axis.
     *
     * @param axis The motion axis to set the range on.
     * @return The motion range for the specified motion axis.
     */
    MotionRange getMotionRange(MotionAxis axis) const;

    /**
     * Sets the linear extent limit for the joint. 
     *
     * @param extent The linear extent limit for the joint. 
     */
    void setLinearLimitExtent(float extent);

    /**
     * Gets the linear extent limit for the joint. 
     *
     * @return The linear extent limit for the joint. 
     */
    float getLinearLimitExtent() const;

    /**
     * Sets the linear bounce limit for the joint. 
     *
     * @param bounce The linear bounce limit for the joint. 
     */
    void setLinearLimitBounce(float bounce);

    /**
     * Gets the linear bounce limit for the joint. 
     *
     * @return The linear bounce limit for the joint. 
     */
    float getLinearLimitBounce() const;

    /**
     * Sets the linear contact distance limit for the joint. 
     *
     * @param contactDistance The linear contact distance limit for the joint. 
     */
    void setLinearLimitContactDistance(float contactDistance);

    /**
     * Gets the linear contact distance limit for the joint. 
     *
     * @return The linear contact distance limit for the joint. 
     */
    float getLinearLimitContactDistance() const;

    /**
     * Sets the linear spring stiffness limit for the joint. 
     *
     * @param stiffness The linear spring stiffness limit for the joint. 
     */
    void setLinearLimitSpringStiffness(float stiffness);

    /**
     * Gets the linear spring stiffness limit for the joint. 
     *
     * @return The linear spring stiffness limit for the joint. 
     */
    float getLinearLimitSpringStiffness() const;

    /**
     * Sets the linear spring dampening limit for the joint. 
     *
     * @param dampening The linear spring dampening limit for the joint. 
     */
    void setLinearLimitSpringDampening(float dampening);

    /**
     * Gets the linear spring dampening limit for the joint. 
     *
     * @return The linear spring dampening limit for the joint. 
     */
    float getLinearLimitSpringDampening() const;

    /**
     * Sets the twist extent limit for the joint. 
     *
     * @param extent The twist extent limit for the joint. 
     */
    void setTwistLimitExtent(float extent);

    /**
     * Gets the twist extent limit for the joint. 
     *
     * @return The twist extent limit for the joint. 
     */
    float getTwistLimitExtent() const;

    /**
     * Sets the twist bounce limit for the joint. 
     *
     * @param bounce The twist bounce limit for the joint.
     */
    void setTwistLimitBounce(float bounce);

    /**
     * Gets the twist bounce limit for the joint. 
     *
     * @return The twist bounce limit for the joint. 
     */
    float getTwistLimitBounce() const;

    /**
     * Sets the twist contact distance limit for the joint. 
     *
     * @param contactDistance The twist contact distance limit for the joint. 
     */
    void setTwistLimitContactDistance(float contactDistance);

    /**
     * Gets the twist contact distance limit for the joint. 
     *
     * @return The twist contact distance limit for the joint. 
     */
    float getTwistLimitContactDistance() const;

    /**
     * Sets the twist spring stiffness limit for the joint. 
     *
     * @param stiffness The twist spring stiffness limit for the joint. 
     */
    void setTwistLimitSpringStiffness(float stiffness);

    /**
     * Gets the twist spring stiffness limit for the joint. 
     *
     * @return The twist spring stiffness limit for the joint. 
     */
    float getTwistLimitSpringStiffness() const;

    /**
     * Sets the twist spring dampening limit for the joint. 
     *
     * @param dampening The twist spring dampening limit for the joint. 
     */
    void setTwistLimitSpringDampening(float dampening);

    /**
     * Gets the twist spring dampening limit for the joint. 
     *
     * @return The twist spring dampening limit for the joint. 
     */
    float getTwistLimitSpringDampening() const;


    /**
     * Sets the swing extent limit for the joint. 
     *
     * @param extent The swing extent limit for the joint. 
     */
    void setSwingLimitExtent(float extent);

    /**
     * Gets the swing extent limit for the joint. 
     *
     * @return The swing extent limit for the joint. 
     */
    float getSwingLimitExtent() const;

    /**
     * Sets the swing bounce limit for the joint. 
     *
     * @param bounce The swing bounce limit for the joint. 
     */
    void setSwingLimitBounce(float bounce);

    /**
     * Gets the swing bounce limit for the joint. 
     *
     * @return The swing bounce limit for the joint. 
     */
    float getSwingLimitBounce() const;

    /**
     * Sets the swing contact distance limit for the joint. 
     *
     * @param contactDistance The swing contact distance limit for the joint. 
     */
    void setSwingLimitContactDistance(float contactDistance);

    /**
     * Gets the swing contact distance limit for the joint. 
     *
     * @return The swing contact distance limit for the joint. 
     */
    float getSwingLimitContactDistance() const;

    /**
     * Sets the swing spring stiffness limit for the joint. 
     *
     * @param stiffness The swing spring stiffness limit for the joint. 
     */
    void setSwingLimitSpringStiffness(float stiffness);

    /**
     * Gets the swing spring stiffness limit for the joint. 
     *
     * @return The swing spring stiffness limit for the joint. 
     */
    float getSwingLimitSpringStiffness() const;

    /**
     * Sets the swing spring dampening limit for the joint. 
     *
     * @param dampening The swing spring dampening limit for the joint. 
     */
    void setSwingLimitSpringDampening(float dampening);

    /**
     * Gets the swing spring dampening limit for the joint. 
     *
     * @return The swing spring dampening limit for the joint. 
     */
    float getSwingLimitSpringDampening() const;

    /**
     * Sets the drive target position.
     *
     * @param targetPosition The drive target position.
     */
    void setDriveTargetPosition(const Vector3& targetPosition);

    /**
     * Gets the drive target position.
     *
     * @return The drive target position.
     */
    Vector3 getDriveTargetPosition() const;

    /**
     * Sets the drive target velocity.
     *
     * @param targetVelocity The drive target velocity.
     */
    void setDriveTargetVelocity(float targetVelocity);

    /**
     * Gets the drive target velocity.
     *
     * @return The drive target velocity.
     */
    float getDriveTargetVelocity() const;

    /**
     * Sets the drive stiffness for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @param stiffness The stiffness to set for the drive.
     */
    void setDriveStiffness(MotionAxis axis, float stiffness);

    /**
     * Gets the drive stiffness for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @return The drive stiffness for the specified motion axis.
     */
    float getDriveStiffness(MotionAxis axis);

    /**
     * Sets the drive dampening for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @param dampening The dampening to set for the drive.
     */
    void setDriveDampening(MotionAxis axis, float dampening);

    /**
     * Gets the drive dampening for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @return The drive dampening for the specified motion axis.
     */
    float getDriveDampening(MotionAxis axis);

    /**
     * Sets the drive force for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @param force The force to set for the drive.
     */
    void setDriveForce(MotionAxis axis, float force);

    /**
     * Gets the drive force for the specified motion axis.
     *
     * @param axis The motion axis to set.
     * @return The drive force for the specified motion axis.
     */
    float getDriveForce(MotionAxis axis);

protected:

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();
    
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
