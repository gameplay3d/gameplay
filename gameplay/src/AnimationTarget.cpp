#include "Base.h"
#include "AnimationTarget.h"
#include "Animation.h"
#include "Game.h"
#include "Node.h"
#include "MaterialParameter.h"

#define ANIMATION_TARGET_INDEFINITE_STR "INDEFINITE"

namespace gameplay
{

AnimationTarget::AnimationTarget()
    : _targetType(SCALAR), _animationChannels(NULL)
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
            GP_ASSERT(channel);
            GP_ASSERT(channel->_animation);
            channel->_animation->removeChannel(channel);
            SAFE_DELETE(channel);
            itr++;
        }
        _animationChannels->clear();
        SAFE_DELETE(_animationChannels);
    }
}

Animation* AnimationTarget::createAnimation(const char* id, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, Curve::InterpolationType type)
{
    GP_ASSERT(type != Curve::BEZIER && type != Curve::HERMITE);
    GP_ASSERT(keyCount >= 1 && keyTimes && keyValues);

    Animation* animation = new Animation(id, this, propertyId, keyCount, keyTimes, keyValues, type);

    return animation;
}

Animation* AnimationTarget::createAnimation(const char* id, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, Curve::InterpolationType type)
{
    GP_ASSERT(keyCount >= 1 && keyTimes && keyValues && keyInValue && keyOutValue);
    Animation* animation = new Animation(id, this, propertyId, keyCount, keyTimes, keyValues, keyInValue, keyOutValue, type);

    return animation;
}

Animation* AnimationTarget::createAnimation(const char* id, const char* url)
{
    Properties* p = Properties::create(url);
    GP_ASSERT(p);

    Animation* animation = createAnimation(id, (strlen(p->getNamespace()) > 0) ? p : p->getNextNamespace());

    SAFE_DELETE(p);

    return animation;
}

Animation* AnimationTarget::createAnimationFromTo(const char* id, int propertyId, float* from, float* to, Curve::InterpolationType type, unsigned long duration)
{
    GP_ASSERT(from);
    GP_ASSERT(to);

    const unsigned int propertyComponentCount = getAnimationPropertyComponentCount(propertyId);
    GP_ASSERT(propertyComponentCount > 0);
    float* keyValues = new float[2 * propertyComponentCount];

    memcpy(keyValues, from, sizeof(float) * propertyComponentCount);
    memcpy(keyValues + propertyComponentCount, to, sizeof(float) * propertyComponentCount);

    unsigned int* keyTimes = new unsigned int[2];
    keyTimes[0] = 0;
    keyTimes[1] = (unsigned int)duration;

    Animation* animation = createAnimation(id, propertyId, 2, keyTimes, keyValues, type);

    SAFE_DELETE_ARRAY(keyValues);
    SAFE_DELETE_ARRAY(keyTimes);

    return animation;
}

Animation* AnimationTarget::createAnimationFromBy(const char* id, int propertyId, float* from, float* by, Curve::InterpolationType type, unsigned long duration)
{
    GP_ASSERT(from);
    GP_ASSERT(by);

    const unsigned int propertyComponentCount = getAnimationPropertyComponentCount(propertyId);
    GP_ASSERT(propertyComponentCount > 0);
    float* keyValues = new float[2 * propertyComponentCount];

    memcpy(keyValues, from, sizeof(float) * propertyComponentCount);

    convertByValues(propertyId, propertyComponentCount, from, by);
    memcpy(keyValues + propertyComponentCount, by, sizeof(float) * propertyComponentCount);

    unsigned int* keyTimes = new unsigned int[2];
    keyTimes[0] = 0;
    keyTimes[1] = (unsigned int)duration;

    Animation* animation = createAnimation(id, propertyId, 2, keyTimes, keyValues, type);

    SAFE_DELETE_ARRAY(keyValues);
    SAFE_DELETE_ARRAY(keyTimes);

    return animation;
}

