#include "Base.h"
#include "AnimationTarget.h"
#include "Animation.h"
#include "Game.h"
#include "Transform.h"

namespace gameplay
{

AnimationTarget::AnimationTarget()
    : _targetType(SCALAR), _animationPropertyBitFlag(0x00), _animationChannels(NULL)
{
}

AnimationTarget::~AnimationTarget()
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();
        while (itr != _animationChannels->end())
        {
            Animation::Channel* channel = (*itr);
            SAFE_DELETE(channel);
            itr++;
        }
        _animationChannels->clear();
        SAFE_DELETE(_animationChannels);
    }
}

void AnimationTarget::addChannel(Animation::Channel* channel)
{
    if (_animationChannels == NULL)
        _animationChannels = new std::vector<Animation::Channel*>;

    _animationChannels->push_back(channel);
}

int AnimationTarget::getPropertyId(TargetType type, const char* propertyIdStr)
{
    if (type == AnimationTarget::TRANSFORM)
    {
        if (strcmp(propertyIdStr, "ANIMATE_SCALE") == 0)
        {
            return Transform::ANIMATE_SCALE;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_SCALE_X") == 0)
        {
            return Transform::ANIMATE_SCALE_X;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_SCALE_Y") == 0)
        {
            return Transform::ANIMATE_SCALE_Y;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_SCALE_Z") == 0)
        {
            return Transform::ANIMATE_SCALE_Z;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_ROTATE") == 0)
        {
            return Transform::ANIMATE_ROTATE;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_TRANSLATE") == 0)
        {
            return Transform::ANIMATE_TRANSLATE;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_TRANSLATE_X") == 0)
        {
            return Transform::ANIMATE_TRANSLATE_X;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_TRANSLATE_Y") == 0)
        {
            return Transform::ANIMATE_TRANSLATE_Y;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_TRANSLATE_Z") == 0)
        {
            return Transform::ANIMATE_TRANSLATE_Z;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_ROTATE_TRANSLATE") == 0)
        {
            return Transform::ANIMATE_ROTATE_TRANSLATE;
        }
        else if (strcmp(propertyIdStr, "ANIMATE_SCALE_ROTATE_TRANSLATE") == 0)
        {
            return Transform::ANIMATE_SCALE_ROTATE_TRANSLATE;
        }
    }
    else
    {
        if (strcmp(propertyIdStr, "ANIMATE_UNIFORM") == 0)
        {
            return MaterialParameter::ANIMATE_UNIFORM;
        }
    }

    return -1;
}

}



