/*
 * AnimationClip.h
 */

#ifndef ANIMATIONCLIP_H_
#define ANIMATIONCLIP_H_

#include "Base.h"
#include "AnimationValue.h"
#include "Curve.h"
#include "Animation.h"

namespace gameplay
{

class Animation;
class AnimationValue;

/**
 * Defines the runtime session of an Animation to be played.
 */
class AnimationClip : public Ref
{
    friend class AnimationController;
    friend class Animation;

public:

    /**
     * Defines a constant for indefinitely repeating an AnimationClip.
     */
    static const unsigned int REPEAT_INDEFINITE = 0;

    /**
     * Defines an animation event listener.
     */
    class Listener
    {
    public:

        /**
         * The type of animation event.
         */
        enum EventType 
        {
            /**
             * Event fired when the clip begins.
             */
            BEGIN,

            /**
             * Event fired when the clip ends.
             */
            END
        };

        /**
         * Handles when a transform has changed.
         */
        virtual void animationEvent(AnimationClip* clip, EventType type) = 0;
    };

    /**
     * Gets the AnimationClip's ID.
     *
     * @return The AnimationClip's ID.
     */
    const char* getID() const;

    /**
     * Gets the Animation that this AnimationClip was created from.
     * 
     * @return The Animation that this clip was created from.
     */
    Animation* getAnimation() const;

    /**
     * Gets the AnimationClip's start time.
     *
     * @return The time (in milliseconds) that the AnimationClip starts playing from.
     */
    unsigned long getStartTime() const;

    /**
     * Gets the AnimationClip's end time.
     * 
     * @return The time (in milliseconds) that the AnimationClip will end.
     */
    unsigned long getEndTime() const;

    /**
     * Gets the AnimationClip's elapsed time.
     *
     * @return The elapsed time of the AnimationClip (in milliseconds).
     */
    unsigned long getElaspedTime() const;

    /**
     * Sets the AnimationClip's repeat count. Overrides repeat duration.
     *
     * Use ANIMATION_REPEAT_INDEFINITE to play the AnimationClip indefinitely.
     * 
     * @param repeatCount The repeat count to set on the AnimationClip.
     */
    void setRepeatCount(float repeatCount);

    /**
     * Gets the AnimationClip's repeat count.
     *
     * @return The repeat count that is set on the AnimationClip.
     */
    float getRepeatCount() const;

    /**
     * Sets the AnimationClip's active duration. Overrides repeat count.
     *
     * Use ANIMATION_REPEAT_INDEFINITE to play the AnimationClip indefinitely.
     *
     * @param duration The active duration that is set on the AnimationClip.
     */
    void setActiveDuration(unsigned long duration);

    /**
     * Gets the AnimationClip's active duration.
     * 
     * @return the AnimationClip's active duration.
     */
    unsigned long getActiveDuration() const;

    /**
     * Set the AnimationClip's running speed. 
     *
     * @param speed The clips running speed.
     */
    void setSpeed(float speed);

    /**
     * Gets the AninimationClip's running speed.
     *
     * @return The AninimationClip's running speed.
     */
    float getSpeed() const;

    /**
     * Checks if the AnimationClip is playing.
     *
     * @return true if the AnimationClip is playing; false if the AnimationClip is not playing.
     */
    bool isPlaying() const;

    /**
     * Plays the AnimationClip.
     */
    void play();

    /**
     * Stops the AnimationClip.
     */
    void stop();

    /**
     * Adds a animation begin listener.
     *
     * @param listener The listener to be called when an animation clip begins.
     */
    void addBeginListener(AnimationClip::Listener* listener);

    /**
     * Adds a animation end listener.
     *
     * @param listener The listener to be called when an animation clip ends.
     */
    void addEndListener(AnimationClip::Listener* listener);

private:

    /**
     * Constructor.
     */
    AnimationClip(const char* id, Animation* animation, unsigned long startTime, unsigned long endTime);

    /**
     * Constructor.
     */
    AnimationClip();

    /**
     * Constructor.
     */
    AnimationClip(const AnimationClip& copy);

    /**
     * Destructor.
     */
    ~AnimationClip();

    /**
     * Updates the animation with the elapsed time.
     */
    bool update(unsigned long elapsedTime);

    /**
     * Handles when the AnimationClip begins.
     */
    void onBegin();

    /**
     * Handles when the AnimationClip ends.
     */
    void onEnd();

    std::string _id;                          // AnimationClip ID.
    Animation* _animation;                    // Animations that this clip plays in parallel.
    unsigned long _startTime;                 // Start time of the clip.
    unsigned long _endTime;                   // End time of the clip.
    unsigned long _elapsedTime;               // Time elapsed while the clip is running.
    long _runningTime;                        // Keeps track of the Animation's relative time in respect to the active duration.
    unsigned int _channelCount;               // The number of channels in the clip.
    std::vector<AnimationValue*> _values;     // AnimationValue holder.
    float _repeatCount;                       // The clip's repeat count.
    float _speed;                             // The speed that the clip is playing. Default is 1.0. Negative goes in reverse.
    unsigned long _duration;                  // The total duration.
    unsigned long _activeDuration;            // The active duration of the clip.
    bool _isPlaying;                          // A flag to indicate whether the clip is playing.
    std::vector<Listener*>* _beginListeners;
    std::vector<Listener*>* _endListeners;
};

}

#endif
