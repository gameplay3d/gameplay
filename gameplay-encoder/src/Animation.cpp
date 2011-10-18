#include "Animation.h"

namespace gameplay
{

Animation::Animation(void)
{
}

Animation::~Animation(void)
{
}

unsigned int Animation::getTypeId(void) const
{
    return ANIMATION_ID;
}
const char* Animation::getElementName(void) const
{
    return "Animation";
}

void Animation::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    // Write AnimationChannels
    write(channels.size(), file);
    for (std::vector<AnimationChannel*>::iterator i = channels.begin(); i != channels.end(); i++)
    {
        (*i)->writeBinary(file);
    }
}

void Animation::writeText(FILE* file)
{   
    fprintElementStart(file);
    if (channels.size() > 0 )
    {
        for (std::vector<AnimationChannel*>::iterator i = channels.begin(); i != channels.end(); i++)
        {
            (*i)->writeText(file);
        }
    }
    fprintElementEnd(file);
}

void Animation::add(AnimationChannel* animationChannel)
{
    channels.push_back(animationChannel);
}

unsigned int Animation::getAnimationChannelCount() const
{
    return channels.size();
}

}
