#include "AnimationChannel.h"

namespace gameplay
{

AnimationChannel::AnimationChannel(void) :
    targetAttrib(0)
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
    write(targetId, file);
    write(targetAttrib, file);
    write(keytimes, file);
    write(keyValues, file);
    write(tangentsIn, file);
    write(tangentsOut, file);
    write(interpolations, file);
}

void AnimationChannel::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "targetId", targetId);
    fprintfElement(file, "targetAttrib", targetAttrib);
    fprintfElement(file, "%f ", "keytimes", keytimes);
    fprintfElement(file, "%f ", "values", keyValues);
    fprintfElement(file, "%f ", "tangentsIn", tangentsIn);
    fprintfElement(file, "%f ", "tangentsOut", tangentsOut);
    fprintfElement(file, "%u ", "interpolations", interpolations);
    fprintElementEnd(file);
}

void AnimationChannel::setTargetId(const std::string str)
{
    targetId = str;
}

void AnimationChannel::setTargetAttribute(unsigned int attrib)
{
    targetAttrib = attrib;
}

void AnimationChannel::setKeyTimes(const std::vector<float>& values)
{
    keytimes = values;
}

void AnimationChannel::setKeyValues(const std::vector<float>& values)
{
    keyValues = values;
}

void AnimationChannel::setTangentsIn(const std::vector<float>& values)
{
    tangentsIn = values;
}

void AnimationChannel::setTangentsOut(const std::vector<float>& values)
{
    tangentsOut = values;
}

void AnimationChannel::setInterpolations(const std::vector<unsigned int>& values)
{
    interpolations = values;
}

const std::vector<float>& AnimationChannel::getKeyValues() const
{
    return keyValues;
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
