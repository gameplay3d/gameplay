#ifndef PHYSICSCONSTRAINT_H_
#define PHYSICSCONSTRAINT_H_

#include "Base.h"
#include "Vector3.h"

namespace gameplay
{
    class Node;
    class PhysicsRigidBody;

/**
 * Defines the base class for physics constraints.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsConstraint
{
    friend class PhysicsController;
    friend class PhysicsRigidBody;

public:
    /**
     * Gets the impulse needed to break the constraint.
     * 
     * @return The impulse needed to break the constraint.
     */
    inline float getBreakingImpulse() const;

    /**
     * Sets the impulse needed to break the constraint
     * (if an impulse greater than or equal to the given
     * value is applied to the constraint, the constraint
     * will be broken).
     * 
     * @param impulse The impulse needed to break the constraint.
     */
    inline void setBreakingImpulse(float impulse);

    /**
     * Gets whether the constraint is enabled or not.
     * 
     * @return Whether the constraint is enabled or not.
     */
    inline bool isEnabled() const;

    /**
     * Sets whether the constraint is enabled or not.
     * 
     * @param enabled Whether the constraint is enabled or not.
     */
    inline void setEnabled(bool enabled);

    /**
     * Calculates the midpoint between the given nodes' centers of mass.
     * 
     * @param a The first node.
     * @param b The second node.
     */
    static Vector3 centerOfMassMidpoint(const Node* a, const Node* b);

    /**
     * Calculates the rotation offset to the given point in the given node's local space.
     * 
     * @param node The node to calculate a rotation offset for.
     * @param point The point to calculate the rotation offset to.
     */
    static Quaternion getRotationOffset(const Node* node, const Vector3& point);

    /**
     * Calculates the translation offset to the given point in the given node's local space.
     * 
     * @param node The node to calculate a translation offset for.
     * @param point The point to calculate the translation offset to.
     */
    static Vector3 getTranslationOffset(const Node* node, const Vector3& point);

protected:

    /**
     * Constructor.
     */
    PhysicsConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

    /**
     * Destructor.
     */
    virtual ~PhysicsConstraint();

    /**
     * Calculates the transform to be used as the offset (i.e. "frame in" 
     * parameter in Bullet terms) to the given constraint origin.
     */
    static btTransform getTransformOffset(const Node* node, const Vector3& origin);
    
    /**
     * Calculates the center of mass in world space of the given node.
     */
    static Vector3 getWorldCenterOfMass(const Node* node);

    /**
     * Offsets the given vector by the given node's center of mass.
     */
    static Vector3 offsetByCenterOfMass(const Node* node, const Vector3& v);

    /**
     * Pointer to the one rigid body bound by this constraint.
     */
    PhysicsRigidBody* _a;
    
    /**
     * Pointer to the other rigid body bound by this constraint.
     */
    PhysicsRigidBody* _b;
    
    /**
     * Pointer to the Bullet constraint.
     */
    btTypedConstraint* _constraint;
};

}

#include "PhysicsConstraint.inl"

#endif
