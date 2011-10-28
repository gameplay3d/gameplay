/*
 * PhysicsRigidBody.h
 */

#ifndef PHYSICSRIGIDBODY_H_
#define PHYSICSRIGIDBODY_H_

#include "Mesh.h"
#include "Ref.h"
#include "Transform.h"
#include "Vector3.h"

namespace gameplay
{

class Node;

/**
 * Defines a class for physics rigid bodies.
 */
class PhysicsRigidBody : public Ref
{
public:

	enum Type
	{
		PHYSICS_SHAPE_BOX,
		PHYSICS_SHAPE_SPHERE,
		PHYSICS_SHAPE_TRIANGLE_MESH,
		PHYSICS_SHAPE_HEIGHTFIELD,
        PHYSICS_SHAPE_NONE
	};

    void applyForce(const Vector3& force, const Vector3* relativePosition = NULL);
	void applyImpulse(const Vector3& impulse, const Vector3* relativePosition = NULL);
	void applyTorque(const Vector3& torque);
    void applyTorqueImpulse(const Vector3& torque);

    inline void setFriction(float friction);
    inline void setRestitution(float restitution);
    inline void setAnisotropicFriction(const Vector3& friction);
    inline void setGravity(const Vector3& gravity);
	inline float getFriction();
    inline float getRestitution();
    inline Vector3 getAnisotropicFriction();
    inline Vector3 getGravity();

    inline void setLinearVelocity(const Vector3& velocity);
	inline void setAngularVelocity(const Vector3& velocity);

	inline Vector3 getLinearVelocity();
	inline Vector3 getAngularVelocity();

	inline void setDamping(float linearDamping, float angularDamping);
	inline float getLinearDamping();
	inline float getAngularDamping();

private:

	friend class Node;
	friend class PhysicsMotionState;

    PhysicsRigidBody(Node* node, PhysicsRigidBody::Type type, float mass, 
		float friction = 0.5, float restitution = 0.0, float linearDamping = 0.0, float angularDamping = 0.0);
    ~PhysicsRigidBody();

    /**
     * Private copy constructor to disallow copying.
     */
    PhysicsRigidBody(const PhysicsRigidBody& body) {}

	static btRigidBody* createBulletRigidBody(btCollisionShape* shape, float mass, Node* node,
        float friction, float restitution, float linearDamping, float angularDamping);

	btRigidBody* _body;
    Node* _node;
};

// Inline functions.

void PhysicsRigidBody::setFriction(float friction)
{
    _body->setFriction(friction);
}

void PhysicsRigidBody::setRestitution(float restitution)
{
    _body->setRestitution(restitution);
}

float PhysicsRigidBody::getFriction()
{
    return _body->getFriction();
}

float PhysicsRigidBody::getRestitution()
{
    return _body->getRestitution();
}

void PhysicsRigidBody::setLinearVelocity(const Vector3& velocity)
{
	_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

void PhysicsRigidBody::setAngularVelocity(const Vector3& velocity)
{
	_body->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

// TODO: We can't cache these Vector3 values, but should we have member variables
// that are set each time instead of creating a new Vector3 every time?
Vector3 PhysicsRigidBody::getLinearVelocity()
{
	const btVector3& v = _body->getLinearVelocity();
	return Vector3(v.x(), v.y(), v.z());
}

Vector3 PhysicsRigidBody::getAngularVelocity()
{
	const btVector3& v = _body->getAngularVelocity();
	return Vector3(v.x(), v.y(), v.z());
}

Vector3 PhysicsRigidBody::getAnisotropicFriction()
{
    const btVector3& af = _body->getAnisotropicFriction();
    return Vector3(af.x(), af.y(), af.z());
}

Vector3 PhysicsRigidBody::getGravity()
{
    const btVector3& g = _body->getGravity();
    return Vector3(g.x(), g.y(), g.z());
}

void PhysicsRigidBody::setDamping(float linearDamping, float angularDamping)
{
    _body->setDamping(linearDamping, angularDamping);
}

float PhysicsRigidBody::getLinearDamping()
{
	return _body->getLinearDamping();
}

float PhysicsRigidBody::getAngularDamping()
{
	return _body->getAngularDamping();
}

void PhysicsRigidBody::setAnisotropicFriction(const Vector3& friction)
{
    _body->setAnisotropicFriction(btVector3(friction.x, friction.y, friction.z));
}

void PhysicsRigidBody::setGravity(const Vector3& gravity)
{
    _body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

}

#endif
