/*
 * AnimationController.h
 */

#ifndef ANIMATIONCONTROLLER_H_
#define ANIMATIONCONTROLLER_H_

#include "AnimationClip.h"
#include "Animation.h"
#include "AnimationTarget.h"

namespace gameplay
{

/**
 * Defines a class for controlling game animation.
 */
class AnimationController
{
    friend class Game;
    friend class AnimationClip;

public:

    /**
     * Creates an animation on this target from a set of key value and key time pairs. 
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     * 
     * @param id The ID of the animation.
     * @param target The animation target.
     * @param propertyId The property on this target to animate.
     * @param keyCount The number of keyframes in the animation. Must be greater than one.
     * @param keyTimes The list of key times for the animation (in milliseconds).
     * @param keyValues The list of key values for the animation.
     * @param type The curve interpolation type.
     *
     * @return The newly created animation, or NULL if an animation with the given ID already exists.
     */
    Animation* createAnimation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, Curve::InterpolationType type);

    /**
     * Creates an animation on this target from a set of key value and key time pairs.
     * 
     * @param id The ID of the animation.
     * @param target The animation target.
     * @param propertyId The property on this target to animate.
     * @param keyCount The number of keyframes in the animation. Must be greater than one.
     * @param keyTimes The list of key times for the animation (in milliseconds).
     * @param keyValues The list of key values for the animation.
     * @param keyInValue The list of key in values for the animation.
     * @param keyOutValue The list of key out values for the animation.
     * @param type The curve interpolation type.
     *
     * @return The newly created animation, or NULL if an animation with the given ID already exists.
     */
    Animation* createAnimation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, Curve::InterpolationType type);

    /**
     * Creates a simple two keyframe from-to animation.
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     *
     * @param id The ID of the animation.
     * @param target The animation target.
     * @param propertyId The property on this target to animate.
     * @param from The values to animate from.
     * @param to The values to animate to.
     * @param type The curve interpolation type.
     * @param duration The duration of the animation (in milliseconds).
     *
     * @return The newly created animation, or NULL if an animation with the given ID already exists.
     */
    Animation* createAnimationFromTo(const char* id, AnimationTarget* target, int propertyId, float* from, float* to, Curve::InterpolationType type, unsigned long duration);

    /**
     * Creates a simple two keyframe from-by animation.
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     *
     * @param id The ID of the animation.
     * @param target The animation target.
     * @param propertyId The property on this target to animate.
     * @param from The values to animate from.
     * @param by The values to animate by.
     * @param type The curve interpolation type.
     * @param duration The duration of the animation (in milliseconds).
     *
     * @return The newly created animation, or NULL if an animation with the given ID already exists.
     */
    Animation* createAnimationFromBy(const char* id, AnimationTarget* target, int propertyId, float* from, float* by, Curve::InterpolationType type, unsigned long duration);

    /**
     * Finds the animation with the given ID.
     *
     * @param id The ID of the animation to get. NULL if the Animation is not found.
     * 
     * @return The animation, or NULL if not found.
     */
    Animation* getAnimation(const char* id) const;

    /** 
     * Stops all AnimationClips currently playing on the AnimationController.
     */
    void stopAllAnimations();
       
private:

    enum State
    {
        RUNNING,
        IDLE,
        PAUSED
    };

    /**
     * Constructor.
     */
    AnimationController();

    /**
     * Constructor.
     */
    AnimationController(const AnimationController& copy);

    /**
     * Destructor.
     */
    ~AnimationController();
    
    /**
     * Gets the controller's state.
     *
     * @return The current state.
     */
    State getState() const;

    /**
     * Callback for when the controller is initialized.
     */
    void initialize();

    /*
     * Callback for when the controller is finalized.
     */
    void finalize();

    /**
     * Resumes the AnimationController.
     */
    void resume();
    
    /**
     * Pauses the AnimationController.
     */
    void pause();

    /**
     * Schedules an AnimationClip to run.
     */
    void schedule(AnimationClip* clip);

    /**
     * Unschedules an AnimationClip.
     */
    void unschedule(AnimationClip* clip);

    /**
     * Callback for when the controller receives a frame update event.
     */
    void update(long elapsedTime);
    
    /**
     * Adds an animation on this AnimationTarget.
     */ 
    void addAnimation(Animation* animation);

    /**
     * Removes the given animation from this AnimationTarget.
     */
    void destroyAnimation(Animation* animation);

    /**
     * Removes all animations from the AnimationTarget.
     */ 
    void destroyAllAnimations();

    State _state;                               // The current state of the AnimationController.
    std::list<AnimationClip*> _runningClips;    // A list of currently running AnimationClips.
    std::vector<Animation*> _animations;
    //Animation** _animations;                    // A list of animations on this target.
};

}

#endif

