/*
 * PropertyListener
 */

#ifndef ANIMATIONTARGET_H_
#define ANIMATIONTARGET_H_

#include "Curve.h"
#include "AnimationController.h"

namespace gameplay
{

class Animation;
class AnimationValue;

/**
 * Defines an interface allowing animation to target
 * an object for changing its animation properties.
 */
class AnimationTarget : public Ref
{
    friend class Animation;
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
     */
    virtual void setAnimationPropertyValue(int propertyId, AnimationValue* value) = 0;

    /**
     * Gets the number of Animations on this target.
     * 
     * @return The number of Animations targeting this object.
     */
    unsigned int getAnimationCount() const;

    /**
     * Gets the Animation with the given index.
     * 
     * @param index The index of the Animation to return.
     *
     * @return The Animation at the given index.
     */
    Animation* getAnimation(unsigned int index) const;

    /**
     * Finds the Animation with the given ID.
     * 
     * @param id The ID of the Animation to get.
     * 
     * @return The Animation with the given ID. NULL if the Animation is not found.
     */
    Animation* getAnimation(const char* id) const;

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

    void addAnimation(Animation* animation);

    TargetType _targetType;             // The type of target this is.

private:

    /**
     * Constructor.
     */
    AnimationTarget(const AnimationTarget& copy);

    std::vector<Animation*>* _animations;

};
}

#endif
