/*
 * Animation.cpp
 */

#include "Base.h"
#include "Animation.h"
#include "AnimationController.h"
#include "AnimationClip.h"
#include "AnimationTarget.h"
#include "Game.h"
#include "Transform.h"
#include <string.h>
#include "Properties.h"

#define ANIMATION_DEFAULT_CLIP_SUFFIX "__default__clip"
#define ANIMATION_INDEFINITE_STR "INDEFINITE"
#define ANIMATION_DEFAULT_CLIP 0
#define ANIMATION_ROTATE_OFFSET 0
#define ANIMATION_SRT_OFFSET 3

namespace gameplay
{

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0), _defaultClip(NULL), _clips(NULL)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, type);
}

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0), _defaultClip(NULL), _clips(NULL)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, keyInValue, keyOutValue, type);
}

Animation::~Animation()
{
    std::vector<Channel*>::iterator channelIter = _channels.begin();
    
    while (channelIter != _channels.end())
    {
        SAFE_DELETE(*channelIter);
        channelIter++;
    }
    _channels.clear();
    
    if (_clips != NULL)
    {
        std::vector<AnimationClip*>::iterator clipIter = _clips->begin();
    
        while (clipIter != _clips->end())
        {   
            AnimationClip* clip = *clipIter;
            SAFE_RELEASE(clip);
            clipIter++;
        }
        _clips->clear();
    }
}

Animation::Channel::Channel(AnimationTarget* target, int propertyId, Curve* curve, unsigned long duration)
    : _isRelative(false)
{
    // get property component count, and ensure the property exists on the AnimationTarget by getting the property component count.
    assert(target->getAnimationPropertyComponentCount(propertyId));

    _target = target;
    _target->addRef();
    _propertyId = propertyId;
    _curve = curve;
    _duration = duration;
}

Animation::Channel::~Channel()
{
    SAFE_RELEASE(_target);
    SAFE_DELETE(_curve);
}

const char* Animation::getId() const
{
    return _id.c_str();
}

unsigned long Animation::getDuration() const
{
    return _duration;
}

void Animation::createClips(const char* animationFile)
{
    assert(animationFile);

    Properties* pAnim = Properties::create(animationFile);
    assert(pAnim);

    Properties* animation = pAnim->getNextNamespace();
    int frameCount = animation->getInt("frameCount");

    const Properties* pClip = animation->getNextNamespace();
    while (pClip != NULL)
    {
        int begin = pClip->getInt("begin");
        int end = pClip->getInt("end");

        AnimationClip* clip = createClip(pClip->getId(), ((float) begin / frameCount) * _duration, ((float) end / frameCount) * _duration);

        const char* repeat = pClip->getString("repeatCount");
        if (repeat)
        {
            if (((std::string)repeat).compare(ANIMATION_INDEFINITE_STR) == 0)
            {
                clip->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
            }
            else
            {
                float value;
                sscanf(repeat, "%f", &value);
                clip->setRepeatCount(value);
            }
        }

        const char* speed = pClip->getString("speed");
        if (speed)
        {
            float value;
            sscanf(speed, "%f", &value);
            clip->setSpeed(value);
        }

        pClip = animation->getNextNamespace();
    }

    SAFE_DELETE(pAnim);
}

AnimationClip* Animation::createClip(const char* id, unsigned long start, unsigned long end)
{
    if (_clips != NULL && findClip(id) != NULL)
    {
        return NULL;
    }
    
    AnimationClip* clip = new AnimationClip(id, this, start, end);

    addClip(clip);

    return clip;
}

AnimationClip* Animation::getClip(const char* id)
{
    // If id is NULL return the default clip.
    if (id == NULL)
    {
        if (_defaultClip == NULL)
            createDefaultClip();

        return _defaultClip;
    }
    else
    {
        return findClip(id);
    }
}

void Animation::play(const char* id)
{
    // If id is NULL, play the default clip.
    if (id == NULL)
    {
        if (_defaultClip == NULL)
            createDefaultClip();
        
        _defaultClip->play();
    }
    else
    {
        // Find animation clip.. and play.
        AnimationClip* clip = findClip(id);
        if (clip != NULL)
        {
            clip->play();
        }
    }
}

