#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Ref.h"

namespace gameplay
{

class AnimationTarget;
class AnimationController;
class AnimationClip;
class Curve;

/**
 * Defines a generic property animation.
 *
 * To run an animation you must play an AnimationClip.
 * Every Animation has the default clip which will run from begin-end time.
 * You can create additional clips to run only parts of an animation and control
 * various runtime characteristics, such as repeat count, etc.
 */
class Animation : public Ref
{
    friend class AnimationController;
    friend class AnimationClip;
    friend class AnimationTarget;
    friend class Package;

public:
    
    /**
     * Gets the Animation's ID.
     * 
     * @return The Animation's ID.
     */
    const char* getId() const;
    
    /**
     * Gets the Animation's duration.
     *
     * @return The Animation's duration (in milliseconds).
     */
    unsigned long getDuration() const;

    /**
     * Creates an AnimationClip from an .animation file.
     */
    void createClips(const char* animationFile);
    
    /**
     * Creates an AnimationClip from the Animation.
     * 
     * @param id The ID to the give the AnimationClip.
     * @param start The time (in milliseconds) that the AnimationClip will begin from.
     * @param end The time (in milliseconds) that the AnimationClip will end.
     * 
     * @return The newly created AnimationClip; NULL if an AnimationClip already exists with the same ID.
     */
    AnimationClip* createClip(const char* id, unsigned long start, unsigned long end);
    
    /**
     * Finds the AnimationClip with the specified name. If NULL, gets the default clip.
     * 
     * @param clipId The ID of the AnimationClip to get.
     *
     * @return The AnimationClip with the specified ID; NULL if an AnimationClip with the given ID is not found.
     */
    AnimationClip* getClip(const char* clipId = NULL);
    
    /**
     * Plays the AnimationClip with the specified name. 
     *
     * @param clipId The ID of the AnimationClip to play. If NULL, plays the default clip.
     */ 
    void play(const char* clipId = NULL);

    /**
     * Stops the AnimationClip with the specified name. 
     * 
     * @param clipId The ID of the AnimationClip to stop. If NULL, stops the default clip.
     */
    void stop(const char* clipId = NULL);

private:

    /**
     * Defines a channel which holds the target, target property, curve values, and duration.
     *
     * An animation can have 1 or more channels. All typical simple property animations
     * will have 1 channel. Skeletal animation will have 1 channel per joint to be animated.
     */
    class Channel
    {
        friend class AnimationClip;
        friend class Animation;
        friend class AnimationTarget;

    private:

        Channel(Animation* animation, AnimationTarget* target, int propertyId, Curve* curve, unsigned long duration);
        Channel(const Channel& copy);
        ~Channel();

        Animation* _animation;                // Reference to the animation this channel belongs to.
        AnimationTarget* _target;             // The target of this channel.
        int _propertyId;                      // The target property this channel targets.
        Curve* _curve;                        // The curve used to represent the animation data.
        unsigned long _duration;              // The length of the animation (in milliseconds).
    };

    /**
     * Constructor.
     */
    Animation();

    /**
     * Constructor.
     */
    Animation(const Animation& copy);

    /**
     * Constructor.
     */
    Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type);
    
    /**
     * Constructor.
     */
    Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, unsigned int type);

    /**
     * Destructor.
     */
    ~Animation();

    /**
     * Adds a clip to this Animation.
     */
    void addClip(AnimationClip* clip);

    /**
     * Finds the clip with the given ID.
     */
    AnimationClip* findClip(const char* id) const;

    /**
     * Creates the default clip.
     */
    void createDefaultClip();

    /**
     * Creates a channel within this animation.
     */ 
    Channel* createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, unsigned int type);

    /**
     * Creates a channel within this animation.
     */
    Channel* createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type);

    /**
     * Adds a channel to the animation.
     */
    void addChannel(Channel* channel);

    /**
     * Removes a channel from the animation.
     */
    void removeChannel(Channel* channel);
    
    AnimationController* _controller;       // The AnimationController that this Animation will run on.
    std::string _id;                        // The Animation's ID.
    unsigned long _duration;                // the length of the animation (in milliseconds).
    std::vector<Channel*> _channels;        // The channels within this Animation.
    AnimationClip* _defaultClip;            // The Animation's default clip.
    std::vector<AnimationClip*>* _clips;    // All the clips created from this Animation.

};

}

#endif
