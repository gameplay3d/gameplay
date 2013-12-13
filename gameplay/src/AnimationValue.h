#ifndef ANIMATIONVALUE_H_
#define ANIMATIONVALUE_H_

#include "Animation.h"

namespace gameplay
{

/**
 * Defines a running animation value which can have one or more floats.
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
     * Copies one or more float values from this AnimationValue into the specified array.
     *
     * @param index The index to start copying from.
     * @param values Pointer to float array to copy values into.
     * @param count Number of values to copy.
     */
    void getFloats(unsigned int index, float* values, unsigned int count) const;

    /**
     * Copies one or more float values into the AnimationValue.
     *
     * @param index The index of the first component to set the value for.
     * @param values Array of values to copy into the AnimationValue.
     * @param count Number of values to in the array to copy in.
     */
    void setFloats(unsigned int index, float* values, unsigned int count);

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

    /**
     * Hidden copy assignment operator.
     */
    AnimationValue& operator=(const AnimationValue& v);

    unsigned int _componentCount;   // The number of float values for the property.
    unsigned int _componentSize;    // The number of bytes of memory the property is.
    float* _value;                  // The current value of the property.

};

}

#endif
