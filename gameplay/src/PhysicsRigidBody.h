/*
 * PhysicsRigidBody.h
 */

#ifndef PHYSICSRIGIDBODY_H_
#define PHYSICSRIGIDBODY_H_

#include "Mesh.h"
#include "PhysicsConstraint.h"
#include "Ref.h"
#include "Transform.h"
#include "Vector3.h"

namespace gameplay
{

class Node;
class PhysicsConstraint;

/**
 * Defines a class for physics rigid bodies.
 */
class PhysicsRigidBody : public Ref
{
    friend class Node;
    friend class PhysicsConstraint;
    friend class PhysicsController;
    friend class PhysicsFixedConstraint;
    friend class PhysicsGenericConstraint;
    friend class PhysicsHingeConstraint;
    friend class PhysicsSocketConstraint;
    friend class PhysicsSpringConstraint;

public:
    /**
     * Represents the different types of rigid bodies.
     */
    enum Type
    {
        SHAPE_BOX,
        SHAPE_SPHERE,
        SHAPE_TRIANGLE_MESH,
        SHAPE_HEIGHTFIELD,
        SHAPE_NONE
    };

    /**
     * Applies the given force to the rigid body (optionally, from the given relative position).
     * 
     * @param force The force to be applied.
     * @param relativePosition The relative position from which to apply the force.
     */
    void applyForce(const Vector3& force, const Vector3* relativePosition = NULL);

    /**
     * Applies the given force impulse to the rigid body (optionally, from the given relative position).
     * 
     * @param impulse The force impulse to be applied.
     * @param relativePosition The relative position from which to apply the force.
     */
    void applyImpulse(const Vector3& impulse, const Vector3* relativePosition = NULL);

    /**
     * Applies the given torque to the rigid body.
     * 
     * @param torque The torque to be applied.
     */
    void applyTorque(const Vector3& torque);

    /**
     * Applies the given torque impulse to the rigid body.
     * 
     * @param torque The torque impulse to be applied.
     */
    void applyTorqueImpulse(const Vector3& torque);
    
    /**
     * Gets the rigid body's angular damping.
     * 
     * @return The angular damping.
     */
    inline float getAngularDamping() const;

    /**
     * Gets the rigid body's angular velocity.
     * 
     * @return The angular velocity.
     */
    inline Vector3 getAngularVelocity() const;

    /**
     * Gets the rigid body's anisotropic friction.
     * 
     * @return The anisotropic friction.
     */
    inline Vector3 getAnisotropicFriction() const;

    /**
     * Gets the rigid body's friction.
     * 
     * @return The friction.
     */
    inline float getFriction() const;

    /**
     * Gets the gravity that affects the rigid body (this can
     * be different from the global gravity; @see #setGravity).
     * 
     * @return The gravity.
     */
    inline Vector3 getGravity() const;

    /**
     * Gets the rigid body's linear damping.
     * 
     * @return The linear damping.
     */
    inline float getLinearDamping() const;

    /**
     * Gets the rigid body's linear velocity.
     * 
     * @return The linear velocity.
     */
    inline Vector3 getLinearVelocity() const;

    /**
     * Gets the rigid body's restitution.
     * 
     * @return The restitution.
     */
    inline float getRestitution() const;

    /**
     * Sets the rigid body's angular velocity.
     * 
     * @param velocity The angular velocity.
     */
    inline void setAngularVelocity(const Vector3& velocity);

    /**
     * Sets the rigid body's anisotropic friction.
     * 
     * @param friction The anisotropic friction.
     */
    inline void setAnisotropicFriction(const Vector3& friction);

    /**
     * Sets the rigid body's linear and angular damping.
     * 
     * @param linearDamping The linear damping; between 0.0 (minimum) and 1.0 (maximum).
     * @param angularDamping The angular damping; between 0.0 (minimum) and 1.0 (maximum).
     */
    inline void setDamping(float linearDamping, float angularDamping);

    /**
     * Sets the rigid body's friction.
     * 
     * @param friction The friction.
     */
    inline void setFriction(float friction);

    /**
     * Sets the rigid body's gravity (this overrides the global gravity for this rigid body).
     * 
     * @param gravity The gravity.
     */
    inline void setGravity(const Vector3& gravity);

    /**
     * Sets the rigid body's linear velocity.
     * 
     * @param velocity The linear velocity.
     */
    inline void setLinearVelocity(const Vector3& velocity);

    /**
     * Sets the rigid body's restitution (or bounciness).
     * 
     * @param restitution The restitution.
     */
    inline void setRestitution(float restitution);

private:
    /**
     * Creates a rigid body.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param type The type of rigid body to set.
     * @param mass The mass of the rigid body, in kilograms.
     * @param friction The friction of the rigid body (non-zero values give best simulation results).
     * @param restitution The restitution of the rigid body (this controls the bounciness of
     *      the rigid body; use zero for best simulation results).
     * @param linearDamping The percentage of linear velocity lost per second (between 0.0 and 1.0).
     * @param angularDamping The percentage of angular velocity lost per second (between 0.0 and 1.0).
     */
    PhysicsRigidBody(Node* node, PhysicsRigidBody::Type type, float mass, float friction = 0.5,
        float restitution = 0.0, float linearDamping = 0.0, float angularDamping = 0.0);

    /**
     * Destructor.
     */
    ~PhysicsRigidBody();

    /**
     * Private copy constructor to disallow copying.
     */
    PhysicsRigidBody(const PhysicsRigidBody& body);

    // Creates the underlying Bullet Physics rigid body object
    // for a PhysicsRigidBody object using the given parameters.
    static btRigidBody* createBulletRigidBody(btCollisionShape* shape, float mass, Node* node,
        float friction, float restitution, float linearDamping, float angularDamping,
        const Vector3* centerOfMassOffset = NULL);

    // Adds a constraint to this rigid body.
    void addConstraint(PhysicsConstraint* constraint);

    // Removes a constraint from this rigid body (used by the constraint destructor).
    void removeConstraint(PhysicsConstraint* constraint);

    btCollisionShape* _shape;
    btRigidBody* _body;
    Node* _node;
    std::vector<PhysicsConstraint*> _constraints;
};

}

#include "PhysicsRigidBody.inl"

#endif
