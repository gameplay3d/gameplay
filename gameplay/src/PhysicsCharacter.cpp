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

	ClosestNotMeConvexResultCallback(btCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
        : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), _me(me), _up(up), _minSlopeDot(minSlopeDot)
	{
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == _me)
			return btScalar(1.0);

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

		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}

protected:

	btCollisionObject* _me;
	const btVector3 _up;
	btScalar _minSlopeDot;
};

PhysicsCharacter::PhysicsCharacter(Node* node, float radius, float height, const Vector3 center)
    : _node(node), _motionState(NULL), _moveVelocity(0,0,0), _forwardVelocity(0.0f), _rightVelocity(0.0f),
    _fallVelocity(0, 0, 0), _currentVelocity(0,0,0), _normalizedVelocity(0,0,0),
    _colliding(false), _collisionNormal(0,0,0), _currentPosition(0,0,0),
    _ghostObject(NULL), _collisionShape(NULL), _ignoreTransformChanged(0),
    _stepHeight(0.2f), _slopeAngle(0.0f), _cosSlopeAngle(0.0f)
{
    setMaxSlopeAngle(45.0f);

    node->addRef();
    node->addListener(this);

    // Create physics motion state for syncing transform between gameplay and bullet
    Vector3 centerOfMassOffset(-center);
    _motionState = new PhysicsMotionState(node, &centerOfMassOffset);

    // Create ghost object, which is used as an efficient way to detect
    // collisions between pairs of objects.
    _ghostObject = bullet_new<btPairCachingGhostObject>();

    // Set initial transform
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());

    // Create a capsule collision shape
    _collisionShape = bullet_new<btCapsuleShape>((btScalar)radius, (btScalar)(height - radius*2));

    // Set the collision shape on the ghost object (get it from the node's rigid body)
    _ghostObject->setCollisionShape(_collisionShape);
    _ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    btDynamicsWorld* world = Game::getInstance()->getPhysicsController()->_world;
    
    // Register the ghost object for collisions with the world.
    // For now specify static flag only, so character does not interact with dynamic objects
    world->addCollisionObject(_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter);

    // Register ourselves as an action on the physics world so we are called back during physics ticks
    world->addAction(this);
}

PhysicsCharacter::~PhysicsCharacter()
{
    // Unregister ourself with world
    btDynamicsWorld* world = Game::getInstance()->getPhysicsController()->_world;
    world->removeCollisionObject(_ghostObject);
    world->removeAction(this);

    SAFE_DELETE(_ghostObject);
    SAFE_DELETE(_collisionShape);

    _node->removeListener(this);
    SAFE_RELEASE(_node);
    SAFE_DELETE(_motionState);
}

