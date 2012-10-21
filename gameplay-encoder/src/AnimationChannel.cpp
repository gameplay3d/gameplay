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
    write((unsigned int)_keytimes.size(), file);
    for (std::vector<float>::const_iterator i = _keytimes.begin(); i != _keytimes.end(); ++i)
    {
        write((unsigned int)*i, file);
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
    fprintf(file, "<%s>%u %s</%s>\n", "targetAttrib", _targetAttrib, Transform::getPropertyString(_targetAttrib), "targetAttrib");
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

std::vector<float>& AnimationChannel::getKeyValues()
{
    return _keyValues;
}

std::vector<float>& AnimationChannel::getKeyTimes()
{
    return _keytimes;
}

std::vector<float>& AnimationChannel::getTangentsIn()
{
    return _tangentsIn;
}

std::vector<float>& AnimationChannel::getTangentsOut()
{
    return _tangentsOut;
}

std::vector<unsigned int>& AnimationChannel::getInterpolationTypes()
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
    LOG(3, "      Removing duplicates for channel with target attribute: %u.\n", _targetAttrib);

    int startCount = _keytimes.size();

    size_t propSize = Transform::getPropertySize(_targetAttrib);

    if (propSize > 1 && !_interpolations.empty() && _interpolations[0] == LINEAR)
    {
        size_t prevIndex = 0;

        std::vector<float>::iterator prevStart = _keyValues.begin();
        std::vector<float>::iterator prevEnd = prevStart + propSize - 1;
        
        size_t i = 1;
        for (i = 1; i < _keytimes.size(); ++i)
        {
            std::vector<float>::iterator start = _keyValues.begin() + i * propSize;
            std::vector<float>::iterator end = start + propSize - 1;

            if (!equal(prevStart, prevEnd, start) || i == _keytimes.size() - 1)
            {
                if (i - prevIndex > 2)
                {
                    deleteRange(prevIndex+1, i, propSize);
                    i = prevIndex;
                    prevStart = _keyValues.begin() + i * propSize;
                    prevEnd = prevStart + propSize - 1;
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
            deleteRange(prevIndex+1, i, propSize);
        }
    }

    LOG(3, "      Removed %d duplicate keyframes from channel.\n", startCount- _keytimes.size());
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

void AnimationChannel::deleteRange(size_t begin, size_t end, size_t propSize)
{
    assert(end > begin);

    // delete range
    LOG(4, "        delete %lu to %lu\n", begin, end - 1);

    std::vector<float>::iterator a = _keyValues.begin() + begin * propSize;
    std::vector<float>::iterator b = _keyValues.begin() + end * propSize;
    _keyValues.erase(a, b);

    a = _keytimes.begin() + begin;
    b = _keytimes.begin() + end;
    _keytimes.erase(a, b);

    if (_interpolations.size() > 1)
    {
        std::vector<unsigned int>::iterator a = _interpolations.begin() + begin;
        std::vector<unsigned int>::iterator b = _interpolations.begin() + end * propSize;
        _interpolations.erase(a, b);
    }

    // TODO: also remove key frames from _tangentsIn and _tangentsOut once other curve types are supported.
}

}
