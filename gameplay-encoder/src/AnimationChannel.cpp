#include "Base.h"
#include "AnimationChannel.h"
#include "Transform.h"

namespace gameplay
{

AnimationChannel::AnimationChannel(void) :
    _targetAttrib(0)
{
}

AnimationChannel::~AnimationChannel(void)
{
}

unsigned int AnimationChannel::getTypeId(void) const
{
    return ANIMATIONCHANNEL_ID;
}
const char* AnimationChannel::getElementName(void) const
{
    return "AnimationChannel";
}

void AnimationChannel::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_targetId, file);
    write(_targetAttrib, file);
    write(_keytimes.size(), file);
    for (std::vector<float>::const_iterator i = _keytimes.begin(); i != _keytimes.end(); ++i)
    {
        write((unsigned long)*i, file);
    }
    write(_keyValues, file);
    write(_tangentsIn, file);
    write(_tangentsOut, file);
    write(_interpolations, file);
}

void AnimationChannel::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "targetId", _targetId);
    fprintfElement(file, "targetAttrib", _targetAttrib);
    fprintfElement(file, "%f ", "keytimes", _keytimes);
    fprintfElement(file, "%f ", "values", _keyValues);
    fprintfElement(file, "%f ", "tangentsIn", _tangentsIn);
    fprintfElement(file, "%f ", "tangentsOut", _tangentsOut);
    fprintfElement(file, "%u ", "interpolations", _interpolations);
    fprintElementEnd(file);
}

void AnimationChannel::setInterpolation(unsigned int interpolation)
{
    _interpolations.clear();
    _interpolations.push_back(interpolation);
}

const std::string& AnimationChannel::getTargetId() const
{
    return _targetId;
}

unsigned int AnimationChannel::getTargetAttribute() const
{
    return _targetAttrib;
}

const std::vector<float>& AnimationChannel::getKeyValues() const
{
    return _keyValues;
}

const std::vector<float>& AnimationChannel::getKeyTimes() const
{
    return _keytimes;
}

const std::vector<float>& AnimationChannel::getTangentsIn() const
{
    return _tangentsIn;
}

const std::vector<float>& AnimationChannel::getTangentsOut() const
{
    return _tangentsOut;
}

const std::vector<unsigned int>& AnimationChannel::getInterpolationTypes() const
{
    return _interpolations;
}

void AnimationChannel::setTargetId(const std::string& str)
{
    _targetId = str;
}

void AnimationChannel::setTargetAttribute(unsigned int attrib)
{
    _targetAttrib = attrib;
}

void AnimationChannel::setKeyTimes(const std::vector<float>& values)
{
    _keytimes = values;
}

void AnimationChannel::setKeyValues(const std::vector<float>& values)
{
    _keyValues = values;
}

void AnimationChannel::setTangentsIn(const std::vector<float>& values)
{
    _tangentsIn = values;
}

void AnimationChannel::setTangentsOut(const std::vector<float>& values)
{
    _tangentsOut = values;
}

void AnimationChannel::setInterpolations(const std::vector<unsigned int>& values)
{
    _interpolations = values;
}

void AnimationChannel::removeDuplicates()
{
    if (_targetAttrib == Transform::ANIMATE_SCALE_ROTATE_TRANSLATE)
    {
        size_t prevIndex = 0;

        std::vector<float>::iterator prevStart = _keyValues.begin();
        std::vector<float>::iterator prevEnd   = _keyValues.begin() + 9;
        
        size_t i = 1;
        for (i = 1; i < _keytimes.size(); ++i)
        {
            std::vector<float>::iterator start = _keyValues.begin() + i * 10;
            std::vector<float>::iterator end = _keyValues.begin() + (i * 10 + 9);

            if (!equal(prevStart, prevEnd, start))
            {
                if (i - prevIndex > 2)
                {
                    deleteRange(prevIndex+1, i);
                    i = prevIndex;
                    prevStart = _keyValues.begin() + i * 10;
                    prevEnd = _keyValues.begin() + (i * 10 + 9);
                }
                else
                {
                    prevStart = start;
                    prevEnd = end;
                    prevIndex = i;
                }
            }
        }
        if (i - 1 - prevIndex >= 2)
        {
            deleteRange(prevIndex+1, i);
        }
    }
}

