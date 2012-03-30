#ifndef PHYSICSCHARACTER_H_
#define PHYSICSCHARACTER_H_

#include "PhysicsGhostObject.h"

namespace gameplay
{

/**
 * Physics controller class for a game character.
 *
 * This class can be used to control the movements and collisions of a character
 * in a game. It interacts with the Physics system to apply gravity and handle
 * collisions, however dynamics are not applied to the character directly by the
 * physics system. Instead, the character's movement is controlled directly by the
 * PhysicsCharacter class. This results in a more responsive and typical game
 * character than would be possible if trying to move a character by applying
 * physical simulation with forces.
 *
 * This class can also be used to control animations for a character. Animation
 * clips can be setup for typical character animations, such as walk, run, jump,
 * etc; and the controller will handle blending between these animations as needed.
 */
class PhysicsCharacter : public PhysicsGhostObject, public btActionInterface
{
    friend class Node;

public:

    /**
     * Flags for controlling how a character animation is played back.
     */
    enum AnimationFlags
    {
        /**
         * Plays an animation once and then stops.
         */
        ANIMATION_STOP,

        /**
         * Play an animation once and then resumes the previous playing animation.
         */
        ANIMATION_RESUME,

        /**
         * Plays an animation and repeats it indefinitely.
         */
         ANIMATION_REPEAT
    };

    /**
     * @see PhysicsCollisionObject#getType
     */
    PhysicsCollisionObject::Type getType() const;

    /**
     * Returns whether physics simulation is enabled for the physics character.
     *
     * @return true if physics simulation is enabled, false otherwise.
     *
     * @see setPhysicsEnabled(bool)
     */
    bool isPhysicsEnabled() const;

    /**
     * Enables or disables phyiscs simulation for the character.
     *
     * When physics simulation is enabled (default), the physics character automatically
     * responds to collisions in the physics world. For example, the character will
     * automatically slide along walls, step up stairs, react to gravity, etc.
     *
     * When disabled, the character will not have any physics applied on it and will
     * therefore be allowed to walk through walls, ceiling, floors, other objects, etc.
     *
     * @param enabled true to enable physics simulation, false otherwise.
     */
    void setPhysicsEnabled(bool enabled);

    /**
     * Returns the maximum step height for the character.
     *
     * @return The maximum step height.
     */
    float getMaxStepHeight() const;

    /**
     * Sets the maximum step height for the character.
     *
     * @param height The maximum step height.
     */
    void setMaxStepHeight(float height);

    /**
     * Returns the maximum slope angle for the character.
     *
     * The maximum slope angle determines the maximum angle of terrain
     * that the character can walk on. Slopes with an angle larger
     * than the specified angle will not allow the character to move on.
     *
     * @return The maximum slope angle.
     */
    float getMaxSlopeAngle() const;

    /**
     * Sets the maximum slope angle (in degrees).
     *
     * @param angle The maximum slope angle.
     */
    void setMaxSlopeAngle(float angle);

    /**
     * Configures a new animation for this character.
     *
     * This method registers an animation for the character, with an associated movement speed.
     * The moveSpeed specifies how fast the character moves while the animation is playing.
     * The final velocity of the character is the product of the current move velocity and
     * the currently playing animation(s) moveSpeed.
     *
     * @param name Name of the animation.
     * @param animationClip Animation clip associated with the new character animation.
     * @param moveSpeed Base movement speed (meters per second) associated with the animation.
     */
    void addAnimation(const char* name, AnimationClip* animationClip, float moveSpeed);

    /**
     * Returns the animation with the specified name.
     *
     * @return The specified animation clip.
     */
    AnimationClip* getAnimation(const char* name);

    /**
     * Plays the specified animation.
     *
     * There are some limiations and considerations that should be ponited out when
     * playing animations:
     * <li>You should avoid playing multiple animations concurrently that have the same target.
     * For example, two animations targetting the character's joints should not be played 
     * concurrently, but it is fine to play one animation that targets the joints and another
     * that targets the character's Node.
     * <li>When playing an animation that targets the transform of the character's Node
     * (such as a motion path animation), the character's velocity vector should be set to
     * Vector3::zero() so that the PhysicsCharacter stops applying motion directly
     * and instead relies on the motion animation to control the character.
     *
     * The optional animation layer can be used to group animations on separate layers.
     * Each animation layer can have at most one active animation. Playing multiple
     * animations concurrently can be achieved by putting the different animations
     * on separate layers. For example, a motion path animation that targets the
     * character's Node can be put on one layer, while a running animation that targets
     * a character's Joints can be put on a separate layer. This allows a character's
     * movement to be animated at the same time as the run animation is playing.
     *
     * @param name Animation name, or NULL to stop all character animations on the given layer.
     * @param flags Animation flags from the AnimationFlags enumeration.
     * @param speed Optional animation speed (default is 1.0).
     * @param blendDuration Optional number of milliseconds to crossfade between the
     *      currently playing animation on the given layer and the new animation.
     * @param layer Optional animation layer.
     */
    void play(const char* name, AnimationFlags flags, float animationSpeed = 1.0f, unsigned int blendDuration = 0, unsigned int layer = 0);