Animation* AnimationTarget::createAnimation(const char* id, Properties* animationProperties)
{
    GP_ASSERT(animationProperties);
    if (std::strcmp(animationProperties->getNamespace(), "animation") != 0)
    {
        GP_ERROR("Invalid animation namespace '%s'.", animationProperties->getNamespace());
        return NULL;
    }

    const char* propertyIdStr = animationProperties->getString("property");
    if (propertyIdStr == NULL)
    {
        GP_ERROR("Attribute 'property' must be specified for an animation.");
        return NULL;
    }

    // Get animation target property id
	int propertyId = getPropertyId(_targetType, propertyIdStr);
    if (propertyId == -1)
    {
        GP_ERROR("Property ID is invalid.");
        return NULL;
    }

    unsigned int keyCount = animationProperties->getInt("keyCount");
    if (keyCount == 0)
    {
        GP_ERROR("Attribute 'keyCount' must be specified for an animation.");
        return NULL;
    }

    const char* keyTimesStr = animationProperties->getString("keyTimes");
    if (keyTimesStr == NULL)
    {
        GP_ERROR("Attribute 'keyTimes' must be specified for an animation.");
        return NULL;
    }

    const char* keyValuesStr = animationProperties->getString("keyValues");
    if (keyValuesStr == NULL)
    {
        GP_ERROR("Attribute 'keyValues' must be specified for an animation.");
        return NULL;
    }

    const char* curveStr = animationProperties->getString("curve");
    if (curveStr == NULL)
    {
        GP_ERROR("Attribute 'curve' must be specified for an animation.");
        return NULL;
    }

    char delimeter = ' ';
    size_t startOffset = 0;
    size_t endOffset = std::string::npos;

    unsigned int* keyTimes = new unsigned int[keyCount];
    for (size_t i = 0; i < keyCount; i++)
    {
        endOffset = static_cast<std::string>(keyTimesStr).find_first_of(delimeter, startOffset);
        if (endOffset != std::string::npos)
        {
            keyTimes[i] = std::strtoul(static_cast<std::string>(keyTimesStr).substr(startOffset, endOffset - startOffset).c_str(), NULL, 0);
        }
        else
        {
            keyTimes[i] = std::strtoul(static_cast<std::string>(keyTimesStr).substr(startOffset, static_cast<std::string>(keyTimesStr).length()).c_str(), NULL, 0);
        }
        startOffset = endOffset + 1;
    }

    startOffset = 0;
    endOffset = std::string::npos;

    int componentCount = getAnimationPropertyComponentCount(propertyId);
    GP_ASSERT(componentCount > 0);

    unsigned int components = keyCount * componentCount;

    float* keyValues = new float[components];
    for (unsigned int i = 0; i < components; i++)
    {
        endOffset = static_cast<std::string>(keyValuesStr).find_first_of(delimeter, startOffset);
        if (endOffset != std::string::npos)
        {
            keyValues[i] = std::atof(static_cast<std::string>(keyValuesStr).substr(startOffset, endOffset - startOffset).c_str());
        }
        else
        {
            keyValues[i] = std::atof(static_cast<std::string>(keyValuesStr).substr(startOffset, static_cast<std::string>(keyValuesStr).length()).c_str());
        }
        startOffset = endOffset + 1;
    }

    const char* keyInStr = animationProperties->getString("keyIn");
    float* keyIn = NULL;
    if (keyInStr)
    {
        keyIn = new float[components];
        startOffset = 0;
        endOffset = std::string::npos;
        for (unsigned int i = 0; i < components; i++)
        {
            endOffset = static_cast<std::string>(keyInStr).find_first_of(delimeter, startOffset);
            if (endOffset != std::string::npos)
            {
                keyIn[i] = std::atof(static_cast<std::string>(keyInStr).substr(startOffset, endOffset - startOffset).c_str());
            }
            else
            {
                keyIn[i] = std::atof(static_cast<std::string>(keyInStr).substr(startOffset, static_cast<std::string>(keyInStr).length()).c_str());
            }
            startOffset = endOffset + 1;
        }
    }

    const char* keyOutStr = animationProperties->getString("keyOut");
    float* keyOut = NULL;
    if (keyOutStr)
    {
        keyOut = new float[components];
        startOffset = 0;
        endOffset = std::string::npos;
        for (unsigned int i = 0; i < components; i++)
        {
            endOffset = static_cast<std::string>(keyOutStr).find_first_of(delimeter, startOffset);
            if (endOffset != std::string::npos)
            {
                keyOut[i] = std::atof(static_cast<std::string>(keyOutStr).substr(startOffset, endOffset - startOffset).c_str());
            }
            else
            {
                keyOut[i] = std::atof(static_cast<std::string>(keyOutStr).substr(startOffset, static_cast<std::string>(keyOutStr).length()).c_str());
            }
            startOffset = endOffset + 1;
        }
    }

    int curve = Curve::getInterpolationType(curveStr);

    Animation* animation = NULL;
    if (keyIn && keyOut)
    {
        animation = createAnimation(id, propertyId, keyCount, keyTimes, keyValues, keyIn, keyOut, (Curve::InterpolationType)curve);
    }
    else
    {
        animation = createAnimation(id, propertyId, keyCount, keyTimes, keyValues, (Curve::InterpolationType) curve);
    }

    const char* repeat = animationProperties->getString("repeatCount");
    if (repeat)
    {
        if (strcmp(repeat, ANIMATION_TARGET_INDEFINITE_STR) == 0)
        {
            animation->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
        }
        else
        {
            float value;
            sscanf(repeat, "%f", &value);
            animation->getClip()->setRepeatCount(value);
        }
    }
    
    SAFE_DELETE_ARRAY(keyOut);
    SAFE_DELETE_ARRAY(keyIn);
    SAFE_DELETE_ARRAY(keyValues);
    SAFE_DELETE_ARRAY(keyTimes);

    Properties* pClip = animationProperties->getNextNamespace();
    if (pClip && std::strcmp(pClip->getNamespace(), "clip") == 0)
    {
        int frameCount = animationProperties->getInt("frameCount");
        if (frameCount <= 0)
        {
            GP_ERROR("Frame count must be greater than zero for a clip.");
            return animation;
        }
        animation->createClips(animationProperties, (unsigned int) frameCount);
    }
    
    return animation;
}