Node* PhysicsCharacter::getNode() const
{
    return _node;
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

void PhysicsCharacter::transformChanged(Transform* transform, long cookie)
{
    if (!_ignoreTransformChanged)
    {
        // Update motion state with transform from node
        _motionState->updateTransformFromNode();

        // Update transform on ghost object
        _motionState->getWorldTransform(_ghostObject->getWorldTransform());
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
    _colliding = false;
    int stepCount = 0;
	while (fixCollision(collisionWorld))
	{
        _colliding = true;

        // After a small number of attempts to fix a collision/penetration, give up...
        if (++stepCount > 4)
		{
            WARN_VARG("Character '%s' could not recover from collision.", _node->getId());
			break;
		}
	}

    // Update current and target world positions
    btTransform transform = _ghostObject->getWorldTransform();
    _currentPosition = transform.getOrigin();

    // Process movement in the up direction
    stepUp(collisionWorld, deltaTimeStep);

    // Process horizontal movement
    stepForwardAndStrafe(collisionWorld, deltaTimeStep);

    // Process movement in the down direction
    stepDown(collisionWorld, deltaTimeStep);

    // Set new position
    transform.setOrigin(_currentPosition);

    // Update world transform
    ++_ignoreTransformChanged;
    _motionState->setWorldTransform(transform);
    --_ignoreTransformChanged;

    // Update ghost object transform
    _motionState->getWorldTransform(_ghostObject->getWorldTransform());
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

    if (_currentVelocity.isZero())
    {
        // No velocity, so we aren't moving
        return;
    }

    // Calculate final velocity
    btVector3 velocity(_currentVelocity);
    if (animationCount > 0)
    {
        velocity *= animationMoveSpeed;
    }
    velocity *= time; // since velocity is in meters per second

    // Translate the target position by the velocity vector (already scaled by t)
    btVector3 targetPosition = _currentPosition + velocity;

    // Check for collisions by performing a bullet convex sweep test
    btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	btScalar fraction = 1.0;
	btScalar distance2 = (_currentPosition-targetPosition).length2();

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

		ClosestNotMeConvexResultCallback callback(_ghostObject, sweepDirNegative, btScalar(0.0));
		callback.m_collisionFilterGroup = _ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = _ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

        // Temporarily increase collision margin by a bit
        //btScalar margin = _collisionShape->getMargin();
        //_collisionShape->setMargin(margin + m_addedMargin);

        _ghostObject->convexSweepTest(_collisionShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

		//m_convexShape->setMargin(margin);

		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
        {
			// We hit something so can move only a fraction
			//btScalar hitDistance = (callback.m_hitPointWorld - _currentPosition).length();

            //_currentPosition.setInterpolate3(_currentPosition, targetPosition, callback.m_closestHitFraction);

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
            //_currentPosition = targetPosition;
            break;
        }
    }

    _currentPosition = targetPosition;
}

void PhysicsCharacter::stepDown(btCollisionWorld* collisionWorld, btScalar time)
{
    // Contribute gravity to fall velocity.
    // TODO: This simple formula assumes no air friction, which is completely unrealistic
    // (characters fall way too fast). We should consider how to support this without much
    // added complexity.
    btVector3 gravity = Game::getInstance()->getPhysicsController()->_world->getGravity();
    _fallVelocity += (gravity * time);

    btVector3 targetPosition = _currentPosition + _fallVelocity;

    // Perform a convex sweep test between current and target position
    btTransform start, end;
    start.setIdentity();
    end.setIdentity();
    start.setOrigin(_currentPosition);
    end.setOrigin(targetPosition);

    // TODO: We probably have to perform sweep tests separately in stepForward and stepDown (and stepUp) since
    // combining the full move into a single targetPosition and computing sweep test between currentPosition and targetPosition
    // is ALYWAYS going to result in a collision at almost exactly currentPosition... this is because, when you are already
    // on the floor and applying gravity, 
    ClosestNotMeConvexResultCallback callback(_ghostObject, btVector3(0, 1, 0), _cosSlopeAngle);
	callback.m_collisionFilterGroup = _ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = _ghostObject->getBroadphaseHandle()->m_collisionFilterMask;
    _ghostObject->convexSweepTest(_collisionShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
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
    btScalar normalMag = 1.0;

	btVector3 movementDirection = targetPosition - _currentPosition;
	btScalar movementLength = movementDirection.length();

	if (movementLength > FLT_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection(movementDirection, collisionNormal);
		reflectDir.normalize();

		btVector3 parallelDir, perpindicularDir;

		parallelDir = parallelComponent(reflectDir, collisionNormal);
		perpindicularDir = perpindicularComponent(reflectDir, collisionNormal);

		targetPosition = _currentPosition;
		/*if (tangentMag != 0.0)
		{
			btVector3 parComponent = parallelDir * btScalar (tangentMag*movementLength);
			targetPosition +=  parComponent;
		}*/

		if (normalMag != 0.0)
		{
			btVector3 perpComponent = perpindicularDir * btScalar (normalMag * movementLength);
			targetPosition += perpComponent;
		}
	}
}

bool PhysicsCharacter::fixCollision(btCollisionWorld* world)
{
	bool collision = false;

    btOverlappingPairCache* pairCache = _ghostObject->getOverlappingPairCache();

    // Tell the world to dispatch collision events for our ghost object
	world->getDispatcher()->dispatchAllCollisionPairs(pairCache, world->getDispatchInfo(), world->getDispatcher());

    // Store our current world position
    btVector3 currentPosition = _ghostObject->getWorldTransform().getOrigin();

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

            // Get the direction of the contact points (used to scale normal vector in the correct direction)
			btScalar directionSign = manifold->getBody0() == _ghostObject ? btScalar(-1.0) : btScalar(1.0);

			for (int p = 0, contactCount = manifold->getNumContacts(); p < contactCount; ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);

                // Get penetration distance for this contact point
				btScalar dist = pt.getDistance();

				if (dist < 0.0)
				{
					if (dist < maxPenetration)
					{
                        // Store collision normal for this point
						maxPenetration = dist;
                        _collisionNormal = pt.m_normalWorldOnB * directionSign;
					}

                    // Calculate new position for object, which is translated back along the collision normal
					currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
					collision = true;
				}
			}
			//manifold->clearManifold();
		}
	}

    // Set the new world transformation to apply to fix the collision
	btTransform newTransform = _ghostObject->getWorldTransform();
	newTransform.setOrigin(currentPosition);
	_ghostObject->setWorldTransform(newTransform);

	return collision;
}

void PhysicsCharacter::debugDraw(btIDebugDraw* debugDrawer)
{
    // debug drawing handled by PhysicsController
}

}
