#include "Base.h"
#include "AnimationChannel.h"

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
    for (std::vector<float>::const_iterator i = _keytimes.begin(); i != _keytimes.end(); i++)
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

}
