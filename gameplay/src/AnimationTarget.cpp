/*
 * AnimationTarget.cpp
 */

#include "Base.h"
#include "AnimationTarget.h"
#include "Animation.h"
#include "Game.h"
#include <string.h>
#include "Transform.h"

namespace gameplay
{

AnimationTarget::AnimationTarget()
    : _targetType(SCALAR), _animations(NULL)
{
}

AnimationTarget::~AnimationTarget()
{
    if (_animations)
    {
        std::vector<Animation*>::iterator animationIter = _animations->begin();
        while (animationIter != _animations->end())
        {
            SAFE_RELEASE((*animationIter));
            animationIter++;
        }
        SAFE_DELETE(_animations);
    }
}

void AnimationTarget::addAnimation(Animation* animation)
{
    if (_animations == NULL)
    {
        _animations = new std::vector<Animation*>;
    }

    _animations->push_back(animation);

    animation->addRef();
}

unsigned int AnimationTarget::getAnimationCount() const
{
    if (_animations)
        return _animations->size();

    return 0;
}

Animation* AnimationTarget::getAnimation(unsigned int index) const
{
    if (_animations)
        return _animations->at(index);
    else
        return 0;
}

Animation* AnimationTarget::getAnimation(const char* id) const
{
    if (_animations)
    {
        std::vector<Animation*>::iterator animationIter = _animations->begin();
        while(animationIter != _animations->end())
        {
            if ((*animationIter)->_id.compare(id) == 0)
            {
                return *animationIter;
            }

            animationIter++;
        }
    }
    
    return NULL;
}

}



