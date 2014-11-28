#ifndef PHYSICSRIGIDBODY_H_
#define PHYSICSRIGIDBODY_H_

#include "Mesh.h"
#include "PhysicsConstraint.h"
#include "Transform.h"
#include "Vector3.h"
#include "PhysicsCollisionObject.h"

namespace gameplay
{

class Node;
class PhysicsConstraint;

/**
 * Defines a class for physics rigid bodies.
 *
 * A rigid body can receive forces and torque to make your objects react to other collision
 * objects around it.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-Collision_Objects
 */
class PhysicsRigidBody : public PhysicsCollisionObject, public Transform::Listener
{
    friend class Node;
    friend class PhysicsCharacter;
    friend class PhysicsVehicle;
    friend class PhysicsVehicleWheel;
    friend class PhysicsConstraint;
    friend class PhysicsController;
    friend class PhysicsFixedConstraint;
    friend class PhysicsGenericConstraint;
    friend class PhysicsHingeConstraint;
    friend class PhysicsSocketConstraint;
    friend class PhysicsSpringConstraint;

public:

    /**
     * Rigid body construction parameters.
     */
    struct Parameters
    {
        /**
         * The mass of the rigid body, in kilograms.
         */
        float mass;

        /**
         * The friction of the rigid body (non-zero values give best simulation results).
         */
        float friction;

        /**
         * The restitution of the rigid body (this controls the bounciness of
         * the rigid body; use zero for best simulation results).
         */
        float restitution;

        /**
         * The percentage of linear velocity lost per second (between 0.0 and 1.0).
         */
        float linearDamping;

        /**
         * The percentage of angular velocity lost per second (between 0.0 and 1.0).
         */
        float angularDamping;

        /**
         * Whether the rigid body is kinematic.
         */
        bool kinematic;

        /**
         * The anisotropic friction term for the rigid body.
         */
        Vector3 anisotropicFriction;

        /**
         * Linear factor for the rigid body. x, y, z coordinates correspond to world 
         * space motion along these axes. Use 1.0 to allow or 0.0 to disallow motion 
         * along certain axis.
         */
        Vector3 linearFactor;

        /**
         * Angular factor for the rigid body. x, y, z coordinates correspond to world 
         * space rotation along these axes. Use 1.0 to allow or 0.0 to disallow rotation
         * along certain axis.
         */
        Vector3 angularFactor;

        /**
         * Constructor.
         */
        Parameters() : mass(0.0f), friction(0.5f), restitution(0.0f),
            linearDamping(0.0f), angularDamping(0.0f),
            kinematic(false), anisotropicFriction(Vector3::one()), linearFactor(Vector3::one()), angularFactor(Vector3::one())
        {
        }

        /**
         * Constructor.
         */
        Parameters(float mass, float friction = 0.5f, float restitution = 0.0f,
            float linearDamping = 0.0f, float angularDamping = 0.0f, bool kinematic = false,
            const Vector3& anisotropicFriction = Vector3::one(), const Vector3& linearFactor = Vector3::one(), 
            const Vector3& angularFactor = Vector3::one())
            : mass(mass), friction(friction), restitution(restitution), linearDamping(linearDamping), angularDamping(angularDamping),
              kinematic(kinematic), anisotropicFriction(anisotropicFriction), linearFactor(linearFactor), angularFactor(angularFactor)
        {
        }
    };

    /**
     * @see PhysicsCollisionObject::getType
     */
    PhysicsCollisionObject::Type getType() const;

    /**
     * Gets the rigid body's mass.
     *
     * @return The mass.
     */
    inline float getMass() const;

    /**
     * Gets the rigid body's friction.
     * 
     * @return The friction.
     */
    inline float getFriction() const;

    /**
     * Sets the rigid body's friction.
     * 
     * @param friction The friction.
     */
    inline void setFriction(float friction);

    /**
     * Gets the rigid body's restitution.
     *
     * @return The restitution.
     */
    inline float getRestitution() const;

    /**
     * Sets the rigid body's restitution (or bounciness).
     * 
     * @param restitution The restitution.
     */
    inline void setRestitution(float restitution);

