#pragma once

#include "Component.h"

namespace gameplay
{

/**
 * Defines a component that controls the object's 
 * position and rotation through physics simulation.
 */
class PhysicsRigidBody : public Component
{
    friend SceneObject;
    friend Serializer::Activator;

public:

    /**
     * Defines constraints used to freeze position and rotation.
     */
    enum FreezeConstraint : uint32_t
    {
        FREEZE_CONTRAINT_POSITION_X = (1 << 0),
        FREEZE_CONTRAINT_POSITION_Y = (1 << 1),
        FREEZE_CONTRAINT_POSITION_Z = (1 << 2),
        FREEZE_CONTRAINT_ROTATION_X = (1 << 3),
        FREEZE_CONTRAINT_ROTATION_Y = (1 << 4),
        FREEZE_CONTRAINT_ROTATION_Z = (1 << 5),
        FREEZE_CONTRAINT_POSITION = FREEZE_CONTRAINT_POSITION_X | FREEZE_CONTRAINT_POSITION_Y | FREEZE_CONTRAINT_POSITION_Z,
        FREEZE_CONTRAINT_ROTATION = FREEZE_CONTRAINT_ROTATION_X | FREEZE_CONTRAINT_ROTATION_Y | FREEZE_CONTRAINT_ROTATION_X
    };

    enum ForceMode
    {
        FORCE_MODE_FORCE,
        FORCE_MODE_IMPULSLE,
        FORCE_MODE_VELOCTY_CHANGE,
        FORCE_MODE_ACCELERATION
    };

    /**
     * Constructor.
     */
    PhysicsRigidBody();
    
    /**
     * Destructor.
     */
    ~PhysicsRigidBody();

    /**
     * Sets the rigid body's mass in kilograms.
     *
     * @param mass The rigid body's mass in kilograms.
     */
    void setMass(float mass) const;

    /**
     * Gets the rigid body's mass in kilograms.
     *
     * @return The rigid body's mass in kilograms.
     */
    float getMass() const;

    /**
     * Sets the rigid body's velocity vector.
     *
     * @param linearVelocity The rigid body's velocity vector
     */
    void setLinearVelocity(float linearVelocity) const;

    /**
     * Gets the rigid body's amount of air resistance when moving from force.
     *
     * @return The rigid body's amount of air resistance when moving from force.
     */
    float getLinearVelocity() const;

    /**
     * Sets the rigid body's amount of air resistance when rotating from torque.
     *
     * @param angularVelocity The rigid body's amount of air resistance when rotating from torque.
     */
    void setAngularVelocity(float angularVelocity) const;

    /**
     * Gets the rigid body's amount of air resistance when rotating from torque.
     *
     * @return The rigid body's amount of air resistance when rotating from torque.
     */
    float getAngularVelocity() const;

    /**
     * Sets the rigid body's amount of air resistance when moving from force.
     *
     * @param linearDrag The rigid body's amount of air resistance when moving from force.
     */
    void setLinearDrag(float linearDrag) const;

    /**
     * Gets the rigid body's amount of air resistance when moving from force.
     *
     * @return The rigid body's amount of air resistance when moving from force.
     */
    float getLinearDrag() const;

    /**
     * Sets the rigid body's amount of air resistance when rotating from torque.
     *
     * @param angularDrag The rigid body's amount of air resistance when rotating from torque.
     */
    void setAngularDrag(float angularDrag) const;

    /**
     * Gets the rigid body's amount of air resistance when rotating from torque.
     *
     * @return The rigid body's amount of air resistance when rotating from torque.
     */
    float getAngularDrag() const;

    /**
     * Sets whether the rigid body is affected by gravity.
     *
     * @param gravityEnabled true if the rigid body is affected by gravity, false if not.
     */
    void setGravityEnabled(bool gravityEnabled);

    /**
     * Determines whether the rigid body is affected by gravity.
     *
     * @return true if the rigid body is affected by gravity, false if not.
     */
    bool isGravityEnabled() const;

    /**
     * Sets whether physics affects the rigidbody.
     *
     * @param kinematic true if the rigid body is kinematic, false if not.
     */
    void setKinematic(bool kinematic);

    /**
     * Determines whether physics affects the rigidbody.
     *
     * @return true if the rigid body is kinematic, false if not.
     */
    bool isKinematic() const;

    /**
     * Sets the restrictions on the rigid body for freezing position and rotation.
     *
     * @param freezeConstraints The restrictions on the rigid body for freezing position and rotation.
     */
    void setFreezeConstraint(uint32_t freezeConstraints);

    /**
     * Gets the restrictions on the rigid body for freezing position and rotation.
     *
     * @return The restrictions on the rigid body for freezing position and rotation.
     */
    uint32_t getFreezeConstraint() const;

    /**
     * Gets the rigid body's center of mass in kilograms.
     *
     * If no custom center of mass is set.
     *
     * @return The rigid body's center of mass in kilograms.
     */
    float getMassCenter() const;

    /**
     * Adds force to the rigid body.
     *
     * This auto wakes if sleeping unless force is (0, 0, 0).
     *
     * @param force The force vector to be applied.
     * @param mode The mode for the force to be applied.
     */
    void addForce(const Vector3& force, PhysicsRigidBody::ForceMode mode);

    /**
     * Clear any force that is on the rigid body.
     *
     * @param mode The mode for the force to be cleared.
     */
    void clearForce(PhysicsRigidBody::ForceMode mode);

    /**
     * Adds torque to the rigid body.
     *
     * This auto wakes if sleeping unless force is (0, 0, 0).
     *
     * @param force The force vector to be applied.
     * @param mode The mode for the force to be applied.
     */
    void addTorque(const Vector3& force, PhysicsRigidBody::ForceMode mode);

    /**
     * Clear any torque that is on the rigid body.
     *
     * @param mode The mode for the torque to be cleared.
     */
    void clearTorque(PhysicsRigidBody::ForceMode mode);

    /**
     * @see Component::getTypeId
     */
    Component::TypeId getTypeId();

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
    
    /**
     * @see Serializer::Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();
    
};

}