void AnimationChannel::convertToQuaternion()
{
    if (_targetAttrib == Transform::ANIMATE_ROTATE_X ||
        _targetAttrib == Transform::ANIMATE_ROTATE_Y ||
        _targetAttrib == Transform::ANIMATE_ROTATE_Z)
    {
        std::vector<float> newKeyValues;
        newKeyValues.resize(_keyValues.size() * 4);
        const size_t count = _keyValues.size();

        float x = _targetAttrib == Transform::ANIMATE_ROTATE_X ? 1.0f : 0.0f;
        float y = _targetAttrib == Transform::ANIMATE_ROTATE_Y ? 1.0f : 0.0f;
        float z = _targetAttrib == Transform::ANIMATE_ROTATE_Z ? 1.0f : 0.0f;
        for (size_t i = 0; i < count; ++i)
        {
            size_t j = i << 2;
            newKeyValues[j] = x;
            newKeyValues[j+1] = y;
            newKeyValues[j+2] = z;
            newKeyValues[j+3] = _keyValues[i];
        }
        setKeyValues(newKeyValues);
        setTargetAttribute(Transform::ANIMATE_ROTATE);
    }
}

void AnimationChannel::convertToTransform()
{
    if (_targetAttrib == Transform::ANIMATE_ROTATE_X ||
        _targetAttrib == Transform::ANIMATE_ROTATE_Y ||
        _targetAttrib == Transform::ANIMATE_ROTATE_Z)
    {
        std::vector<float> newKeyValues;
        newKeyValues.resize(_keyValues.size() * 10);
        const size_t count = _keyValues.size();

        float x = _targetAttrib == Transform::ANIMATE_ROTATE_X ? 1.0f : 0.0f;
        float y = _targetAttrib == Transform::ANIMATE_ROTATE_Y ? 1.0f : 0.0f;
        float z = _targetAttrib == Transform::ANIMATE_ROTATE_Z ? 1.0f : 0.0f;
        for (size_t i = 0; i < count; ++i)
        {
            size_t j = i << 2;
            newKeyValues[j+0] = 1.0f;
            newKeyValues[j+1] = 1.0f;
            newKeyValues[j+2] = 1.0f;
            newKeyValues[j+3] = x;
            newKeyValues[j+4] = y;
            newKeyValues[j+5] = z;
            newKeyValues[j+6] = _keyValues[i];
            newKeyValues[j+7] = 0.0f;
            newKeyValues[j+8] = 0.0f;
            newKeyValues[j+9] = 0.0f;
        }
        setKeyValues(newKeyValues);
        setTargetAttribute(Transform::ANIMATE_SCALE_ROTATE_TRANSLATE);
    }
}

unsigned int AnimationChannel::getInterpolationType(const char* str)
{
    unsigned int value = 0;
    switch (str[0])
    {
    case 'L':
        if (strcmp(str, "LINEAR") == 0)
        {
            value = AnimationChannel::LINEAR;
        }
        break;
    case 'B':
        if (strcmp(str, "BEZIER") == 0)
        {
            value = AnimationChannel::BEZIER;
        }
        else if (strcmp(str, "BSPLINE") == 0)
        {
            value = AnimationChannel::BSPLINE;
        }
        break;
    case 'C':
        if (strcmp(str, "CARDINAL") == 0)
        {
            value = AnimationChannel::CARDINAL;
        }
        break;
    case 'H':
        if (strcmp(str, "HERMITE") == 0)
        {
            value = AnimationChannel::HERMITE;
        }
        break;
    case 'S':
        if (strcmp(str, "STEP") == 0)
        {
            value = AnimationChannel::STEP;
        }
        break;
    default:
        break;
    }
    return value;
}

void AnimationChannel::deleteRange(size_t begin, size_t end)
{
    // delete range
    printf("delete %lu to %lu\n", begin, end - 1);

    std::vector<float>::iterator a = _keyValues.begin() + begin * 10;
    std::vector<float>::iterator b = _keyValues.begin() + end * 10;
    _keyValues.erase(a, b);

    a = _keytimes.begin() + begin;
    b = _keytimes.begin() + end;
    _keytimes.erase(a, b);

    if (_interpolations.size() > 1)
    {
        std::vector<unsigned int>::iterator a = _interpolations.begin() + begin;
        std::vector<unsigned int>::iterator b = _interpolations.begin() + end * 10;
        _interpolations.erase(a, b);
    }
}

}
