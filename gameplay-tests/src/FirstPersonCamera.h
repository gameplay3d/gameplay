#ifndef FIRSTPERSONCAMERA_H_
#define FIRSTPERSONCAMERA_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * FirstPersonCamera controls a camera like a first person shooter game.
 */
class FirstPersonCamera
{
public:

    /**
     * Constructor.
     */
    FirstPersonCamera();

    /**
     * Destructor.
     */
    ~FirstPersonCamera();

    /**
     * Initializes the first person camera. Should be called after the Game has been initialized.
     */
    void initialize(float nearPlane = 1.0f, float farPlane = 1000.0f, float fov = 45.0f);

    /**
     * Gets root node. May be NULL if not initialized.
     * 
     * @return Root node or NULL.
     */
    Node* getRootNode();

    /**
     * Gets the camera. May be NULL.
     * 
     * @return Camera or NULL.
     */
    Camera* getCamera();

    /**
     * Sets the position of the camera.
     * 
     * @param position The position to move to.
     */
    void setPosition(const Vector3& position);

    /**
     * Moves the camera forward in the direction that it is pointing. (Fly mode)
     */
    void moveForward(float amount);

    /**
     * Moves the camera in the opposite direction that it is pointing.
     */
    void moveBackward(float amount);

    /**
     * Strafes that camera left, which is perpendicular to the direction it is facing.
     */
    void moveLeft(float amount);

    /**
     * Strafes that camera right, which is perpendicular to the direction it is facing.
     */
    void moveRight(float amount);

    void moveUp(float amount);

    void moveDown(float amount);

    /**
     * Rotates the camera in place in order to change the direction it is looking.
     * 
     * @param yaw Rotates the camera around the yaw axis in radians. Positive looks right, negative looks left.
     * @param pitch Rotates the camera around the ptich axis in radians. Positive looks up, negative looks down.
     */
    void rotate(float yaw, float pitch);

private:

    Node* _pitchNode;
    Node* _rootNode;
};

#endif
