#include "Base.h"
#include "AnimationTarget.h"
#include "Animation.h"
#include "Game.h"
#include <string.h>
#include "Transform.h"

namespace gameplay
{

AnimationTarget::AnimationTarget()
    : _targetType(SCALAR), _highestPriority(NULL), _animationChannels(NULL)
{
}

AnimationTarget::~AnimationTarget()
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();
        while (itr != _animationChannels->end())
        {
            SAFE_DELETE((*itr));
            itr++;
        }
        SAFE_DELETE(_animationChannels);
    }
}

void AnimationTarget::addChannel(Animation::Channel* channel)
{
    if (_animationChannels == NULL)
        _animationChannels = new std::vector<Animation::Channel*>;

    _animationChannels->push_back(channel);
}

}