void Animation::stop(const char* id)
{
    // If id is NULL, play the default clip.
    if (id == NULL)
    {
        if (_defaultClip == NULL)
            createDefaultClip();

        _defaultClip->stop();
    }
    else
    {
        // Find animation clip.. and play.
        AnimationClip* clip = findClip(id);
        if (clip != NULL)
        {
            clip->stop();
        }
    }
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, unsigned int type)
{
    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    assert(propertyComponentCount > 0);

    Curve* curve = new Curve(keyCount, propertyComponentCount);
    if (target->_targetType == AnimationTarget::TRANSFORM)
    {
        switch (propertyId)
        {
        case Transform::ANIMATE_ROTATE:
        case Transform::ANIMATE_ROTATE_TRANSLATE:
            curve->addQuaternionOffset(ANIMATION_ROTATE_OFFSET);
            break;
        case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
            curve->addQuaternionOffset(ANIMATION_SRT_OFFSET);
            break;
        }
    }

    unsigned long lowest = keyTimes[0];
    unsigned long duration = keyTimes[keyCount-1] - lowest;

    float* normalizedKeyTimes = new float[keyCount];

    normalizedKeyTimes[0] = 0.0f;
    curve->setPoint(0, normalizedKeyTimes[0], keyValues, (Curve::InterpolationType) type);

    unsigned int pointOffset = propertyComponentCount;
    unsigned int i = 1;
    for (; i < keyCount - 1; i++)
    {
        normalizedKeyTimes[i] = (float) (keyTimes[i] - lowest) / (float) duration;
        curve->setPoint(i, normalizedKeyTimes[i], (keyValues + pointOffset), (Curve::InterpolationType) type);
        pointOffset += propertyComponentCount;
    }
    i = keyCount - 1;
    normalizedKeyTimes[i] = 1.0f;
    curve->setPoint(i, normalizedKeyTimes[i], keyValues + pointOffset, (Curve::InterpolationType) type);

    SAFE_DELETE(normalizedKeyTimes);

    Channel* channel = new Channel(target, propertyId, curve, duration);

    addChannel(channel);

    return channel;
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type)
{
    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    assert(propertyComponentCount > 0);

    Curve* curve = new Curve(keyCount, propertyComponentCount);
    if (target->_targetType == AnimationTarget::TRANSFORM)
    {
        switch (propertyId)
        {
        case Transform::ANIMATE_ROTATE:
        case Transform::ANIMATE_ROTATE_TRANSLATE:
            curve->addQuaternionOffset(ANIMATION_ROTATE_OFFSET);
            break;
        case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
            curve->addQuaternionOffset(ANIMATION_SRT_OFFSET);
            break;
        }
    }
    
    unsigned long lowest = keyTimes[0];
    unsigned long duration = keyTimes[keyCount-1] - lowest;

    float* normalizedKeyTimes = new float[keyCount];
    
    normalizedKeyTimes[0] = 0.0f;
    curve->setPoint(0, normalizedKeyTimes[0], keyValues, (Curve::InterpolationType) type, keyInValue, keyOutValue);

    unsigned int pointOffset = propertyComponentCount;
    unsigned int i = 1;
    for (; i < keyCount - 1; i++)
    {
        normalizedKeyTimes[i] = (float) (keyTimes[i] - lowest) / (float) duration;
        curve->setPoint(i, normalizedKeyTimes[i], (keyValues + pointOffset), (Curve::InterpolationType) type, (keyInValue + pointOffset), (keyOutValue + pointOffset));
        pointOffset += propertyComponentCount;
    }
    i = keyCount - 1;
    normalizedKeyTimes[i] = 1.0f;
    curve->setPoint(i, normalizedKeyTimes[i], keyValues + pointOffset, (Curve::InterpolationType) type, keyInValue + pointOffset, keyOutValue + pointOffset);

    SAFE_DELETE(normalizedKeyTimes);

    Channel* channel = new Channel(target, propertyId, curve, duration);

    addChannel(channel);

    return channel;
}

void Animation::addChannel(Channel* channel)
{
    _channels.push_back(channel);
    
    if (channel->_duration > _duration)
        _duration = channel->_duration;
}

void Animation::createDefaultClip()
{
    std::string clipId = _id + ANIMATION_DEFAULT_CLIP_SUFFIX;

    _defaultClip = new AnimationClip(clipId.c_str(), this, 0.0f, _duration);
}

void Animation::addClip(AnimationClip* clip)
{
    if (_clips == NULL)
        _clips = new std::vector<AnimationClip*>;

    _clips->push_back(clip);
}

AnimationClip* Animation::findClip(const char* id) const
{
    unsigned int clipCount = _clips->size();
    for (unsigned int i = 0; i < clipCount; i++)
    {
        if (_clips->at(i)->_id.compare(id) == 0)
        {
            return _clips->at(i);
        }
    }
    return NULL;
}

}
