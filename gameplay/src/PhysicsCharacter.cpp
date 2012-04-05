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

class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:

	ClosestNotMeConvexResultCallback(PhysicsCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
        : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), _me(me), _up(up), _minSlopeDot(minSlopeDot)
	{
	}

	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
        PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(convexResult.m_hitCollisionObject->getUserPointer());

		if (object == _me || object->getType() == PhysicsCollisionObject::GHOST_OBJECT)
            return 1.0f;

        /*
		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{
			// transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = _up.dot(hitNormalWorld);
		if (dotUp < _minSlopeDot)
        {
			return btScalar(1.0);
		}
        */

		return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}

protected:

	PhysicsCollisionObject* _me;
	const btVector3 _up;
	btScalar _minSlopeDot;
};

PhysicsCharacter::PhysicsCharacter(Node* node, const PhysicsCollisionShape::Definition& shape)
    : PhysicsGhostObject(node, shape), _moveVelocity(0,0,0), _forwardVelocity(0.0f), _rightVelocity(0.0f),
    _fallVelocity(0, 0, 0), _currentVelocity(0,0,0), _normalizedVelocity(0,0,0),
    _colliding(false), _collisionNormal(0,0,0), _currentPosition(0,0,0),
    _stepHeight(0.1f), _slopeAngle(0.0f), _cosSlopeAngle(0.0f), _physicsEnabled(true)
{
	setMaxSlopeAngle(45.0f);

    // Set the collision flags on the ghost object to indicate it's a character
    _ghostObject->setCollisionFlags(_ghostObject->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Register ourselves as an action on the physics world so we are called back during physics ticks
    Game::getInstance()->getPhysicsController()->_world->addAction(this);
}

PhysicsCharacter::~PhysicsCharacter()
{
    // Unregister ourselves as action from world
    Game::getInstance()->getPhysicsController()->_world->removeAction(this);
}

PhysicsCharacter* PhysicsCharacter::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    assert(properties);
    if (!properties || !(strcmp(properties->getNamespace(), "character") == 0))
    {
        WARN("Failed to load physics character from properties object: must be non-null object and have namespace equal to \'character\'.");
        return NULL;
    }

    // Load the physics collision shape definition.
    PhysicsCollisionShape::Definition* shape = PhysicsCollisionShape::Definition::create(node, properties);
    if (shape == NULL)
    {
        WARN("Failed to create collision shape during physics character creation.");
        return NULL;
    }

    // Create the physics character.
    PhysicsCharacter* character = new PhysicsCharacter(node, *shape);
    SAFE_DELETE(shape);

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

void PhysicsCharacter::addAnimation(const char* name, AnimationClip* clip, float moveSpeed)
{
    CharacterAnimation a;
    a.name = name;
    a.clip = clip;
    a.moveSpeed = moveSpeed;
    a.layer = 0;
    a.playing = false;
    a.animationFlags = ANIMATION_STOP;
    a.prev = NULL;
    _animations[name] = a;
}

AnimationClip* PhysicsCharacter::getAnimation(const char* name)
{
    if (name)
    {
        // Lookup the specified animation
        std::map<const char*, CharacterAnimation>::iterator aitr = _animations.find(name);
        if (aitr != _animations.end())
        {
            return aitr->second.clip;
        }
    }
    return NULL;
}

void PhysicsCharacter::play(const char* name, AnimationFlags flags, float speed, unsigned int blendDuration, unsigned int layer)
{
    CharacterAnimation* animation = NULL;
    if (name)
    {
        // Lookup the specified animation
        std::map<const char*, CharacterAnimation>::iterator aitr = _animations.find(name);
        if (aitr == _animations.end())
            return; // invalid animation name

        animation = &(aitr->second);

        // Set animation flags
        animation->clip->setRepeatCount(flags & ANIMATION_REPEAT ? AnimationClip::REPEAT_INDEFINITE : 1);
        animation->clip->setSpeed(speed);
        animation->animationFlags = flags;
        animation->layer = layer;
        animation->blendDuration = blendDuration;
        animation->prev = NULL;

        // If the animation is already marked playing, do nothing more
        if (animation->playing)
            return;
    }

    play(animation, layer);
}

void PhysicsCharacter::play(CharacterAnimation* animation, unsigned int layer)
{
    // Is there already an animation playing on this layer?
    std::map<unsigned int, CharacterAnimation*>::iterator litr = _layers.find(layer);
    CharacterAnimation* prevAnimation = (litr == _layers.end() ? NULL : litr->second);
    if (prevAnimation && prevAnimation->playing)
    {
        // An animation is already playing on this layer
        if (animation)
        {
            if (animation->animationFlags == ANIMATION_RESUME)
                animation->prev = prevAnimation;

            if (animation->blendDuration > 0L)
            {
                // Crossfade from current animation into the new one
                prevAnimation->clip->crossFade(animation->clip, animation->blendDuration);
            }
            else
            {
                // Stop the previous animation (no blending)
                prevAnimation->clip->stop();

                // Play the new animation
                animation->clip->play();
            }
        }
        else
        {
            // No new animaton specified - stop current animation on this layer
            prevAnimation->clip->stop();
        }

        prevAnimation->playing = false;
    }
    else if (animation)
    {
        // No animations currently playing - just play the new one
        animation->clip->play();
    }

    // Update animaton and layers
    if (animation)
    {
        animation->playing = true;

        // Update layer to point to the new animation
        if (litr != _layers.end())
            litr->second = animation;
        else
            _layers[layer] = animation;
    }
    else if (litr != _layers.end())
    {
        // Remove layer sine we stopped the animation previously on it
        _layers.erase(litr);
    }
}

void PhysicsCharacter::setVelocity(const Vector3& velocity)
{
    _moveVelocity.setValue(velocity.x, velocity.y, velocity.z);
}

void PhysicsCharacter::rotate(const Vector3& axis, float angle)
{
    _node->rotate(axis, angle);
}

void PhysicsCharacter::rotate(const Quaternion& rotation)
{
    _node->rotate(rotation);
}

void PhysicsCharacter::setRotation(const Vector3& axis, float angle)
{
    _node->setRotation(axis, angle);
}

void PhysicsCharacter::setRotation(const Quaternion& rotation)
{
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

void PhysicsCharacter::jump(float height)
{
    // TODO
}

void PhysicsCharacter::updateCurrentVelocity()
{
    Vector3 temp;
    btScalar velocity2 = 0;

    // Reset velocity vector
    _normalizedVelocity.setValue(0, 0, 0);

    // Add movement velocity contribution
    if (!_moveVelocity.isZero())
    {
        _normalizedVelocity = _moveVelocity;
        velocity2 = _moveVelocity.length2();
    }

    // Add forward velocity contribution
    if (_forwardVelocity != 0)
    {
        _node->getWorldMatrix().getForwardVector(&temp);
        temp.normalize();
        temp *= -_forwardVelocity;
        _normalizedVelocity += btVector3(temp.x, temp.y, temp.z);
        velocity2 = std::max(std::abs(velocity2), std::abs(_forwardVelocity*_forwardVelocity));
    }

    // Add right velocity contribution
    if (_rightVelocity != 0)
    {
        _node->getWorldMatrix().getRightVector(&temp);
        temp.normalize();
        temp *= _rightVelocity;
        _normalizedVelocity += btVector3(temp.x, temp.y, temp.z);
        velocity2 = std::max(std::abs(velocity2), std::abs(_rightVelocity*_rightVelocity));
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

void PhysicsCharacter::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
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
                // Most likely we are wedged between a number of different collision objects
			    break;
		    }
	    }
    }

    // Update current and target world positions
	btVector3 startPosition = _ghostObject->getWorldTransform().getOrigin();
	_currentPosition = startPosition;

    // Process movement in the up direction
    if (_physicsEnabled)
        stepUp(collisionWorld, deltaTimeStep);

    // Process horizontal movement
    stepForwardAndStrafe(collisionWorld, deltaTimeStep);

    // Process movement in the down direction
	if (_physicsEnabled)
		stepDown(collisionWorld, deltaTimeStep);

    // Set new position
	btVector3 translation = _currentPosition - startPosition;
	_node->translate(translation.x(), translation.y(), translation.z());
}

