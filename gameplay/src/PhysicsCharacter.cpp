/**
 * PhysicsCharacter.cpp
 *
 * Much of the collision detection code for this implementation is based off the
 * btbtKinematicCharacterController class from Bullet Physics 2.7.6.
 */
#include "Base.h"
#include "PhysicsCharacter.h"
#include "Scene.h"
#include "Game.h"
#include "PhysicsController.h"

namespace gameplay
{

/**
 * @script{ignore}
 */
class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:

    /**
     * @see btCollisionWorld::ClosestConvexResultCallback::ClosestConvexResultCallback
     */
    ClosestNotMeConvexResultCallback(PhysicsCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
        : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), _me(me), _up(up), _minSlopeDot(minSlopeDot)
    {
    }

    /**
     * @see btCollisionWorld::ClosestConvexResultCallback::addSingleResult
     */
    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
    {
        PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(convexResult.m_hitCollisionObject->getUserPointer());
        
        GP_ASSERT(object);
        if (object == _me || object->getType() == PhysicsCollisionObject::GHOST_OBJECT)
            return 1.0f;

        return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
    }

protected:

    PhysicsCollisionObject* _me;
    const btVector3 _up;
    btScalar _minSlopeDot;
};

PhysicsCharacter::PhysicsCharacter(Node* node, const PhysicsCollisionShape::Definition& shape, float mass, int group, int mask)
    : PhysicsGhostObject(node, shape, group, mask), _moveVelocity(0,0,0), _forwardVelocity(0.0f), _rightVelocity(0.0f),
    _verticalVelocity(0, 0, 0), _currentVelocity(0,0,0), _normalizedVelocity(0,0,0),
    _colliding(false), _collisionNormal(0,0,0), _currentPosition(0,0,0), _stepHeight(0.1f),
    _slopeAngle(0.0f), _cosSlopeAngle(1.0f), _physicsEnabled(true), _mass(mass), _actionInterface(NULL)
{
    setMaxSlopeAngle(45.0f);

    // Set the collision flags on the ghost object to indicate it's a character.
    GP_ASSERT(_ghostObject);
    _ghostObject->setCollisionFlags(_ghostObject->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Register ourselves as an action on the physics world so we are called back during physics ticks.
    GP_ASSERT(Game::getInstance()->getPhysicsController() && Game::getInstance()->getPhysicsController()->_world);
    _actionInterface = new ActionInterface(this);
    Game::getInstance()->getPhysicsController()->_world->addAction(_actionInterface);
}

PhysicsCharacter::~PhysicsCharacter()
{
    // Unregister ourselves as action from world.
    GP_ASSERT(Game::getInstance()->getPhysicsController() && Game::getInstance()->getPhysicsController()->_world);
    Game::getInstance()->getPhysicsController()->_world->removeAction(_actionInterface);
    SAFE_DELETE(_actionInterface);

}

PhysicsCharacter* PhysicsCharacter::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    if (!properties || !(strcmp(properties->getNamespace(), "collisionObject") == 0))
    {
        GP_ERROR("Failed to load physics character from properties object: must be non-null object and have namespace equal to 'collisionObject'.");
        return NULL;
    }

    // Check that the type is specified and correct.
    const char* type = properties->getString("type");
    if (!type)
    {
        GP_ERROR("Failed to load physics character from properties object; required attribute 'type' is missing.");
        return NULL;
    }
    if (strcmp(type, "CHARACTER") != 0)
    {
        GP_ERROR("Failed to load physics character from properties object; attribute 'type' must be equal to 'CHARACTER'.");
        return NULL;
    }

    // Load the physics collision shape definition.
    PhysicsCollisionShape::Definition shape = PhysicsCollisionShape::Definition::create(node, properties);
    if (shape.isEmpty())
    {
        GP_ERROR("Failed to create collision shape during physics character creation.");
        return NULL;
    }

    // Load the character's parameters.
    properties->rewind();
    float mass = 1.0f;
    float maxStepHeight = 0.1f;
    float maxSlopeAngle = 0.0f;
    const char* name = NULL;
    while ((name = properties->getNextProperty()) != NULL)
    {
        if (strcmp(name, "mass") == 0)
        {
            mass = properties->getFloat();
        }
        else if (strcmp(name, "maxStepHeight") == 0)
        {
            maxStepHeight = properties->getFloat();
        }
        else if (strcmp(name, "maxSlopeAngle") == 0)
        {
            maxSlopeAngle = properties->getFloat();
        }
        else
        {
            // Ignore this case (the attributes for the character's collision shape would end up here).
        }
    }

    // Create the physics character.
    PhysicsCharacter* character = new PhysicsCharacter(node, shape, mass);
    character->setMaxStepHeight(maxStepHeight);
    character->setMaxSlopeAngle(maxSlopeAngle);

    return character;
}

