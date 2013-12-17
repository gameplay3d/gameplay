#ifndef AUDIOLISTENER_H_
#define AUDIOLISTENER_H_

#include "Vector3.h"
#include "Transform.h"
#include "Camera.h"

namespace gameplay
{

/**
 * Defines an audio listener in 3D space.
 *
 * The audio listener represents where you are listening from.
 * In a 3D scene this is usually the active camera.
 * 
 * @seee Scene::bindAudioListenerToCamera
 */
class AudioListener : public Camera::Listener
{
    friend class AudioController;
    friend class Game;

public:

    /**
     * Gets the single instance of the audio listener.
     *
     * @return The single instance of the audio listener.
     */
    static AudioListener* getInstance();

    /**
     * Gets the current position of the audio listener.
     *
     * @return The position of the audio listener
     */
    const Vector3& getPosition() const;

    /**
     * Sets the position of the audio source.
     *
     * @param position The position to set the listener to.
     */
    void setPosition(const Vector3& position);

    /**
     * Sets the position of the audio source.
     * 
     * @param x The x coordinate of the position.
     * @param y The y coordinate of the position.
     * @param z The z coordinate of the position.
     */
    void setPosition(float x, float y, float z);

    /**
     * Returns the gain of the audio listener.
     *
     * @return The gain of the audio listener.
     */
    float getGain() const;

    /**
     * Sets the gain/volume of the audio listener.
     *
     * @param gain The gain/volume of the listener.
     */
    void setGain(float gain);

    /**
     * Gets the velocity of the audio source.
     *
     * @return The velocity as a vector.
     */
    const Vector3& getVelocity() const;

    /**
     * Sets the velocity of the audio source
     *
     * @param velocity A vector representing the velocity.
     */
    void setVelocity(const Vector3& velocity);

    /**
     * Sets the velocity of the audio source
     * 
     * @param x The x coordinate of the velocity.
     * @param y The y coordinate of the velocity.
     * @param z The z coordinate of the velocity.
     */
    void setVelocity(float x, float y, float z);

    /**
     * Gets the float pointer to the orientation of the audio listener.
     * Orientation is represented as 6 floats. (forward.x, forward.y, forward.z, up.x, up.y, up.z).
     * 
     * @return Pointer to the 6 orientation float values.
     * @script{ignore}
     */
    const float* getOrientation() const;

    /**
     * Gets the forward orientation vector of the audio listener.
     *
     * @return The forward vector.
     */
    const Vector3& getOrientationForward() const;

    /**
     * Gets the up orientation vector of the audio listener.
     *
     * @return The up vector.
     */
    const Vector3& getOrientationUp() const;

    /**
     * Sets the orientation of the audio listener.
     *
     * @param forward The forward vector.
     * @param up The up vector.
     */
    void setOrientation(const Vector3& forward, const Vector3& up);

    /**
     * Sets the orientation of the audio listener.
     * 
     * @param forwardX The x coordinate of the forward vector.
     * @param forwardY The y coordinate of the forward vector.
     * @param forwardZ The z coordinate of the forward vector.
     * @param upX The x coordinate of the up vector.
     * @param upY The y coordinate of the up vector.
     * @param upZ The z coordinate of the up vector.
     */
    void setOrientation(float forwardX, float forwardY, float forwardZ, float upX, float upY, float upZ);

    /**
     * Gets the camera currently associated with the audio listener.
     *
     * @return camera The camera currently associated with the audio listener.
     */
    Camera* getCamera() const;

    /**
     * Sets the camera that is associated with the audio listener. This should usually be the current camera.
     *
     * @param camera The camera that is associated with the audio listener
     */
    void setCamera(Camera* camera);

private:

    /**
    * Constructor.
    */
    AudioListener();

    /**
    * Destructor.
    */
    ~AudioListener();

    /**
    * @see Camera::Listener::cameraChanged
    */
    void cameraChanged(Camera* camera);

    float _gain;
    Vector3 _position;
    Vector3 _velocity;
    Vector3 _orientation[2];
    Camera* _camera;
};

}

#endif
