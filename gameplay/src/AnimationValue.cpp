#include "Base.h"
#include "AnimationValue.h"

namespace gameplay
{

AnimationValue::AnimationValue(unsigned int componentCount)
  : _componentCount(componentCount), _componentSize(componentCount * sizeof(float))
{
    GP_ASSERT(_componentCount > 0);
    _value = new float[_componentCount];
}

AnimationValue::~AnimationValue()
{
    SAFE_DELETE_ARRAY(_value);
}

float AnimationValue::getFloat(unsigned int index) const
{
    GP_ASSERT(index < _componentCount);
    GP_ASSERT(_value);

    return _value[index];
}

void AnimationValue::setFloat(unsigned int index, float value)
{
    GP_ASSERT(index < _componentCount);
    GP_ASSERT(_value);

    _value[index] = value;
}

void AnimationValue::getFloat(float* value, unsigned int offset, unsigned int length) const
{
    GP_ASSERT(_value && value && offset < _componentCount && (offset + length) <= _componentCount);

    memcpy(value + offset, _value, length * sizeof(float));
}

void AnimationValue::setFloat(float* value, unsigned int offset, unsigned int length)
{
    GP_ASSERT(_value && value && offset < _componentCount && (offset + length) <= _componentCount);

    memcpy(_value, value + offset, length * sizeof(float));
}

}