void AnimationTarget::destroyAnimation(const char* id)
{
    // Find the animation with the specified ID.
    Animation::Channel* channel = getChannel(id);
    if (channel == NULL)
        return;

    // Remove this target's channel from animation, and from the target's list of channels.
    GP_ASSERT(channel->_animation);
    channel->_animation->removeChannel(channel);
    removeChannel(channel);

    SAFE_DELETE(channel);
}

Animation* AnimationTarget::getAnimation(const char* id) const
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();
        GP_ASSERT(*itr);

        if (id == NULL)
            return (*itr)->_animation;

        Animation::Channel* channel = NULL;
        for (; itr != _animationChannels->end(); itr++)
        {
            channel = (Animation::Channel*)(*itr);
            GP_ASSERT(channel);
            GP_ASSERT(channel->_animation);
            if (channel->_animation->_id.compare(id) == 0)
            {
                return channel->_animation;
            }
        }
    }

    return NULL;
}

int AnimationTarget::getPropertyId(TargetType type, const char* propertyIdStr)
{
    GP_ASSERT(propertyIdStr);

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

void AnimationTarget::addChannel(Animation::Channel* channel)
{
    if (_animationChannels == NULL)
        _animationChannels = new std::vector<Animation::Channel*>;

    GP_ASSERT(channel);
    _animationChannels->push_back(channel);
}

void AnimationTarget::removeChannel(Animation::Channel* channel)
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();
        for ( ; itr != _animationChannels->end(); itr++)
        {
            Animation::Channel* temp = *itr;
            if (channel == temp)
            {
                _animationChannels->erase(itr);

                if (_animationChannels->empty())
                    SAFE_DELETE(_animationChannels);

                return;
            }
        }
    }
}