PhysicsCollisionObject::Type PhysicsCharacter::getType() const
{
    return PhysicsCollisionObject::CHARACTER;
}

btCollisionObject* PhysicsCharacter::getCollisionObject() const
{
    return _ghostObject;
}

bool PhysicsCharacter::isPhysicsEnabled() const
{
    return _physicsEnabled;
}

void PhysicsCharacter::setPhysicsEnabled(bool enabled)
{
    _physicsEnabled = enabled;
}

float PhysicsCharacter::getMaxStepHeight() const
{
    return _stepHeight;
}

void PhysicsCharacter::setMaxStepHeight(float height)
{
    _stepHeight = height;
}

float PhysicsCharacter::getMaxSlopeAngle() const
{
    return _slopeAngle;
}

void PhysicsCharacter::setMaxSlopeAngle(float angle)
{
    _slopeAngle = angle;
    _cosSlopeAngle = std::cos(MATH_DEG_TO_RAD(angle));
}

void PhysicsCharacter::setVelocity(const Vector3& velocity)
{
    _moveVelocity.setValue(velocity.x, velocity.y, velocity.z);
}

void PhysicsCharacter::setVelocity(float x, float y, float z)
{
    _moveVelocity.setValue(x, y, z);
}

void PhysicsCharacter::rotate(const Vector3& axis, float angle)
{
    GP_ASSERT(_node);
    _node->rotate(axis, angle);
}

void PhysicsCharacter::rotate(const Quaternion& rotation)
{
    GP_ASSERT(_node);
    _node->rotate(rotation);
}

void PhysicsCharacter::setRotation(const Vector3& axis, float angle)
{
    GP_ASSERT(_node);
    _node->setRotation(axis, angle);
}

void PhysicsCharacter::setRotation(const Quaternion& rotation)
{
    GP_ASSERT(_node);
    _node->setRotation(rotation);
}

void PhysicsCharacter::setForwardVelocity(float velocity)
{
    _forwardVelocity = velocity;
}

void PhysicsCharacter::setRightVelocity(float velocity)
{
    _rightVelocity = velocity;
}

Vector3 PhysicsCharacter::getCurrentVelocity() const
{
    Vector3 v(_currentVelocity.x(), _currentVelocity.y(), _currentVelocity.z());
    v.x += _verticalVelocity.x();
    v.y += _verticalVelocity.y();
    v.z += _verticalVelocity.z();
    return v;
}

void PhysicsCharacter::jump(float height)
{
    // TODO: Add support for different jump modes (i.e. double jump, changing direction in air, holding down jump button for extra height, etc)
    if (!_verticalVelocity.isZero())
        return;

    // v = sqrt(v0^2 + 2 a s)
    //  v0 == initial velocity (zero for jumping)
    //  a == acceleration (inverse gravity)
    //  s == linear displacement (height)
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Vector3 jumpVelocity = -Game::getInstance()->getPhysicsController()->getGravity() * height * 2.0f;
    jumpVelocity.set(
        jumpVelocity.x == 0 ? 0 : std::sqrt(jumpVelocity.x),
        jumpVelocity.y == 0 ? 0 : std::sqrt(jumpVelocity.y),
        jumpVelocity.z == 0 ? 0 : std::sqrt(jumpVelocity.z));
    _verticalVelocity += BV(jumpVelocity);
}

