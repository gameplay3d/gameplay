#include "Base.h"
#include "AnimationTarget.h"
#include "Animation.h"
#include "Game.h"

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
            channel->_animation->removeChannel(channel);
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

void AnimationTarget::deleteChannel(Animation::Channel* channel)
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();
        for ( ; itr != _animationChannels->end(); itr++)
        {
            Animation::Channel* temp = *itr;
            if (channel == temp)
            {
                SAFE_DELETE(channel);
                _animationChannels->erase(itr);

                if (_animationChannels->empty())
                    SAFE_DELETE(_animationChannels);

                return;
            }
        }
    }
}

void AnimationTarget::cloneInto(AnimationTarget* target, CloneContext &context) const
{
    if (_animationChannels)
    {
        for (std::vector<Animation::Channel*>::const_iterator it = _animationChannels->begin(); it != _animationChannels->end(); ++it)
        {
            Animation::Channel* channel = *it;
            assert(channel->_animation);

            bool animationCloned = false;

            // Don't clone the Animaton if it is already in the CloneContext.
            Animation* animation = context.findClonedAnimation(channel->_animation);
            if (animation == NULL)
            {
                animation = channel->_animation->clone();
                animationCloned = true;
            }
            assert(animation);

            context.registerClonedAnimation(channel->_animation, animation);
            
            Animation::Channel* channelCopy = new Animation::Channel(*channel, animation, target);
            animation->addChannel(channelCopy);
        }
    }
}

}



