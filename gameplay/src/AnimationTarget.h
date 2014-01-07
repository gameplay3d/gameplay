#ifndef ANIMATIONTARGET_H_
#define ANIMATIONTARGET_H_

#include "Curve.h"
#include "AnimationController.h"

namespace gameplay
{

class Animation;
class AnimationValue;
class NodeCloneContext;

/**
 * Defines an interface allowing animation to target
 * an object for changing its animation properties.
 */
class AnimationTarget
{
    friend class Animation;
    friend class AnimationClip;

public:

    /**
     * Creates an animation on this target from a set of key value and key time pairs.
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     *
     * @param id The ID of the animation.
     * @param propertyId The property on this target to animate.
     * @param keyCount The number of keyframes in the animation. Must be greater than one.
     * @param keyTimes The list of key times for the animation (in milliseconds).
     * @param keyValues The list of key values for the animation.
     * @param type The curve interpolation type.
     *
     * @return The newly created animation.
     */
    Animation* createAnimation(const char* id, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, Curve::InterpolationType type);

    /**
     * Creates an animation on this target from a set of key value and key time pairs.
     *
     * @param id The ID of the animation.
     * @param propertyId The property on this target to animate.
     * @param keyCount The number of keyframes in the animation. Must be greater than one.
     * @param keyTimes The list of key times for the animation (in milliseconds).
     * @param keyValues The list of key values for the animation.
     * @param keyInValue The list of key in values for the animation.
     * @param keyOutValue The list of key out values for the animation.
     * @param type The curve interpolation type.
     *
     * @return The newly created animation.
     */
    Animation* createAnimation(const char* id, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, Curve::InterpolationType type);

    /**
     * Creates an animation on this target using the data from the Properties object defined at the specified URL,
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional).
     *
     * @param id The ID of the animation.
     * @param url The URL pointing to the Properties object defining the animation data.
     *
     * @return The newly created animation.
     */
    Animation* createAnimation(const char* id, const char* url);

    /**
     * Creates an animation on this target using the data from the given properties object.
     *
     * @param id The ID of the animation.
     * @param animationProperties The properties object defining the animation data.
     *
     * @return The newly created animation.
     */
    Animation* createAnimation(const char* id, Properties* animationProperties);

    /**
     * Creates a simple two keyframe from-to animation.
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     *
     * @param id The ID of the animation.
     * @param propertyId The property on this target to animate.
     * @param from The values to animate from.
     * @param to The values to animate to.
     * @param type The curve interpolation type.
     * @param duration The duration of the animation (in milliseconds).
     *
     * @return The newly created animation.
     */
    Animation* createAnimationFromTo(const char* id, int propertyId, float* from, float* to, Curve::InterpolationType type, unsigned long duration);

    /**
     * Creates a simple two keyframe from-by animation.
     * Cannot use Curve::BEZIER or CURVE::HERMITE as the interpolation type since they require tangents/control points.
     *
     * @param id The ID of the animation.
     * @param propertyId The property on this target to animate.
     * @param from The values to animate from.
     * @param by The values to animate by.
     * @param type The curve interpolation type.
     * @param duration The duration of the animation (in milliseconds).
     *
     * @return The newly created animation.
     */
    Animation* createAnimationFromBy(const char* id, int propertyId, float* from, float* by, Curve::InterpolationType type, unsigned long duration);

    /**
     * Destroys the animation with the specified ID. Destroys the first animation if ID is NULL.
     *
     * @param id The ID of the animation to destroy.
     */
    void destroyAnimation(const char* id = NULL);

    /**
     * Abstract method to return the property component count of the given property ID on the AnimationTarget.
     *
     * @param propertyId The ID of the property on the AnimationTarget to obtain the component count for.
     *
     * @return The property component count of the given property.
     */
    virtual unsigned int getAnimationPropertyComponentCount(int propertyId) const = 0;

    /**
     * Abstract method for getting the animation property value for the given property ID on the AnimationTarget.
     *
     * @param propertyId The ID of the property on the AnimationTarget to get the animation property value for.
     * @param value The container to get the animation property value in.
     */
    virtual void getAnimationPropertyValue(int propertyId, AnimationValue* value) = 0;

    /**
     * Abstract method for setting the animation property value for the given property ID on the AnimationTarget.
     *
     * @param propertyId The ID of the property on the AnimationTarget to set the animation property value on.
     * @param value The container to set the animation property value in.
     * @param blendWeight The blend weight.
     */
    virtual void setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight = 1.0f) = 0;

    /**
     * Gets the animation with the specified ID. If the ID is NULL, this function will return the first animation it finds.
     *
     * @param id The name of the animation to get.
     */
    Animation* getAnimation(const char* id = NULL) const;

protected:

    /**
     * The type of animation target.
     */
    enum TargetType
    {
        SCALAR,
        TRANSFORM
    };

    /**
     * Constructor.
     */
    AnimationTarget();

    /**
     * Destructor.
     */
    virtual ~AnimationTarget();

	/**
     * Gets the TargetType's property ID value for the specified property ID string.
     *
     * @param type The TargetType of the AnimationTarget.
     * @param propertyIdStr The property ID string.
     * @return The property ID value for the property ID string; -1 if the propertyIdStr does not exist
     *    for the TargetType.
     */
    virtual int getPropertyId(TargetType type, const char* propertyIdStr);

    /**
     * Adds the given animation channel to this animation target.
     *
     * @param channel The animation channel to add.
     */
    void addChannel(Animation::Channel* channel);

    /**
     * Removes the given animation channel from this animation target.
     *
     * @param channel The animation channel to delete.
     */
    void removeChannel(Animation::Channel* channel);

    /**
     * Gets the Animation::Channel that belongs to the Animation with the specified ID.
     *
     * @param id The ID of the Animation the Channel belongs to.
     */
    Animation::Channel* getChannel(const char* id) const;

    /**
     * Copies data from this animation target into the given target for the purpose of cloning.
     *
     * @param target The target to copy into.
     * @param context The clone context.
     */
    void cloneInto(AnimationTarget* target, NodeCloneContext &context) const;

    /**
     * The target's type.
     *
     * @see TargetType::SCALAR
     * @see TargetType::TRANSFORM
     */
    TargetType _targetType;

private:

    /**
     * Constructor.
     */
    AnimationTarget(const AnimationTarget& copy);

    /**
     * Converts by-value animations to to-value animations.
     */
    void convertByValues(unsigned int propertyId, unsigned int componentCount, float* from, float* by);

    /**
     * Converts a Quaternion by-value into a to-value.
     */
    void convertQuaternionByValues(float* from, float* by);

    /**
     * Converts a Scale by-value into a to-value.
     */
    void convertScaleByValues(float* from, float* by, unsigned int componentCount);

    /**
     * Converts a by-value into a to-value.
     */
    void convertByValues(float* from, float* by, unsigned int componentCount);

    std::vector<Animation::Channel*>* _animationChannels;   // Collection of all animation channels that target the AnimationTarget

};
}

#endif
