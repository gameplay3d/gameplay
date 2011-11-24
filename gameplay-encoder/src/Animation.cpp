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
    // Animation writes its ID because it is not listed in the ref table.
    write(getId(), file);
    write(_channels.size(), file);
    for (std::vector<AnimationChannel*>::iterator i = _channels.begin(); i != _channels.end(); ++i)
    {
        (*i)->writeBinary(file);
    }
}

void Animation::writeText(FILE* file)
{
    fprintElementStart(file);
    if (_channels.size() > 0 )
    {
        for (std::vector<AnimationChannel*>::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            (*i)->writeText(file);
        }
    }
    fprintElementEnd(file);
}

void Animation::add(AnimationChannel* animationChannel)
{
    _channels.push_back(animationChannel);
}

unsigned int Animation::getAnimationChannelCount() const
{
    return _channels.size();
}

}
