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

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, float* keyTimes, float* keyValues, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, type);
    createDefaultClip();
}

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, float* keyTimes, float* keyValues, float* keyTangentIn, float* keyTangentOut, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, keyTangentIn, keyTangentOut, type);
    createDefaultClip();
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
    
    std::vector<AnimationClip*>::iterator clipIter = _clips.begin();
    
    while (clipIter != _clips.end())
    {
        AnimationClip* clip = *clipIter;
        SAFE_RELEASE(clip);
        clipIter++;
    }
    _clips.clear();
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

    const vector<Properties*>* vAnim = pAnim->getProperties();
    
    Properties* animation = vAnim->front();
    int frameCount = animation->getInt("frameCount");

    const std::vector<Properties*>* clips = animation->getProperties();
    vector<Properties*>::const_iterator itr;

    for (itr = clips->begin(); itr < clips->end(); itr++)
    {
        Properties* pClip = *itr;

        int begin = pClip->getInt("begin");
        int end = pClip->getInt("end");

        AnimationClip* clip = createClip(pClip->getID(), ((float) begin / frameCount) * _duration, ((float) end / frameCount) * _duration);

        const char* repeat = pClip->getString("repeatCount");
        if (repeat)
        {
            if (((std::string)repeat).compare(ANIMATION_INDEFINITE_STR) == 0)
            {
                clip->setRepeatCount(ANIMATION_REPEAT_COUNT_INDEFINITE);
            }
            else
            {
                float value;
                sscanf(repeat, "%f", &value);
                clip->setRepeatCount(value);
            }
        }
    }

    SAFE_DELETE(pAnim);
}

AnimationClip* Animation::createClip(const char* id, unsigned long start, unsigned long end)
{
    if (findClip(id) != NULL)
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
        return _clips[ANIMATION_DEFAULT_CLIP];
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
        _clips[ANIMATION_DEFAULT_CLIP]->play();
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
        _clips[ANIMATION_DEFAULT_CLIP]->stop();
    }
    else
    {
        // Find animation clip.. and play.
        AnimationClip* clip = findClip(id);
        if (clip != NULL)
            clip->stop();
    }
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, float* keyTimes, float* keyValues, unsigned int type)
{
    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    assert(propertyComponentCount > 0);

    Curve* curve = new Curve(keyCount, propertyComponentCount);
    if (target->_targetType == AnimationTarget::TRANSFORM)
    {
        switch (propertyId)
        {
            case TRANSFORM_ANIMATE_ROTATE: 
            case TRANSFORM_ANIMATE_ROTATE_TRANSLATE:
                curve->_rotationOffset = ANIMATION_ROTATE_OFFSET;
                break;
            case TRANSFORM_ANIMATE_SCALE_ROTATE_TRANSLATE:
                curve->_rotationOffset = ANIMATION_SRT_OFFSET;
                break;
        }
    }

    float lowest = keyTimes[0];
    float duration = keyTimes[keyCount-1] - lowest;

    unsigned int pointOffset = 0;
    for (unsigned int i = 0; i < keyCount; i++)
    {
        pointOffset = i * propertyComponentCount;
        keyTimes[i] = (keyTimes[i] - lowest) / duration;
        curve->setPoint(i, keyTimes[i], keyValues + pointOffset, (Curve::InterpolationType) type);
    }

    Channel* channel = new Channel(target, propertyId, curve, duration);

    addChannel(channel);

    return channel;
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, float* keyTimes, float* keyValues, float* keyTangentIn, float* keyTangentOut, unsigned int type)
{
    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    assert(propertyComponentCount > 0);

    Curve* curve = new Curve(keyCount, propertyComponentCount);
    if (target->_targetType == AnimationTarget::TRANSFORM)
    {
        switch (propertyId)
        {
            case TRANSFORM_ANIMATE_ROTATE: 
            case TRANSFORM_ANIMATE_ROTATE_TRANSLATE:
                curve->_rotationOffset = ANIMATION_ROTATE_OFFSET;
                break;
            case TRANSFORM_ANIMATE_SCALE_ROTATE_TRANSLATE:
                curve->_rotationOffset = ANIMATION_SRT_OFFSET;
                break;
        }
    }

    float lowest = keyTimes[0];
    float duration = keyTimes[keyCount-1] - lowest;

    unsigned int pointOffset = 0;
    for (unsigned int i = 0; i < keyCount; i++)
    {
        pointOffset = i * propertyComponentCount;
        keyTimes[i] = (keyTimes[i] - lowest) / duration;
        curve->setPoint(i, keyTimes[i], keyValues + pointOffset, (Curve::InterpolationType) type, keyTangentIn + pointOffset, keyTangentOut + pointOffset);
    }

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
    _clips.push_back(new AnimationClip(clipId.c_str(), this, 0.0f, _duration));
}

void Animation::addClip(AnimationClip* clip)
{
    _clips.push_back(clip);
}

AnimationClip* Animation::findClip(const char* id) const
{
    unsigned int clipCount = _clips.size();
    for (unsigned int i = 0; i < clipCount; i++)
    {
        if (_clips.at(i)->_id.compare(id) == 0)
            return _clips.at(i);
    }
    return NULL;
}

void Animation::transfer(Animation* animation)
{
    // WARNING: This function is leaking some memory, but it should be removed when this hack is removed!

    unsigned int count = animation->_channels.size();
    for (unsigned int i = 0; i < count; i++)
    {
        Channel* channel = animation->_channels[i];
        addChannel(channel);
    }

    // AnimationChannel is not reference counted so they should not belong to two Animations.
    // Delete the AnimationChannel array and all of the clips.
    /*std::vector<Channel*>::iterator channelIter = animation->_channels.begin();
    
    while (channelIter != animation->_channels.end())
    {
        Channel* channel = *channelIter;
        channelIter = animation->_channels.erase(channelIter);
        SAFE_DELETE(channel);
    }
    */
    /*std::vector<AnimationClip*>::iterator clipIter = animation->_clips.begin();
    
    while (clipIter != animation->_clips.end())
    {
        AnimationClip* clip = *clipIter;
        clipIter = animation->_clips.erase(clipIter);
        SAFE_RELEASE(clip);
    }*/
    /*
    animation->_channelCount = 0;
    SAFE_DELETE_ARRAY(animation->_channels);
    for (unsigned int i = 0; i < animation->_channelCount; i++)
    {
        SAFE_RELEASE(animation->_clips[i]);
    }
    SAFE_DELETE_ARRAY(animation->_clips);*/
}

}