void PhysicsCharacter::updateCurrentVelocity()
{
    GP_ASSERT(_node);
    
    Vector3 temp;
    btScalar velocity2 = 0;

    // Reset velocity vector.
    _normalizedVelocity.setValue(0, 0, 0);

    // Add movement velocity contribution.
    if (!_moveVelocity.isZero())
    {
        _normalizedVelocity = _moveVelocity;
        velocity2 = _moveVelocity.length2();
    }

    // Add forward velocity contribution.
    if (_forwardVelocity != 0)
    {
        _node->getWorldMatrix().getForwardVector(&temp);
        temp.normalize();
        temp *= -_forwardVelocity;
        _normalizedVelocity += btVector3(temp.x, temp.y, temp.z);
        velocity2 = std::max(std::fabs(velocity2), std::fabs(_forwardVelocity*_forwardVelocity));
    }

    // Add right velocity contribution.
    if (_rightVelocity != 0)
    {
        _node->getWorldMatrix().getRightVector(&temp);
        temp.normalize();
        temp *= _rightVelocity;
        _normalizedVelocity += btVector3(temp.x, temp.y, temp.z);
        velocity2 = std::max(std::fabs(velocity2), std::fabs(_rightVelocity*_rightVelocity));
    }

    // Compute final combined movement vectors
    if (_normalizedVelocity.isZero())
    {
        _currentVelocity.setZero();
    }
    else
    {
        _normalizedVelocity.normalize();
        _currentVelocity = _normalizedVelocity * std::sqrt(velocity2);
    }
}

void PhysicsCharacter::stepUp(btCollisionWorld* collisionWorld, btScalar time)
{
    btVector3 targetPosition(_currentPosition);

    if (_verticalVelocity.isZero())
    {
        // Simply increase our position by step height to enable us
        // to smoothly move over steps.
        targetPosition += btVector3(0, _stepHeight, 0);
    }

    // TODO: Convex sweep test to ensure we didn't hit anything during the step up.

    _currentPosition = targetPosition;
}