    /**
     * Gets the rigid body's linear damping.
     *
     * @return The linear damping.
     */
    inline float getLinearDamping() const;

    /**
     * Gets the rigid body's angular damping.
     *
     * @return The angular damping.
     */
    inline float getAngularDamping() const;

    /**
     * Sets the rigid body's linear and angular damping.
     * 
     * @param linearDamping The linear damping; between 0.0 (minimum) and 1.0 (maximum).
     * @param angularDamping The angular damping; between 0.0 (minimum) and 1.0 (maximum).
     */
    inline void setDamping(float linearDamping, float angularDamping);

    /**
     * Gets the rigid body's linear velocity.
     * 
     * @return The linear velocity.
     */
    inline Vector3 getLinearVelocity() const;

    /**
     * Sets the rigid body's linear velocity.
     * 
     * @param velocity The linear velocity.
     */
    inline void setLinearVelocity(const Vector3& velocity);

    /**
     * Sets the rigid body's linear velocity.
     * 
     * @param x The x coordinate of the linear velocity vector.
     * @param y The y coordinate of the linear velocity vector.
     * @param z The z coordinate of the linear velocity vector.
     */
    inline void setLinearVelocity(float x, float y, float z);

    /**
     * Gets the rigid body's angular velocity.
     * 
     * @return The angular velocity.
     */
    inline Vector3 getAngularVelocity() const;

    /**
     * Sets the rigid body's angular velocity.
     * 
     * @param velocity The angular velocity.
     */
    inline void setAngularVelocity(const Vector3& velocity);

    /**
     * Sets the rigid body's angular velocity.
     * 
     * @param x The x coordinate of the angular velocity vector.
     * @param y The y coordinate of the angular velocity vector.
     * @param z The z coordinate of the angular velocity vector.
     */
    inline void setAngularVelocity(float x, float y, float z);

    /**
     * Gets the rigid body's anisotropic friction.
     * 
     * @return The anisotropic friction.
     */
    inline Vector3 getAnisotropicFriction() const;

    /**
     * Sets the rigid body's anisotropic friction.
     * 
     * @param friction The anisotropic friction.
     */
    inline void setAnisotropicFriction(const Vector3& friction);

    /**
     * Sets the rigid body's anisotropic friction.
     * 
     * @param x The x coordinate of the anisotropic friction.
     * @param y The y coordinate of the anisotropic friction.
     * @param z The z coordinate of the anisotropic friction.
     */
    inline void setAnisotropicFriction(float x, float y, float z);

    /**
     * Gets the gravity that affects the rigid body (this can
     * be different from the global gravity; @see setGravity(Vector3)).
     * 
     * @return The gravity.
     */
    inline Vector3 getGravity() const;

    /**
     * Sets the rigid body's gravity (this overrides the global gravity for this rigid body).
     * 
     * @param gravity The gravity.
     */
    inline void setGravity(const Vector3& gravity);

    /**
     * Sets the rigid body's gravity (this overrides the global gravity for this rigid body).
     * 
     * @param x The x coordinate of the gravity vector.
     * @param y The y coordinate of the gravity vector.
     * @param z The z coordinate of the gravity vector.
     */
    inline void setGravity(float x, float y, float z);

    /**
     * Gets the rigid body's angular factor.
     * 
     * @return The angular factor.
     */
    inline Vector3 getAngularFactor() const;

    /**
     * Sets the rigid body's angular factor.  x, y, z coordinates correspond to world 
     * space rotation along these axes. Use 1.0 to allow or 0.0 to disallow rotation 
     * along certain axis.
     * 
     * @param angularFactor angular factor vector
     */
    inline void setAngularFactor(const Vector3& angularFactor);

    /**
     * Sets the rigid body's angular factor.  x, y, z coordinates correspond to world 
     * space rotation along these axes. Use 1.0 to allow or 0.0 to disallow rotation 
     * along certain axis.
     * 
     * @param x The x coordinate of the angular factor vector.
     * @param y The y coordinate of the angular factor vector.
     * @param z The z coordinate of the angular factor vector.
     */
    inline void setAngularFactor(float x, float y, float z);