    /**
     * Sets the velocity of the character.
     *
     * Calling this function sets the velocity (speed and direction) for the character.
     * The velocity is maintained until this method is called again. The final velocity
     * of the character is determined by product of the current velocity vector(s)
     * and the current character animation's move speed. Therefore, specifying a
     * normalized (unit-length) velocity vector results in the character speed being
     * controled entirely by the current animation's velocity; whereas the speed of
     * the character can be augmented by modifying the magnitude of the velocity vector.

     * Note that a zero velocity vector and/or a zero animation move speed will
     * result in no character movement (the character will be stationary). A zero
     * velocity vector should be used when playing an animation that targets the
     * character's transform directly (such as a motion path animation), since these
     * animations will overwrite any transformations on the character's node.
     *
     * @param velocity Movement velocity.
     */
    void setVelocity(const Vector3& velocity);

    /**
     * Rotates the character.
     *
     * @param axis Axis of rotation.
     * @param angle Angle in radians.
     */
    void rotate(const Vector3& axis, float angle);

    /**
     * Rotates the character.
     *
     * @param rotation Quaternion representing the rotation to apply.
     */
    void rotate(const Quaternion& rotation);

    /**
     * Sets the rotation of the character.
     *
     * @param axis Axis of rotation.
     * @param angle Angle in radians.
     */
    void setRotation(const Vector3& axis, float angle);

    /**
     * Sets the rotation of the character.
     *
     * @param rotation Quaternion representing the new rotation.
     */
    void setRotation(const Quaternion& rotation);

    /**
     * Moves the character forward with the given velocity vector.
     *
     * The forward velocity is defined by the character's current orientation
     * (it is the forward vector from the character's current world transform).
     *
     * The specified velocity acts as a multiplier on the currently playing animation's
     * velocity (or, if there is no animation playing, it directly impacts velocity).
     *
     * Note that a negative velocity (i.e. -1.0f) will move the character backwards.
     *
     * @param velocity Optional velocity modifier.
     */
    void setForwardVelocity(float velocity = 1.0f);

    /**
     * Moves the character right with the given velocity vector.
     *
     * The right velocity is defined by the character's current orientation
     * (it is the right vector from the character's current world transform).
     *
     * The specified velocity acts as a multiplier on the currently playing animation's
     * velocity (or, if there is no animation playing, it directly impacts velocity).
     *
     * Note that a negative velocity (i.e. -1.0f) will move the character left.
     *
     * @param velocity Optional velocity modifier.
     */
    void setRightVelocity(float velocity = 1.0f);

    /**
     * Causes the character to jump with the specified initial upwards velocity.
     *
     * @param velocity Initial jump velocity.
     */
    void jump(float height);

    /**
     * @see btActionInterface::updateAction
     */
    void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

    /**
     * @see btActionInterface::debugDraw
     */
	void debugDraw(btIDebugDraw* debugDrawer);

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

private:

    struct CharacterAnimation
    {
        const char* name;
        AnimationClip* clip;
        float moveSpeed;
        unsigned int layer;
        bool playing;
        AnimationFlags animationFlags;
        unsigned int blendDuration;
        CharacterAnimation* prev;
    };

    /**
     * Creates a new PhysicsCharacter.
     *
     * Use PhysicsController::createCharacter to create physics characters.
     *
     * @param node Scene node that represents the character.
	 * @param shape Physis collision shape definition.
     */
	PhysicsCharacter(Node* node, const PhysicsCollisionShape::Definition& shape);

    /**
     * Destructor.
     *
     * Use PhysicsController::destroyCharacter to destroy physics characters.
     */
    virtual ~PhysicsCharacter();

    void updateCurrentVelocity();

    void play(CharacterAnimation* animation, unsigned int layer);

    void stepUp(btCollisionWorld* collisionWorld, btScalar time);

    void stepDown(btCollisionWorld* collisionWorld, btScalar time);

    void stepForwardAndStrafe(btCollisionWorld* collisionWorld, float time);

    void updateTargetPositionFromCollision(btVector3& targetPosition, const btVector3& collisionNormal);

    bool fixCollision(btCollisionWorld* world);

    btVector3 _moveVelocity;
    float _forwardVelocity;
    float _rightVelocity;
    btVector3 _fallVelocity;
    btVector3 _currentVelocity;
    btVector3 _normalizedVelocity;
    bool _colliding;
    btVector3 _collisionNormal;
    btVector3 _currentPosition;
    std::map<const char*, CharacterAnimation> _animations;
    std::map<unsigned int, CharacterAnimation*> _layers;
    btManifoldArray	_manifoldArray;
    float _stepHeight;
    float _slopeAngle;
    float _cosSlopeAngle;
    bool _physicsEnabled;
};

}

#endif
