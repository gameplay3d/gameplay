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

AnimationValue::AnimationValue(const AnimationValue& copy)
{
    _value = new float[copy._componentCount];
    _componentSize = copy._componentSize;
    _componentCount = copy._componentCount;
    memcpy(_value, copy._value, _componentSize);
}

AnimationValue::~AnimationValue()
{
    SAFE_DELETE_ARRAY(_value);
}

AnimationValue& AnimationValue::operator=(const AnimationValue& v)
{
    if (this != &v)
    {
        if (_value == NULL || _componentSize != v._componentSize || _componentCount != v._componentCount)
        {
            _componentSize = v._componentSize;
            _componentCount = v._componentCount;
            SAFE_DELETE_ARRAY(_value);
            _value = new float[v._componentCount];
        }
        memcpy(_value, v._value, _componentSize);
    }
    return *this;
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