Animation::Channel* AnimationTarget::getChannel(const char* id) const
{
    if (_animationChannels)
    {
        std::vector<Animation::Channel*>::iterator itr = _animationChannels->begin();

        if (id == NULL)
            return (*itr);

        Animation::Channel* channel = NULL;
        for (; itr != _animationChannels->end(); itr++)
        {
            channel = (Animation::Channel*)(*itr);
            GP_ASSERT(channel);
            if (channel->_animation->_id.compare(id) == 0)
            {
                return channel;
            }
        }
    }

    return NULL;
}

void AnimationTarget::cloneInto(AnimationTarget* target, NodeCloneContext &context) const
{
    if (_animationChannels)
    {
        for (std::vector<Animation::Channel*>::const_iterator it = _animationChannels->begin(); it != _animationChannels->end(); ++it)
        {
            Animation::Channel* channel = *it;
            GP_ASSERT(channel);
            GP_ASSERT(channel->_animation);

            Animation* animation = context.findClonedAnimation(channel->_animation);
            if (animation != NULL)
            {
                Animation::Channel* channelCopy = new Animation::Channel(*channel, animation, target);
                animation->addChannel(channelCopy);
            }
            else
            {
                // Clone the animation and register it with the context so that it only gets cloned once.
                animation = channel->_animation->clone(channel, target);
                context.registerClonedAnimation(channel->_animation, animation);
            }
        }
    }
}

void AnimationTarget::convertByValues(unsigned int propertyId, unsigned int componentCount, float* from, float* by)
{
    if (_targetType == AnimationTarget::TRANSFORM)
    {
        switch(propertyId)
        {
            case Transform::ANIMATE_SCALE:
            case Transform::ANIMATE_SCALE_UNIT:
            case Transform::ANIMATE_SCALE_X:
            case Transform::ANIMATE_SCALE_Y:
            case Transform::ANIMATE_SCALE_Z:
            {
                convertScaleByValues(from, by, componentCount);
                break;
            }
            case Transform::ANIMATE_TRANSLATE:
            case Transform::ANIMATE_TRANSLATE_X:
            case Transform::ANIMATE_TRANSLATE_Y:
            case Transform::ANIMATE_TRANSLATE_Z:
            {
                convertByValues(from, by, componentCount);
                break;
            }
            case Transform::ANIMATE_ROTATE:
            {
                convertQuaternionByValues(from, by);
                break;
            }
            case Transform::ANIMATE_ROTATE_TRANSLATE:
            {
                convertQuaternionByValues(from, by);
                convertByValues(from + 4, by + 4, 3);
                break;
            }
            case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
            {
                convertScaleByValues(from, by, 3);
                convertQuaternionByValues(from + 3, by + 3);
                convertByValues(from + 7, by + 7, 3);
                break;
            }
        }
    }
    else
    {
        convertByValues(from, by, componentCount);
    }
}

void AnimationTarget::convertQuaternionByValues(float* from, float* by)
{
    Quaternion qFrom(from);
    Quaternion qBy(by);
    qBy.multiply(qFrom);
    memcpy(by, (float*)&qBy, sizeof(float) * 4);
}

void AnimationTarget::convertScaleByValues(float* from, float* by, unsigned int componentCount)
{
    for (unsigned int i = 0; i < componentCount; i++)
    {
        by[i] *= from[i];
    }
}

void AnimationTarget::convertByValues(float* from, float* by, unsigned int componentCount)
{
    for (unsigned int i = 0; i < componentCount; i++)
    {
        by[i] += from[i];
    }
}

}



