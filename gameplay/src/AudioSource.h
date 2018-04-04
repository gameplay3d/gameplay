#pragma once

#include "Component.h"

namespace gameplay
{
   
    class Vector3;

/**
 *  Defines an audio source to play an audio clip in a scene.
 */
class AudioSource  : public Component
{
    friend class Audio;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * The audio source's state.
     */
    enum class State
    {
        eInitial,
        ePlaying,
        ePaused,
        eStopped
    };

    /**
     * Constructor.
     */
    AudioSource();

    /**
     * Destructor.
     */
    ~AudioSource();

    /**
     * Plays the audio source.
     */
    virtual void play() = 0;

    /**
     * Pauses the playing of the audio source.
     */
    virtual void pause() = 0;

    /**
     * Resumes playing of the audio source.
     */
    virtual void resume() = 0;

    /**
     * Stops the playing of the audio source.
     */
    virtual void stop() = 0;

    /**
     * Rewinds the audio source to the beginning.
     */
    virtual void rewind() = 0;

    /**
     * Gets the state of the audio source.
     *
     * @return ePlaying if the source is playing,
               eSopped if the source is stopped,
               ePaused if the source is paused and eInitial otherwise.
     */
    virtual AudioSource::State getState() const = 0;

    /**
     * Determines whether the audio source is looped or not.
     *
     * @return true if the audio source is looped, false if not.
     */
    virtual bool isLooped() const = 0;

    /**
     * Sets the state of the audio source to be looping or not.
     *
     * @param looped true to loop the sound, false to not loop it.
     */
    virtual void setLooped(bool looped) = 0;

    /**
     * Gets the gain of the audio source.
     *
     * @return The gain.
     */
    virtual float getGain() const = 0;

    /**
     * Sets the gain/volume of the audio source.
     *
     * @param gain The gain/volume of the source.
     */
    virtual void setGain(float gain) = 0;

    /**
     * Gets the pitch of the audio source.
     *
     * @return The pitch.
     */
    virtual float getPitch() const = 0;

    /**
     * Sets the pitch of the audio source.
     *
     * @param pitch The pitch of the source.
     */
    virtual void setPitch(float pitch) = 0;

    /**
     * Event occurs when the postition changes.
     *
     * @param position The changed position to be updated.
     */
    virtual void onPositionChanged(const Vector3& position) = 0;

    /**
     * Event occurs when the velocity changes of an attached rigid body changes.
     *
     * @param velocity The changed velocity to be updated.
     */
    virtual void onVelocityChanged(const Vector3& velocity) = 0;

protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();
    
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);
};

}
