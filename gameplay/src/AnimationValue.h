#ifndef ANIMATIONVALUE_H_
#define ANIMATIONVALUE_H_

#include "Animation.h"

namespace gameplay
{

/**
 * The runtime interface to represent an animation value.
 */
class AnimationValue
{
    friend class AnimationClip;

public:

    /**
     * Gets the value at the specified index.
     *
     * @param index The index of the component to get the value for.
     *
     * @return The float value at the specified index.
     */
    float getFloat(unsigned int index) const;

    /**
     * Sets the value at the specified index.
     *
     * @param index The index of the component to set the value for.
     * @param value The value to set the component to.
     */
    void setFloat(unsigned int index, float value);

    /**
     * Gets the value of the AnimationValue in a float array.
     *
     * @param value The array to populate with the AnimationValue's values.
     * @param offset The offset into the value to start populating.
     * @param length The number of values to copy into the array.
     */
    void getFloat(float* value, unsigned int offset, unsigned int length) const;

    /**
     * Sets the value of the AnimationValue.
     *
     * @param value The array to populate the AnimationValue's values.
     * @param offset The offset into the value array to start populating from.
     * @param length The number of values to copy into the AnimationValue.
     */
    void setFloat(float* value, unsigned int offset, unsigned int length);

private:

    /**
     * Constructor.
     */
    AnimationValue();

    /**
     * Constructor.
     */
    AnimationValue(unsigned int componentCount);

    /**
     * Constructor.
     */
    AnimationValue(const AnimationValue& copy);

    /**
     * Destructor.
     */
    ~AnimationValue();

    bool _isFirstActing;            // Flag indicating if this value's channel is the first to act on the target.
    unsigned int _componentCount;   // The number of float values for the property.
    unsigned int _componentSize;    // The number of bytes of memory the property is.
    float* _currentValue;           // The current value of the property.
    float* _interpolatedValue;      // The last interpolated value of the property.
};

}

#endif