void PhysicsCharacter::stepForwardAndStrafe(btCollisionWorld* collisionWorld, float time)
{
    updateCurrentVelocity();

    // Calculate final velocity
    btVector3 velocity(_currentVelocity);
    velocity *= time; // since velocity is in meters per second

    if (velocity.isZero())
    {
        // No velocity, so we aren't moving
        return;
    }

    // Translate the target position by the velocity vector (already scaled by t)
    btVector3 targetPosition = _currentPosition + velocity;

    // If physics is disabled, simply update current position without checking collisions
    if (!_physicsEnabled)
    {
        _currentPosition = targetPosition;
        return;
    }

    // Check for collisions by performing a bullet convex sweep test
    btTransform start;
    btTransform end;
    start.setIdentity();
    end.setIdentity();

    btScalar fraction = 1.0;
    btScalar distance2;

    if (_colliding && (_normalizedVelocity.dot(_collisionNormal) > btScalar(0.0)))
    {
        updateTargetPositionFromCollision(targetPosition, _collisionNormal);
    }

    int maxIter = 10;

    GP_ASSERT(_ghostObject && _ghostObject->getBroadphaseHandle());
    GP_ASSERT(_collisionShape);
    GP_ASSERT(collisionWorld);
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    while (fraction > btScalar(0.01) && maxIter-- > 0)
    {
        start.setOrigin(_currentPosition);
        end.setOrigin(targetPosition);

        btVector3 sweepDirNegative(_currentPosition - targetPosition);

        ClosestNotMeConvexResultCallback callback(this, sweepDirNegative, btScalar(0.0));
        callback.m_collisionFilterGroup = _ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = _ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

        _ghostObject->convexSweepTest(static_cast<btConvexShape*>(_collisionShape->getShape()), start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

        fraction -= callback.m_closestHitFraction;

        if (callback.hasHit())
        {
            Vector3 normal(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
            PhysicsCollisionObject* o = Game::getInstance()->getPhysicsController()->getCollisionObject(callback.m_hitCollisionObject);
            GP_ASSERT(o);
            if (o->getType() == PhysicsCollisionObject::RIGID_BODY && o->isDynamic())
            {
                PhysicsRigidBody* rb = static_cast<PhysicsRigidBody*>(o);
                GP_ASSERT(rb);
                normal.normalize();
                rb->applyImpulse(_mass * -normal * velocity.length());
            }

            updateTargetPositionFromCollision(targetPosition, callback.m_hitNormalWorld);

            btVector3 currentDir = targetPosition - _currentPosition;
            distance2 = currentDir.length2();
            if (distance2 > FLT_EPSILON)
            {
                currentDir.normalize();

                // If velocity is against original velocity, stop to avoid tiny oscilations in sloping corners.
                if (currentDir.dot(_normalizedVelocity) <= btScalar(0.0))
                {
                    break;
                }
            }
        }
        else
        {
            // Nothing in our way
            break;
        }
    }

    _currentPosition = targetPosition;
}

void PhysicsCharacter::stepDown(btCollisionWorld* collisionWorld, btScalar time)
{
    GP_ASSERT(Game::getInstance()->getPhysicsController() && Game::getInstance()->getPhysicsController()->_world);
    GP_ASSERT(_ghostObject && _ghostObject->getBroadphaseHandle());
    GP_ASSERT(_collisionShape);
    GP_ASSERT(collisionWorld);

    // Contribute gravity to vertical velocity.
    btVector3 gravity = Game::getInstance()->getPhysicsController()->_world->getGravity();
    _verticalVelocity += (gravity * time);

    // Compute new position from vertical velocity.
    btVector3 targetPosition = _currentPosition + (_verticalVelocity * time);
    targetPosition -= btVector3(0, _stepHeight, 0);

    // Perform a convex sweep test between current and target position.
    btTransform start;
    btTransform end;
    start.setIdentity();
    end.setIdentity();

    btScalar fraction = 1.0;
    int maxIter = 10;
    while (fraction > btScalar(0.01) && maxIter-- > 0)
    {
        start.setOrigin(_currentPosition);
        end.setOrigin(targetPosition);

        btVector3 sweepDirNegative(_currentPosition - targetPosition);

        ClosestNotMeConvexResultCallback callback(this, sweepDirNegative, 0.0);
        callback.m_collisionFilterGroup = _ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = _ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

        _ghostObject->convexSweepTest(static_cast<btConvexShape*>(_collisionShape->getShape()), start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

        fraction -= callback.m_closestHitFraction;

        if (callback.hasHit())
        {
            // Collision detected, fix it.
            Vector3 normal(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
            normal.normalize();

            float dot = normal.dot(Vector3::unitY());
            if (dot > _cosSlopeAngle - MATH_EPSILON)
            {
                targetPosition.setInterpolate3(_currentPosition, targetPosition, callback.m_closestHitFraction);

                // Zero out fall velocity when we hit an object going straight down.
                _verticalVelocity.setZero();
                break;
            }
            else
            {
                PhysicsCollisionObject* o = Game::getInstance()->getPhysicsController()->getCollisionObject(callback.m_hitCollisionObject);
                GP_ASSERT(o);
                if (o->getType() == PhysicsCollisionObject::RIGID_BODY && o->isDynamic())
                {
                    PhysicsRigidBody* rb = static_cast<PhysicsRigidBody*>(o);
                    GP_ASSERT(rb);
                    normal.normalize();
                    rb->applyImpulse(_mass * -normal * sqrt(BV(normal).dot(_verticalVelocity)));
                }

                updateTargetPositionFromCollision(targetPosition, BV(normal));
            }
        }
        else
        {
            // Nothing is in the way.
            break;
        }
    }

    // Calculate what the vertical velocity actually is.
    // In cases where the character might not actually be able to move down,
    // but isn't intersecting with an object straight down either, we don't
    // want to keep increasing the vertical velocity until the character 
    // randomly drops through the floor when it can finally move due to its
    // vertical velocity having such a great magnitude.
    if (!_verticalVelocity.isZero() && time > 0.0f)
        _verticalVelocity = ((targetPosition + btVector3(0.0, _stepHeight, 0.0)) - _currentPosition) / time;

    _currentPosition = targetPosition;
}

/*
 * Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'.
 */
static btVector3 computeReflectionDirection(const btVector3& direction, const btVector3& normal)
{
    return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

/*
 * Returns the portion of 'direction' that is parallel to 'normal'.
 */
static btVector3 parallelComponent(const btVector3& direction, const btVector3& normal)
{
    btScalar magnitude = direction.dot(normal);
    return normal * magnitude;
}

/*
 * Returns the portion of 'direction' that is perpendicular to 'normal'.
 */
static btVector3 perpindicularComponent(const btVector3& direction, const btVector3& normal)
{
    return direction - parallelComponent(direction, normal);
}

void PhysicsCharacter::updateTargetPositionFromCollision(btVector3& targetPosition, const btVector3& collisionNormal)
{
    btVector3 movementDirection = targetPosition - _currentPosition;
    btScalar movementLength = movementDirection.length();

    if (movementLength > FLT_EPSILON)
    {
        movementDirection.normalize();

        btVector3 reflectDir = computeReflectionDirection(movementDirection, collisionNormal);
        reflectDir.normalize();

        btVector3 perpindicularDir = perpindicularComponent(reflectDir, collisionNormal);
        targetPosition = _currentPosition;

        // Disallow the character from moving up during collision recovery (using an arbitrary reasonable epsilon).
        // Note that this will need to be generalized to allow for an arbitrary up axis.
        //
        // TO DO: FIX THIS! Fine tuning of this condition is needed. For now we force it true.
        bool forceTrue = true;
        if (forceTrue || perpindicularDir.y() < _stepHeight + 0.001 || collisionNormal.y() > _cosSlopeAngle - MATH_EPSILON)
        {
            btVector3 perpComponent = perpindicularDir * movementLength;
            targetPosition += perpComponent;
        }
    }
}

bool PhysicsCharacter::fixCollision(btCollisionWorld* world)
{
    GP_ASSERT(_node);
    GP_ASSERT(_ghostObject);
    GP_ASSERT(world && world->getDispatcher());
    GP_ASSERT(Game::getInstance()->getPhysicsController());

    bool collision = false;
    btOverlappingPairCache* pairCache = _ghostObject->getOverlappingPairCache();
    GP_ASSERT(pairCache);

    // Tell the world to dispatch collision events for our ghost object.
    world->getDispatcher()->dispatchAllCollisionPairs(pairCache, world->getDispatchInfo(), world->getDispatcher());

    // Store our current world position.
    Vector3 startPosition;
    _node->getWorldMatrix().getTranslation(&startPosition);
    btVector3 currentPosition = BV(startPosition);

    // Handle all collisions/overlapping pairs.
    btScalar maxPenetration = btScalar(0.0);
    for (int i = 0, count = pairCache->getNumOverlappingPairs(); i < count; ++i)
    {
        _manifoldArray.resize(0);

        // Query contacts between this overlapping pair (store in _manifoldArray).
        btBroadphasePair* collisionPair = &pairCache->getOverlappingPairArray()[i];
        if (collisionPair->m_algorithm)
        {
            collisionPair->m_algorithm->getAllContactManifolds(_manifoldArray);
        }

        for (int j = 0, manifoldCount = _manifoldArray.size(); j < manifoldCount; ++j)
        {
            btPersistentManifold* manifold = _manifoldArray[j];
            GP_ASSERT(manifold);

            // Get the direction of the contact points (used to scale normal vector in the correct direction).
            btScalar directionSign = manifold->getBody0() == _ghostObject ? -1.0f : 1.0f;

            // Skip ghost objects.
            PhysicsCollisionObject* object = Game::getInstance()->getPhysicsController()->getCollisionObject(
                (btCollisionObject*)(manifold->getBody0() == _ghostObject ? manifold->getBody1() : manifold->getBody0()));
            if (!object || object->getType() == PhysicsCollisionObject::GHOST_OBJECT)
                continue;

            for (int p = 0, contactCount = manifold->getNumContacts(); p < contactCount; ++p)
            {
                const btManifoldPoint& pt = manifold->getContactPoint(p);

                // Get penetration distance for this contact point.
                btScalar dist = pt.getDistance();

                if (dist < 0.0)
                {
                    // A negative distance means the objects are overlapping.
                    if (dist < maxPenetration)
                    {
                        // Store collision normal for this point.
                        maxPenetration = dist;
                        _collisionNormal = pt.m_normalWorldOnB * directionSign;
                    }

                    // Calculate new position for object, which is translated back along the collision normal.
                    currentPosition += pt.m_normalWorldOnB * directionSign * dist * 0.2f;
                    collision = true;
                }
            }
        }
    }

    // Set the new world transformation to apply to fix the collision.
    Vector3 newPosition = Vector3(currentPosition.x(), currentPosition.y(), currentPosition.z()) - startPosition;
    if (newPosition != Vector3::zero())
        _node->translate(newPosition);

    return collision;
}

PhysicsCharacter::ActionInterface::ActionInterface(PhysicsCharacter* character) : character(character)
{
}

void PhysicsCharacter::ActionInterface::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
    character->updateAction(collisionWorld, deltaTimeStep);
}

void PhysicsCharacter::ActionInterface::debugDraw(btIDebugDraw* debugDrawer)
{
    // Not used yet.
}

void PhysicsCharacter::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
    GP_ASSERT(_ghostObject);
    GP_ASSERT(_node);

    // First check for existing collisions and attempt to respond/fix them.
    // Basically we are trying to move the character so that it does not penetrate
    // any other collision objects in the scene. We need to do this to ensure that
    // the following steps (movement) start from a clean slate, where the character
    // is not colliding with anything. Also, this step handles collision between
    // dynamic objects (i.e. objects that moved and now intersect the character).
    if (_physicsEnabled)
    {
        _colliding = false;
        int stepCount = 0;
        while (fixCollision(collisionWorld))
        {
            _colliding = true;

            if (++stepCount > 4)
            {
                // Most likely we are wedged between a number of different collision objects.
                break;
            }
        }
    }

    // Update current and target world positions.
    btVector3 startPosition = _ghostObject->getWorldTransform().getOrigin();
    _currentPosition = startPosition;

    // Process movement in the up direction.
    if (_physicsEnabled)
        stepUp(collisionWorld, deltaTimeStep);
    
    // Process horizontal movement.
    stepForwardAndStrafe(collisionWorld, deltaTimeStep);

    // Process movement in the down direction.
    if (_physicsEnabled)
        stepDown(collisionWorld, deltaTimeStep);

    // Set new position.
    btVector3 newPosition = _currentPosition - startPosition;
    Vector3 translation = Vector3(newPosition.x(), newPosition.y(), newPosition.z());
    if (translation !=  Vector3::zero())
        _node->translate(translation);
}


}