void PhysicsCharacter::stepUp(btCollisionWorld* collisionWorld, btScalar time)
{
    // Note: btKinematicCharacterController implements this by always just setting
    // target position to currentPosition.y + stepHeight, and then checking for collisions.
    // Don't let the character move up if it hits the ceiling (or something above).
    // Do this WITHOUT using time in the calculation - this way you are always guarnateed
    // to step over a step that is stepHeight high.
    // 
    // Note that stepDown() will be called right after this, so the character will move back
    // down to collide with the ground so that he smoothly steps up stairs.
	_currentPosition += btVector3(0, _stepHeight, 0);
}

void PhysicsCharacter::stepForwardAndStrafe(btCollisionWorld* collisionWorld, float time)
{
    // Process currently playing movements+animations and determine final move location
    float animationMoveSpeed = 0.0f;
    unsigned int animationCount = 0;
    for (std::map<unsigned int, CharacterAnimation*>::iterator itr = _layers.begin(); itr != _layers.end(); ++itr)
    {
        CharacterAnimation* animation = itr->second;

        // If the animation is not playing, ignore it
        if (!animation->playing)
            continue;

        AnimationClip* clip = animation->clip;

        // Did the clip finish playing (but we still have it marked playing)?
        if (!clip->isPlaying())
        {
            // If the animaton was flaged the ANIMATION_RESUME bit, start the previously playing animation
            if ((animation->animationFlags == ANIMATION_RESUME) && animation->prev)
            {
                play(animation->prev, animation->prev->layer);
            }

            animation->playing = false;

            continue;
        }

        animationMoveSpeed += animation->moveSpeed;
        ++animationCount;
    }

    updateCurrentVelocity();

    // Calculate final velocity
    btVector3 velocity(_currentVelocity);
    if (animationCount > 0)
    {
        velocity *= animationMoveSpeed;
    }
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
			/*Vector3 normal(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
			PhysicsCollisionObject* o = Game::getInstance()->getPhysicsController()->getCollisionObject(callback.m_hitCollisionObject);
			if (o->getType() == PhysicsCollisionObject::RIGID_BODY && o->isDynamic())
			{
				PhysicsRigidBody* rb = static_cast<PhysicsRigidBody*>(o);
				normal.normalize();
				rb->applyImpulse(-normal);
			}*/

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
    // Contribute basic gravity to fall velocity.
	btVector3 gravity = Game::getInstance()->getPhysicsController()->_world->getGravity();
    _fallVelocity += (gravity * time);

    btVector3 targetPosition = _currentPosition + (_fallVelocity * time);
	targetPosition -= btVector3(0, _stepHeight, 0);

    // Perform a convex sweep test between current and target position
	btTransform start;
	btTransform end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(_currentPosition);
	end.setOrigin(targetPosition);

    ClosestNotMeConvexResultCallback callback(this, btVector3(0, 1, 0), _cosSlopeAngle);
	callback.m_collisionFilterGroup = _ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = _ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

	_ghostObject->convexSweepTest(static_cast<btConvexShape*>(_collisionShape->getShape()), start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

	if (callback.hasHit())
	{
        // Collision detected, fix it
		_currentPosition.setInterpolate3(_currentPosition, targetPosition, callback.m_closestHitFraction);

        // Zero out fall velocity when we hit an object
        _fallVelocity.setZero();
	}
    else
    {
        // We can move here
        _currentPosition = targetPosition;
	}
}

/*
 * Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'
 */
btVector3 computeReflectionDirection(const btVector3& direction, const btVector3& normal)
{
	return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

/*
 * Returns the portion of 'direction' that is parallel to 'normal'
 */
btVector3 parallelComponent(const btVector3& direction, const btVector3& normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
}

/*
 * Returns the portion of 'direction' that is perpindicular to 'normal'
 */
btVector3 perpindicularComponent(const btVector3& direction, const btVector3& normal)
{
	return direction - parallelComponent(direction, normal);
}

void PhysicsCharacter::updateTargetPositionFromCollision(btVector3& targetPosition, const btVector3& collisionNormal)
{
    //btScalar tangentMag = 0.0;
    //btScalar normalMag = 1.0;

	btVector3 movementDirection = targetPosition - _currentPosition;
	btScalar movementLength = movementDirection.length();

	if (movementLength > FLT_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection(movementDirection, collisionNormal);
		reflectDir.normalize();

		//btVector3 parallelDir = parallelComponent(reflectDir, collisionNormal);
		btVector3 perpindicularDir = perpindicularComponent(reflectDir, collisionNormal);

		targetPosition = _currentPosition;
		/*if (tangentMag != 0.0)
		{
			btVector3 parComponent = parallelDir * btScalar (tangentMag*movementLength);
			targetPosition +=  parComponent;
		}*/

		//if (normalMag != 0.0)
		//{
			btVector3 perpComponent = perpindicularDir * btScalar (/*normalMag **/ movementLength);
			targetPosition += perpComponent;
		//}
	}
}

bool PhysicsCharacter::fixCollision(btCollisionWorld* world)
{
	bool collision = false;

    btOverlappingPairCache* pairCache = _ghostObject->getOverlappingPairCache();

    // Tell the world to dispatch collision events for our ghost object
	world->getDispatcher()->dispatchAllCollisionPairs(pairCache, world->getDispatchInfo(), world->getDispatcher());

    // Store our current world position
    Vector3 startPosition;
	_node->getWorldMatrix().getTranslation(&startPosition);
	btVector3 currentPosition = BV(startPosition);

    // Handle all collisions/overlappign pairs
	btScalar maxPenetration = btScalar(0.0);
	for (int i = 0, count = pairCache->getNumOverlappingPairs(); i < count; ++i)
	{
		_manifoldArray.resize(0);

        // Query contacts between this overlapping pair (store in _manifoldArray)
		btBroadphasePair* collisionPair = &pairCache->getOverlappingPairArray()[i];
		if (collisionPair->m_algorithm)
        {
			collisionPair->m_algorithm->getAllContactManifolds(_manifoldArray);
        }

		for (int j = 0, manifoldCount = _manifoldArray.size(); j < manifoldCount; ++j)
		{
			btPersistentManifold* manifold = _manifoldArray[j];

            // Get the direction of the contact points (used to scale normal vector in the correct direction).
            btScalar directionSign = manifold->getBody0() == _ghostObject ? -1.0f : 1.0f;

            // Skip ghost objects
            PhysicsCollisionObject* object = Game::getInstance()->getPhysicsController()->getCollisionObject(
                (btCollisionObject*)(manifold->getBody0() == _ghostObject ? manifold->getBody1() : manifold->getBody0()));
            if (!object || object->getType() == PhysicsCollisionObject::GHOST_OBJECT)
                continue;

			for (int p = 0, contactCount = manifold->getNumContacts(); p < contactCount; ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);

                // Get penetration distance for this contact point
				btScalar dist = pt.getDistance();

				if (dist < 0.0)
				{
                    // A negative distance means the objects are overlapping
					if (dist < maxPenetration)
					{
                        // Store collision normal for this point
						maxPenetration = dist;
                        _collisionNormal = pt.m_normalWorldOnB * directionSign;
					}

                    // Calculate new position for object, which is translated back along the collision normal
					currentPosition += pt.m_normalWorldOnB * directionSign * dist * 0.2f;
					collision = true;
				}
			}
		}
	}

	// Set the new world transformation to apply to fix the collision
	_node->translate(Vector3(currentPosition.x(), currentPosition.y(), currentPosition.z()) - startPosition);

	return collision;
}

void PhysicsCharacter::debugDraw(btIDebugDraw* debugDrawer)
{
    // debug drawing handled by PhysicsController
}

}
