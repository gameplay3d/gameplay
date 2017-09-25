#pragma once

namespace gameplay
{

    class Vector3;

/**
 * Defines the audio listener in scene.
 *
 * The audio listener represents where you are listening from.
 * It is always connected and maintains state to the active camera.
 */
class AudioListener
{
public:

    /**
     * Constructor.
     */
    AudioListener();

    /**
     * Destructor.
     */
    ~AudioListener();

    /**
     * Gets the gain of the audio listener.
     *
     * @return The gain of the audio listener.
     */
    virtual float getGain() const = 0;

    /**
     * Sets the gain of the audio listener.
     *
     * @param gain The gain/volume of the listener.
     */
    virtual void setGain(float gain) = 0;

    /**
     * Events occurs when the position changes.
     *
     * @param position The changed position to be updated.
     */
    virtual void onPositionChanged(const Vector3& position) = 0;

    /**
     * Event occurs when the velocity changes.
     *
     * @param velocity The velocity to change to.
     */
    virtual void onVelocityChanged(const Vector3& velocity) = 0;

    /**
     * Event occurs when active camera orientation changes.
     *
     * @param forward The forward vector to change to.
     * @param up The up vector to change to.
     */
    virtual void onOrientationChanged(const Vector3& forward, const Vector3& up) = 0;
};

}
