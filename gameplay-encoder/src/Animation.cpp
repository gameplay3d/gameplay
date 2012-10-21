#include "Base.h"
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
    write((unsigned int)_channels.size(), file);
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

void Animation::remove(AnimationChannel* animationChannel)
{
    std::vector<AnimationChannel*>::iterator it = std::find(_channels.begin(), _channels.end(), animationChannel);
    if (it != _channels.end())
    {
        _channels.erase(it);
    }
}

unsigned int Animation::getAnimationChannelCount() const
{
    return _channels.size();
}

AnimationChannel* Animation::getAnimationChannel(unsigned int index) const
{
    assert(index < _channels.size());
    return _channels[index];
}

}
