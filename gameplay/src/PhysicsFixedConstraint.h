/*
 * PhysicsFixedConstraint.h
 */

#ifndef PHYSICSFIXEDCONSTRAINT_H_
#define PHYSICSFIXEDCONSTRAINT_H_

#include "PhysicsGenericConstraint.h"
#include "Vector3.h"

namespace gameplay
{

// TODO!
/*
// Taken from http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl.html.
bool matrixToEulerXYZ(const Matrix& m, Vector3& xyz)
{
    if (m.m[8] < 1.0f)
    {
            if (m.m[8] > -1.0f)
            {
                    xyz.x = atan2f(-m.m[5],m.m[2]);
                    xyz.y = asinf(m.m[8]);
                    xyz.z = atan2f(-m.m[4],m.m[0]);
                    return true;
            }
            else
            {
                    // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
                    xyz.x = -atan2f(m.m[12],m.m[1]);
                    xyz.y = -MATH_PIOVER2;
                    xyz.z = 0.0;
                    return false;
            }
    }
    else
    {
            // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
            xyz.x = atan2f(m.m[12],m.m[1]);
            xyz.y = MATH_PIOVER2;
            xyz.z = 0.0;
    }
    return false;
}
*/

/**
 * Represents a constraint where two rigid bodies 
 * (or one rigid body and the world) are bound together
 * with a given orientation and translation offset and all
 * movement is restricted.
 *
 * (This is similar in concept to parenting one node to another,
 * but can be used in specific situations for a more appropriate effect
 * (e.g. for implementing sticky projectiles, etc.).)
 */
class PhysicsFixedConstraint : public PhysicsGenericConstraint
{
    friend class PhysicsController;

public:
    /**
     * Sets the angular limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The angular limits along the local X, Y, and Z axes.
     */
    void setAngularLimit(const Vector3& limits)
    {
        setAngularLowerLimit(limits);
        setAngularUpperLimit(limits);
    }

    /**
     * Sets the linear limits along the constraint's local
     * X, Y, and Z axes using the values in the given vector.
     * 
     * @param limits The linear limits along the local X, Y, and Z axes.
     */
    void setLinearLimit(const Vector3& limits)
    {
        setLinearLowerLimit(limits);
        setLinearUpperLimit(limits);
    }

protected:

    void initialize()
    {
        /*
        const BoundingBox& box = joint10->getModel()->getMesh()->getBoundingBox();
        float a = 0.5f * (box.max.x - box.min.x);
    
        const BoundingSphere& sphere = _basketball->getModel()->getMesh()->getBoundingSphere();
        float b = -sphere.radius;
    
        Quaternion r1, r2;
        joint10->getWorldMatrix().getRotation(&r1);
        _basketball->getWorldMatrix().getRotation(&r2);

        Vector3 bMin, bMax;
        joint10->getWorldMatrix().transformPoint(box.min, &bMin);
        joint10->getWorldMatrix().transformPoint(box.max, &bMax);
        Vector3 c1(bMin, bMax);
        c1.scale(0.5f);
        c1.add(bMin);

        Vector3 c2;
        _basketball->getWorldMatrix().transformPoint(sphere.center, &c2);

        Vector3 d(c1, c2);
        d.scale(0.5f);

        Quaternion q;
        joint10->getWorldMatrix().getRotation(&q);
        Vector3 v;
        joint10->getWorldMatrix().getTranslation(&v);
        Transform t(Vector3::one(), q, v);
        Matrix w1(t.getMatrix());
        Matrix w1i; w1.invert(&w1i);
        Vector3 t1(d);
        w1i.transformNormal(&t1);
        t1.x *= 1.0f / joint10->getScaleX();
        t1.y *= 1.0f / joint10->getScaleY();
        t1.z *= 1.0f / joint10->getScaleZ();

        _basketball->getWorldMatrix().getRotation(&q);
        _basketball->getWorldMatrix().getTranslation(&v);
        t.set(Vector3::one(), q, v);
        Matrix w2(t.getMatrix());
        Matrix w2i; w2.invert(&w2i);
        Vector3 t2(d);
        t2.negate();
        w2i.transformNormal(&t2);
        t2.x *= 1.0f / _basketball->getScaleX();
        t2.y *= 1.0f / _basketball->getScaleY();
        t2.z *= 1.0f / _basketball->getScaleZ();

        Quaternion::createFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), MATH_DEG_TO_RAD(90.0f), &q);
    
        // TODO!
        // Construct the correct rotation limits.
        r1.inverse();
        r2.inverse();
        Matrix mm1;
        Matrix::createRotation(r1, &mm1);
        Matrix::multiply(w1, mm1, &mm1);
        mm1.invert();
        Matrix mm2;
        Matrix::createRotation(r2, &mm2);
        Matrix::multiply(w2, mm2, &mm2);
        Matrix mm;
        Matrix::multiply(mm1, mm2, &mm);
        matrixToEulerXYZ(mm, euler);

        PhysicsGenericConstraint* constraint = Game::getInstance()->getPhysicsController()->createGenericConstraint(joint10->getPhysicsRigidBody(),
            Quaternion::identity(), t1, _basketball->getPhysicsRigidBody(), Quaternion::identity(), t2);

        Vector3 rz90(0.0f, 0.0f, MATH_DEG_TO_RAD(90.0f));
        //constraint->setAngularLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
        //constraint->setAngularUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
        //constraint->setAngularLowerLimit(rz90);
        //constraint->setAngularUpperLimit(rz90);
        constraint->setAngularLowerLimit(euler);
        constraint->setAngularUpperLimit(euler);
    
        d.scale(2.0f);
        d.x *= 1.0f / _basketball->getScaleX();
        d.y *= 1.0f / _basketball->getScaleY();
        d.z *= 1.0f / _basketball->getScaleZ();;
        w1i.transformNormal(&d);
        constraint->setLinearLowerLimit(d);
        constraint->setLinearUpperLimit(d);
        //*/
    }

    /**
     * @see PhysicsGenericConstraint
     */
    PhysicsFixedConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,
        PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
        : PhysicsGenericConstraint(a, rotationOffsetA, translationOffsetA, b, rotationOffsetB, translationOffsetB)
    {
    }

    // Note: We make these functions protected to prevent usage
    // (these are public in the base class, PhysicsGenericConstraint).

    /**
     * Protected to prevent usage.
     */
    inline void setAngularLowerLimit(const Vector3& limit)
    {
        PhysicsGenericConstraint::setAngularLowerLimit(limit);
    }
    
    /**
     * Protected to prevent usage.
     */
    inline void setAngularUpperLimit(const Vector3& limit)
    {
        PhysicsGenericConstraint::setAngularUpperLimit(limit);
    }
    
    /**
     * Protected to prevent usage.
     */
    inline void setLinearLowerLimit(const Vector3& limit)
    {
        PhysicsGenericConstraint::setLinearLowerLimit(limit);
    }
    
    /**
     * Protected to prevent usage.
     */
    inline void setLinearUpperLimit(const Vector3& limit)
    {
        PhysicsGenericConstraint::setLinearUpperLimit(limit);
    }

};

}

#endif
