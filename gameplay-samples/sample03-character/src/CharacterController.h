#ifndef CHARACTERCONTROLLER_H_
#define CHARACTERCONTROLLER_H_

#include "gameplay.h"

namespace gameplay
{

/**
 * Controller class for a human-like game character.
 *
 * This class can be used to control the movements and collisions of a character
 * in a game. It interacts with the Physics system to apply gravity and handle
 * collisions, however it does not behave like a typical physics rigid body in
 * that real-world physics is not applied.
 *
 * Animation clips can be setup for typical character animations, such as walk,
 * run, jump, etc; and the controller will handle blending between these
 * animations as needed.
 */
class CharacterController
{
public:

    /**
     * Creates a new character controller.
     *
     * @param node Scene node that represents the character.
     */
    CharacterController(Node* node);

    /**
     * Destructor.
     */
    virtual ~CharacterController();

    /**
     * Moves the character by the given 
    void move(const Vector3& velocity);*/

    /**
     * Configures a new movement for the character.
     */
    void addMovement(const char* name, AnimationClip* animationClip, const Vector3& velocity);

    /**
     * Sets the character's movement to the movement with the specified name.
     */
    void setMovement(const char* name);

    /**
     * Stops the character controller.
     */
    void stop();

    /**
     * Updates this character controller.
     */
    void update(long elapsedTime);

private:

    struct Movement
    {
        AnimationClip* clip;    // animation clip for movement
        Vector3 velocity;       // meters per second
    };

    Node* _node;
    std::map<const char*, Movement> _movements;
    Movement* _currentMovement;

};

}

#endif
