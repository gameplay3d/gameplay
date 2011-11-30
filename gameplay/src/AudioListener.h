#ifndef AUDIOLISTENER_H_
#define AUDIOLISTENER_H_

#include "Vector3.h"
#include "Transform.h"

namespace gameplay
{

class Camera;

/**
 * Defines an audio listener in 3D space.
 */
class AudioListener : public Transform::Listener
{
    friend class AudioController;

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
    * @see Transform::Listener::transformChanged
    */
    void transformChanged(Transform* transform, long cookie);

    float _gain;
    Vector3 _position;
    Vector3 _velocity;
    Vector3 _orientation[2];
    Camera* _camera;
};

}

#endif
