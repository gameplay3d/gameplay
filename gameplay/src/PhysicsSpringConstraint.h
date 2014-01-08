#ifndef PHYSICSSPRINGCONSTRAINT_H_
#define PHYSICSSPRINGCONSTRAINT_H_

#include "PhysicsGenericConstraint.h"

namespace gameplay
{

/**
 * Defines a generic spring constraint between two
 * rigid bodies (or one rigid body and the world)
 * where the spring strength and damping can be set
 * for all six degrees of freedom.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Constraints
 */
class PhysicsSpringConstraint : public PhysicsGenericConstraint
{
    friend class PhysicsController;

public:

    /**
     * Sets the angular damping along the constraint's local X axis.
     * 
     * @param damping The angular damping value.
     */
    inline void setAngularDampingX(float damping);

    /**
     * Sets the angular damping along the constraint's local Y axis.
     * 
     * @param damping The angular damping value.
     */
    inline void setAngularDampingY(float damping);

    /**
     * Sets the angular damping along the constraint's local Z axis.
     * 
     * @param damping The angular damping value.
     */
    inline void setAngularDampingZ(float damping);

    /**
     * Sets the angular strength along the constraint's local X axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for angular movement about the X axis (setting to zero disables it).
     * 
     * @param strength The angular strength value.
     */
    inline void setAngularStrengthX(float strength);

    /**
     * Sets the angular strength along the constraint's local Y axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for angular movement about the Y axis (setting to zero disables it).
     * 
     * @param strength The angular strength value.
     */
    inline void setAngularStrengthY(float strength);

    /**
     * Sets the angular strength along the constraint's local Z axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for angular movement about the Z axis (setting to zero disables it).
     * 
     * @param strength The angular strength value.
     */
    inline void setAngularStrengthZ(float strength);

    /**
     * Sets the linear damping along the constraint's local X axis.
     * 
     * @param damping The linear damping value.
     */
    inline void setLinearDampingX(float damping);

    /**
     * Sets the linear damping along the constraint's local Y axis.
     * 
     * @param damping The linear damping value.
     */
    inline void setLinearDampingY(float damping);

    /**
     * Sets the linear damping along the constraint's local Z axis.
     * 
     * @param damping The linear damping value.
     */
    inline void setLinearDampingZ(float damping);

    /**
     * Sets the linear strength along the constraint's local X axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for linear movement along the X axis (setting to zero disables it).
     * 
     * @param strength The linear strength value.
     */
    inline void setLinearStrengthX(float strength);

    /**
     * Sets the linear strength along the constraint's local Y axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for linear movement along the Y axis (setting to zero disables it).
     * 
     * @param strength The linear strength value.
     */
    inline void setLinearStrengthY(float strength);

    /**
     * Sets the linear strength along the constraint's local Z axis.
     *
     * Note: setting the strength to a non-zero value enables the
     * spring for linear movement along the Z axis (setting to zero disables it).
     * 
     * @param strength The linear strength value.
     */
    inline void setLinearStrengthZ(float strength);

private:

    // Represents the different properties that
    // can be set on the spring constraint.
    // 
    // (Note: the values map to the index parameter
    // used in the member functions of the Bullet
    // class btGeneric6DofSpringConstraint.)
    enum SpringProperty
    {
        LINEAR_X = 0,
        LINEAR_Y,
        LINEAR_Z,
        ANGULAR_X,
        ANGULAR_Y,
        ANGULAR_Z
    };

    /**
     * Creates a spring constraint so that the rigid body (or bodies) is
     * (are) constrained using its (their) current world position(s) for
     * the translation offset(s) to the constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param b The second rigid body to constrain (optional).
     */
    PhysicsSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b);

    /**
     * Creates a spring constraint.
     * 
     * @param a The first (possibly only) rigid body to constrain. If this is the only rigid
     *      body specified the constraint applies between it and the global physics world object.
     * @param rotationOffsetA The rotation offset for the first rigid body 
     *      (in its local space) with respect to the constraint joint.
     * @param translationOffsetA The translation offset for the first rigid body
     *      (in its local space) with respect to the constraint joint.
     * @param b The second rigid body to constrain (optional).
     * @param rotationOffsetB The rotation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     * @param translationOffsetB The translation offset for the second rigid body
     *      (in its local space) with respect to the constraint joint (optional).
     */
    PhysicsSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
                            PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB);

    /**
     * Destructor.
     */
    ~PhysicsSpringConstraint();

    // Sets the strength for the given angular/linear 
    // X/Y/Z axis combination determined by the given index.
    // 
    // See the Bullet class btGeneric6DofSpringConstraint
    // for more information.
    void setStrength(SpringProperty property, float strength);

    // Sets the damping for the given angular/linear 
    // X/Y/Z axis combination determined by the given index.
    // 
    // See the Bullet class btGeneric6DofSpringConstraint
    // for more information.
    void setDamping(SpringProperty property, float damping);
};

}

#include "PhysicsSpringConstraint.inl"

#endif
