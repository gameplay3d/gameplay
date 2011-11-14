/*
 * PhysicsRigidBody.cpp
 */

#include "Base.h"
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsMotionState.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{

PhysicsRigidBody::PhysicsRigidBody(Node* node, PhysicsRigidBody::Type type, float mass, 
        float friction, float restitution, float linearDamping, float angularDamping)
        : _shape(NULL), _body(NULL), _node(node), _listeners(NULL), _angularVelocity(NULL),
        _anisotropicFriction(NULL), _gravity(NULL), _linearVelocity(NULL)
{
    switch (type)
    {
        case PhysicsRigidBody::SHAPE_BOX:
        {
            const BoundingBox& box = node->getModel()->getMesh()->getBoundingBox();

            PhysicsController* physics = Game::getInstance()->getPhysicsController();
            _shape = physics->getBox(box.min, box.max, btVector3(node->getScaleX(), node->getScaleY(), node->getScaleZ()));
            
            // Use the center of the bounding box as the center of mass offset.
            Vector3 c(box.min, box.max);
            c.scale(0.5f);
            c.add(box.min);
            c.negate();

            if (c.lengthSquared() > MATH_EPSILON)
                _body = createBulletRigidBody(_shape, mass, node, friction, restitution, linearDamping, angularDamping, &c);
            else
                _body = createBulletRigidBody(_shape, mass, node, friction, restitution, linearDamping, angularDamping);

            break;
        }
        case PhysicsRigidBody::SHAPE_SPHERE:
        {
            const BoundingSphere& sphere = node->getModel()->getMesh()->getBoundingSphere();

            PhysicsController* physics = Game::getInstance()->getPhysicsController();
            _shape = physics->getSphere(sphere.radius, btVector3(node->getScaleX(), node->getScaleY(), node->getScaleZ()));

            // Use the center of the bounding sphere as the center of mass offset.
            Vector3 c(sphere.center);
            c.negate();

            if (c.lengthSquared() > MATH_EPSILON)
                _body = createBulletRigidBody(_shape, mass, node, friction, restitution, linearDamping, angularDamping, &c);
            else
                _body = createBulletRigidBody(_shape, mass, node, friction, restitution, linearDamping, angularDamping);

            break;
        }
    }

    // Add the rigid body to the physics world.
    Game::getInstance()->getPhysicsController()->addRigidBody(this);
}

PhysicsRigidBody::~PhysicsRigidBody()
{
    // Clean up all constraints linked to this rigid body.
    PhysicsConstraint* ptr = NULL;
    while (_constraints.size() > 0)
    {
        ptr = _constraints.back();
        _constraints.pop_back();
        SAFE_DELETE(ptr);
    }

    // Clean up the rigid body and its related objects.
    if (_body)
    {
        if (_body->getMotionState())
            delete _body->getMotionState();

        if (_shape)
            SAFE_DELETE(_shape);

        Game::getInstance()->getPhysicsController()->removeRigidBody(this);
        SAFE_DELETE(_body);
    }

    SAFE_DELETE(_listeners);
    SAFE_DELETE(_angularVelocity);
    SAFE_DELETE(_anisotropicFriction);
    SAFE_DELETE(_gravity);
    SAFE_DELETE(_linearVelocity);
}

void PhysicsRigidBody::addCollisionListener(Listener* listener, PhysicsRigidBody* body)
{
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    listener->_rbA = this;
    if (body)
        listener->_rbB = body;

    _listeners->push_back(listener);
}

void PhysicsRigidBody::applyForce(const Vector3& force, const Vector3* relativePosition)
{
    // If the force is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the force.
    if (force.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        if (relativePosition)
            _body->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativePosition->x, relativePosition->y, relativePosition->z));
        else
            _body->applyCentralForce(btVector3(force.x, force.y, force.z));
    }
}

void PhysicsRigidBody::applyImpulse(const Vector3& impulse, const Vector3* relativePosition)
{
    // If the impulse is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the impulse.
    if (impulse.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();

        if (relativePosition)
        {
            _body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relativePosition->x, relativePosition->y, relativePosition->z));
        }
        else
            _body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
    }
}

void PhysicsRigidBody::applyTorque(const Vector3& torque)
{
    // If the torque is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorque(btVector3(torque.x, torque.y, torque.z));
    }
}

void PhysicsRigidBody::applyTorqueImpulse(const Vector3& torque)
{
    // If the torque impulse is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque impulse.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
    }
}

btRigidBody* PhysicsRigidBody::createBulletRigidBody(btCollisionShape* shape, float mass, Node* node,
    float friction, float restitution, float linearDamping, float angularDamping, const Vector3* centerOfMassOffset)
{
    // If the mass is non-zero, then the object is dynamic
    // and we need to calculate the local inertia.
    btVector3 localInertia(0.0, 0.0, 0.0);
    if (mass != 0.0)
        shape->calculateLocalInertia(mass, localInertia);

    // Create the Bullet physics rigid body object.
    PhysicsMotionState* motionState = new PhysicsMotionState(node, centerOfMassOffset);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
    rbInfo.m_friction = friction;
    rbInfo.m_restitution = restitution;
    rbInfo.m_linearDamping = linearDamping;
    rbInfo.m_angularDamping = angularDamping;
    btRigidBody* body = new btRigidBody(rbInfo);

    return body;
}

void PhysicsRigidBody::addConstraint(PhysicsConstraint* constraint)
{
    _constraints.push_back(constraint);
}

void PhysicsRigidBody::removeConstraint(PhysicsConstraint* constraint)
{
    for (unsigned int i = 0; i < _constraints.size(); i++)
    {
        if (_constraints[i] == constraint)
        {
            _constraints.erase(_constraints.begin() + i);
            break;
        }
    }
}

PhysicsRigidBody::Listener::Listener()
    : _rbA(NULL), _rbB(NULL)
{
    // DUMMY FUNCTION
}

btScalar PhysicsRigidBody::Listener::addSingleResult(btManifoldPoint& cp, const btCollisionObject* a,
    int partIdA, int indexA, const btCollisionObject* b, int partIdB, int indexB)
{
    PhysicsRigidBody* rbA = Game::getInstance()->getPhysicsController()->getPhysicsRigidBody(a);
    PhysicsRigidBody* rbB = Game::getInstance()->getPhysicsController()->getPhysicsRigidBody(b);

    if (rbA == _rbA)
        collisionEvent(rbB, Vector3(cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z()));
    else
        collisionEvent(rbA, Vector3(cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z()));

    return 0.0f;
}

}