    /**
     * Gets the rigid body's linear factor.
     * 
     * @return The linear factor.
     */
    inline Vector3 getLinearFactor() const;

    /**
     * Sets the rigid body's linear factor.  x, y, z coordinates correspond to world 
     * space motion along these axes. Use 1.0 to allow or 0.0 to disallow motion 
     * along certain axis.
     * 
     * @param linearFactor linear factor vector
     */
    inline void setLinearFactor(const Vector3& linearFactor);

    /**
     * Sets the rigid body's linear factor.  x, y, z coordinates correspond to world 
     * space motion along these axes. Use 1.0 to allow or 0.0 to disallow motion 
     * along certain axis.
     * 
     * @param x The x coordinate of the linear factor vector.
     * @param y The y coordinate of the linear factor vector.
     * @param z The z coordinate of the linear factor vector.
     */
    inline void setLinearFactor(float x, float y, float z);

    /**
     * Sets whether the rigid body is a kinematic rigid body or not.
     * 
     * @param kinematic Whether the rigid body is kinematic or not.
     */
    void setKinematic(bool kinematic);

    /**
     * Sets whether the rigid body is enabled or disabled in the physics world.
     *
     * @param enable true enables the collision object, false disables it.
     */
    void setEnabled(bool enable);

    /**
     * Gets the height at the given point (only for rigid bodies of type HEIGHTFIELD).
     * 
     * @param x The x position, in world space.
     * @param z The z position, in world space.
     * @return The height at the given point, or zero if this is not a heightfield rigid body.
     */
    float getHeight(float x, float z) const;

    /**
     * Gets whether the rigid body is a static rigid body or not.
     *
     * @return Whether the rigid body is static.
     */
    bool isStatic() const;

    /**
     * Applies the given force to the rigid body (optionally, from the given relative position).
     * Note that the total force applied depends on the duration of the next frame.
     * If you want to apply an "impulse" irrespective of the frame duration, consider using applyImpulse.
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

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

private:

    /**
     * Creates a rigid body.
     * 
     * @param node The node to create a rigid body for; note that the node must have
     *      a model attached to it prior to creating a rigid body for it.
     * @param shape The rigid body shape construction information.
     * @param parameters The rigid body construction parameters.
     * @param group Group identifier
     * @param mask Bitmask field for filtering collisions with this object.
     */
    PhysicsRigidBody(Node* node, const PhysicsCollisionShape::Definition& shape, const Parameters& parameters, int group = PHYSICS_COLLISION_GROUP_DEFAULT, int mask = PHYSICS_COLLISION_MASK_DEFAULT);

    /**
     * Destructor.
     */
    ~PhysicsRigidBody();

    /**
     * Private copy constructor to disallow copying.
     */
    PhysicsRigidBody(const PhysicsRigidBody& body);

    /**
     * Creates a rigid body from the specified properties object.
     * 
     * @param node The node to create a rigid body for; note that the node must have a model attached to it prior to creating a rigid body for it.
     * @param properties The properties object defining the rigid body.
     * @param nspace The namespace expected (default is "RIGID_BODY").
     * @return The newly created rigid body, or <code>NULL</code> if the rigid body failed to load.
     */
    static PhysicsRigidBody* create(Node* node, Properties* properties, const char* nspace = "RIGID_BODY");

    // Adds a constraint to this rigid body.
    void addConstraint(PhysicsConstraint* constraint);

    // Removes a constraint from this rigid body (used by the constraint destructor).
    void removeConstraint(PhysicsConstraint* constraint);

    // Whether or not the rigid body supports constraints fully.
    bool supportsConstraints();

    // Used for implementing getHeight() when the heightfield has a transform that can change.
    void transformChanged(Transform* transform, long cookie);

    btRigidBody* _body;
    float _mass;
    std::vector<PhysicsConstraint*>* _constraints;
    bool _inDestructor;

};

}

#include "PhysicsRigidBody.inl"

#endif
