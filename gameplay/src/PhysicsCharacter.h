#ifndef PHYSICSCHARACTER_H_
#define PHYSICSCHARACTER_H_

#include "Node.h"
#include "PhysicsGhostObject.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Defines a physics controller class for a game character.
 *
 * This can be used to control the movements and collisions of a character
 * in a game. It interacts with the Physics system to apply gravity and handle
 * collisions, however dynamics are not applied to the character directly by the
 * physics system. Instead, the character's movement is controlled directly by the
 * PhysicsCharacter class. This results in a more responsive and typical game
 * character than would be possible if trying to move a character by applying
 * physical simulation with forces.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-Collision_Objects
 */
class PhysicsCharacter : public PhysicsGhostObject
{
    friend class Node;

public:

    /**
     * @see PhysicsCollisionObject::getType
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
     * Enables or disables physics simulation for the character.
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
     * Sets the velocity of the character.
     *
     * Calling this function sets the velocity (speed and direction) for the character.
     * The velocity is maintained until this method is called again. The final velocity
     * of the character is determined by product of the current velocity, right and
     * forward vectors.
     *
     * @param velocity Movement velocity.
     */
    void setVelocity(const Vector3& velocity);

    /**
     * Sets the velocity of the character.
     *
     * Calling this function sets the velocity (speed and direction) for the character.
     * The velocity is maintained until this method is called again. The final velocity
     * of the character is determined by product of the current velocity, right and
     * forward vectors.
     * 
     * @param x The x coordinate of the velocity vector.
     * @param y The y coordinate of the velocity vector.
     * @param z The z coordinate of the velocity vector.
     */
    void setVelocity(float x, float y, float z);

    /**
     * Resets the internal velocity state which brings the character to an immediate stop
     *
    */
    void resetVelocityState();

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
     * Note that a negative velocity (i.e. -1.0f) will move the character left.
     *
     * @param velocity Optional velocity modifier.
     */
    void setRightVelocity(float velocity = 1.0f);

    /**
     * Returns the current velocity of the character.
     *
     * @return The current velocity.
     */
    Vector3 getCurrentVelocity() const;

    /**
     * Causes the character to jump to the specified height.
     *
     * @param height The amount to jump.
     * @param force Set true to force the character to jump even if vertical velocity is non-zero
     */
    void jump(float height, bool force = false);

protected:

    /**
     * @see PhysicsCollisionObject::getCollisionObject
     */
    btCollisionObject* getCollisionObject() const;

private:

    /**
     * Creates a new PhysicsCharacter.
     *
     * Use PhysicsController::createCharacter to create physics characters.
     *
     * @param node Scene node that represents the character.
     * @param shape Physics collision shape definition.
     * @param mass The mass of the character.
     * @param group Group identifier
     * @param mask Bitmask field for filtering collisions with this object.
     */
    PhysicsCharacter(Node* node, const PhysicsCollisionShape::Definition& shape, float mass, int group = PHYSICS_COLLISION_GROUP_DEFAULT, int mask = PHYSICS_COLLISION_MASK_DEFAULT);

    /**
     * Destructor.
     *
     * Use PhysicsController::destroyCharacter to destroy physics characters.
     */
    virtual ~PhysicsCharacter();

    /**
     * Creates a physics character from the specified properties object.
     * 
     * @param node The node to create a physics character for; note that the node must have
     *      a model attached to it prior to creating a physics character for it.
     * @param properties The properties object defining the physics character (must have namespace equal to 'character').
     * @return The newly created physics character, or <code>NULL</code> if the physics character failed to load.
     */
    static PhysicsCharacter* create(Node* node, Properties* properties);

    void updateCurrentVelocity();

    void stepUp(btCollisionWorld* collisionWorld, btScalar time);

    void stepDown(btCollisionWorld* collisionWorld, btScalar time);

    void stepForwardAndStrafe(btCollisionWorld* collisionWorld, float time);

    void updateTargetPositionFromCollision(btVector3& targetPosition, const btVector3& collisionNormal);

    bool fixCollision(btCollisionWorld* world);

    /**
     * Hides the callback interfaces within the PhysicsCharacter.
     * @script{ignore}
     */
    class ActionInterface : public btActionInterface
    {
    public:

        ActionInterface(PhysicsCharacter* character);

        void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

        void debugDraw(btIDebugDraw* debugDrawer);
        
        PhysicsCharacter* character;
    };

    void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

    btVector3 _moveVelocity;
    float _forwardVelocity;
    float _rightVelocity;
    btVector3 _verticalVelocity;
    btVector3 _currentVelocity;
    btVector3 _normalizedVelocity;
    bool _colliding;
    btVector3 _collisionNormal;
    btVector3 _currentPosition;
    btManifoldArray _manifoldArray;
    float _stepHeight;
    float _slopeAngle;
    float _cosSlopeAngle;
    bool _physicsEnabled;
    float _mass;
    ActionInterface* _actionInterface;
};

}

#endif
