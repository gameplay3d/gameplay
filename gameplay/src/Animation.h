#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Ref.h"
#include "Properties.h"
#include "Curve.h"

namespace gameplay
{

class AnimationTarget;
class AnimationController;
class AnimationClip;

/**
 * Defines a generic property animation.
 *
 * To run an animation you must play an AnimationClip.
 * Every Animation has the default clip which will run from begin-end time.
 * You can create additional clips to run only parts of an animation and control
 * various runtime characteristics, such as repeat count, etc.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Animation
 */
class Animation : public Ref
{
    friend class AnimationClip;
    friend class AnimationTarget;
    friend class Bundle;

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
     * Creates an AnimationClip from the Properties object defined at the specified URL,
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional).
     *
     * @param url The URL pointing to the Properties object containing the clip definitions.
     */
    void createClips(const char* url);

    /**
     * Creates an AnimationClip from the Animation.
     *
     * @param id The ID to the give the AnimationClip.
     * @param start The time (in milliseconds) that the AnimationClip will begin from.
     * @param end The time (in milliseconds) that the AnimationClip will end.
     *
     * @return The newly created AnimationClip; NULL if an AnimationClip already exists with the same ID.
     * @script{create}
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
     * Returns the AnimationClip at the given index.
     *
     * @param index Index of the clip to return.
     */
    AnimationClip* getClip(unsigned int index) const;

    /**
     * Returns the number of animation clips in this animation.
     */
    unsigned int getClipCount() const;

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

    /**
     * Pauses the AnimationClip with the specified name.
     *
     * @param clipId The ID of the AnimationClip to pause. If NULL, pauses the default clip.
     */
    void pause(const char* clipId = NULL);

    /**
     * Returns true if this animation targets the given AnimationTarget.
     */
    bool targets(AnimationTarget* target) const;

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
        Channel(const Channel& copy, Animation* animation, AnimationTarget* target);
        Channel(const Channel&); // Hidden copy constructor.
        ~Channel();
        Channel& operator=(const Channel&); // Hidden copy assignment operator.
        Curve* getCurve() const;

        Animation* _animation;                // Reference to the animation this channel belongs to.
        AnimationTarget* _target;             // The target of this channel.
        int _propertyId;                      // The target property this channel targets.
        Curve* _curve;                        // The curve used to represent the animation data.
        unsigned long _duration;              // The length of the animation (in milliseconds).
    };

    /**
     * Hidden copy constructor.
     */
    Animation(const Animation& copy);

    /**
     * Constructor.
     */
    Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type);

    /**
     * Constructor.
     */
    Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, unsigned int type);

    /**
     * Constructor.
     */
    Animation(const char* id);

    /**
     * Destructor.
     */
    ~Animation();

    /**
     * Hidden copy assignment operator.
     */
    Animation& operator=(const Animation&);

    /**
     * Creates the default clip.
     */
    void createDefaultClip();

    /**
     * Creates AnimationClip's for this Animation from the specified Property object.
     */
    void createClips(Properties* animationProperties, unsigned int frameCount);

    /**
     * Adds a clip to this Animation.
     */
    void addClip(AnimationClip* clip);

    /**
     * Finds the clip with the given ID.
     */
    AnimationClip* findClip(const char* id) const;

    /**
     * Creates a channel within this animation.
     */
    Channel* createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, unsigned int type);

    /**
     * Creates a channel within this animation.
     */
    Channel* createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type);

    /**
     * Adds a channel to the animation.
     */
    void addChannel(Channel* channel);

    /**
     * Removes a channel from the animation.
     */
    void removeChannel(Channel* channel);

    /**
     * Sets the rotation offset in a Curve representing a Transform's animation data.
     */
    void setTransformRotationOffset(Curve* curve, unsigned int propertyId);

    /**
     * Clones this animation.
     *
     * @param channel The channel to clone and add to the animation.
     * @param target The target of the animation.
     *
     * @return The newly created animation.
     */
    Animation* clone(Channel* channel, AnimationTarget* target);

    AnimationController* _controller;       // The AnimationController that this Animation will run on.
    std::string _id;                        // The Animation's ID.
    unsigned long _duration;                // the length of the animation (in milliseconds).
    std::vector<Channel*> _channels;        // The channels within this Animation.
    AnimationClip* _defaultClip;            // The Animation's default clip.
    std::vector<AnimationClip*>* _clips;    // All the clips created from this Animation.

};

}

#endif
