#ifndef ANIMATIONTARGET_H_
#define ANIMATIONTARGET_H_

#include "Curve.h"
#include "AnimationController.h"
#include "CloneContext.h"

namespace gameplay
{

class Animation;
class AnimationValue;

/**
 * Defines an interface allowing animation to target
 * an object for changing its animation properties.
 */
class AnimationTarget
{
    friend class Animation;
    friend class AnimationClip;
    friend class AnimationController;

public:

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

protected:
    
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
     * Adds the given animation channel to this animation target.
     * 
     * @param channel The animation channel to add.
     */
    void addChannel(Animation::Channel* channel);

    /**
     * Deletes the given animation channel from this animation target.
     * 
     * @param channel The animation channel to delete.
     */
    void deleteChannel(Animation::Channel* channel);

    /**
     * Copies data from this animation target into the given target for the purpose of cloning.
     * 
     * @param target The target to copy into.
     * @param context The clone context.
     */
    void cloneInto(AnimationTarget* target, CloneContext &context) const;

    TargetType _targetType;             // The type of target this is.

    char _animationPropertyBitFlag;     // Bit flag used to indicate which properties on the AnimationTarget are currently animating.

private:

    /**
     * Constructor.
     */
    AnimationTarget(const AnimationTarget& copy);

    /**
     * Gets the TargetType's property ID value for the specified property ID string.
     * 
     * @param type The TargetType of the AnimationTarget.
     * @param propertyIdStr The property ID string.
     * @return The property ID value for teh property ID string; -1 if the propertyIdStr does not exist
     *    for the TargetType.
     */
    static int getPropertyId(TargetType type, const char* propertyIdStr);

    std::vector<Animation::Channel*>* _animationChannels;   // Collection of all animation channels that target the AnimationTarget
    
};
}

#endif
