#include "Base.h"
#include "Animation.h"
#include "AnimationController.h"
#include "AnimationClip.h"
#include "AnimationTarget.h"
#include "Game.h"
#include "Transform.h"
#include "Properties.h"

#define ANIMATION_INDEFINITE_STR "INDEFINITE"
#define ANIMATION_DEFAULT_CLIP 0
#define ANIMATION_ROTATE_OFFSET 0
#define ANIMATION_SRT_OFFSET 3

namespace gameplay
{

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0L), _defaultClip(NULL), _clips(NULL)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, type);

    // Release the animation because a newly created animation has a ref count of 1 and the channels hold the ref to animation.
    release();
    GP_ASSERT(getRefCount() == 1);
}

Animation::Animation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0L), _defaultClip(NULL), _clips(NULL)
{
    createChannel(target, propertyId, keyCount, keyTimes, keyValues, keyInValue, keyOutValue, type);
    // Release the animation because a newly created animation has a ref count of 1 and the channels hold the ref to animation.
    release();
    GP_ASSERT(getRefCount() == 1);
}

Animation::Animation(const char* id)
    : _controller(Game::getInstance()->getAnimationController()), _id(id), _duration(0L), _defaultClip(NULL), _clips(NULL)
{
}

Animation::~Animation()
{
    _channels.clear();

    if (_defaultClip)
    {
        if (_defaultClip->isClipStateBitSet(AnimationClip::CLIP_IS_PLAYING_BIT))
        {
            GP_ASSERT(_controller);
            _controller->unschedule(_defaultClip);
        }
        SAFE_RELEASE(_defaultClip);
    }

    if (_clips)
    {
        std::vector<AnimationClip*>::iterator clipIter = _clips->begin();

        while (clipIter != _clips->end())
        {
            AnimationClip* clip = *clipIter;
            GP_ASSERT(clip);
            if (clip->isClipStateBitSet(AnimationClip::CLIP_IS_PLAYING_BIT))
            {
                GP_ASSERT(_controller);
                _controller->unschedule(clip);
            }
            SAFE_RELEASE(clip);
            clipIter++;
        }
        _clips->clear();
    }
    SAFE_DELETE(_clips);
}

Animation::Channel::Channel(Animation* animation, AnimationTarget* target, int propertyId, Curve* curve, unsigned long duration)
    : _animation(animation), _target(target), _propertyId(propertyId), _curve(curve), _duration(duration)
{
    GP_ASSERT(_animation);
    GP_ASSERT(_target);
    GP_ASSERT(_curve);

    // get property component count, and ensure the property exists on the AnimationTarget by getting the property component count.
    GP_ASSERT(_target->getAnimationPropertyComponentCount(propertyId));
    _curve->addRef();
    _target->addChannel(this);
    _animation->addRef();
}

Animation::Channel::Channel(const Channel& copy, Animation* animation, AnimationTarget* target)
    : _animation(animation), _target(target), _propertyId(copy._propertyId), _curve(copy._curve), _duration(copy._duration)
{
    GP_ASSERT(_curve);
    GP_ASSERT(_target);
    GP_ASSERT(_animation);

    _curve->addRef();
    _target->addChannel(this);
    _animation->addRef();
}

Animation::Channel::~Channel()
{
    SAFE_RELEASE(_curve);
    SAFE_RELEASE(_animation);
}

Curve* Animation::Channel::getCurve() const
{
    return _curve;
}

const char* Animation::getId() const
{
    return _id.c_str();
}

unsigned long Animation::getDuration() const
{
    return _duration;
}

void Animation::createClips(const char* url)
{
    Properties* properties = Properties::create(url);
    GP_ASSERT(properties);

    Properties* pAnimation = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    GP_ASSERT(pAnimation);

    int frameCount = pAnimation->getInt("frameCount");
    if (frameCount <= 0)
        GP_ERROR("The animation's frame count must be greater than 0.");

    createClips(pAnimation, (unsigned int)frameCount);

    SAFE_DELETE(properties);
}

AnimationClip* Animation::createClip(const char* id, unsigned long start, unsigned long end)
{
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

AnimationClip* Animation::getClip(unsigned int index) const
{
    if (_clips)
        return _clips->at(index);

    return NULL;
}

unsigned int Animation::getClipCount() const
{
    return _clips ? (unsigned int)_clips->size() : 0;
}

void Animation::play(const char* clipId)
{
    // If id is NULL, play the default clip.
    if (clipId == NULL)
    {
        if (_defaultClip == NULL)
            createDefaultClip();

        _defaultClip->play();
    }
    else
    {
        // Find animation clip and play.
        AnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->play();
    }
}

void Animation::stop(const char* clipId)
{
    // If id is NULL, play the default clip.
    if (clipId == NULL)
    {
        if (_defaultClip)
            _defaultClip->stop();
    }
    else
    {
        // Find animation clip and play.
        AnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->stop();
    }
}

void Animation::pause(const char * clipId)
{
    if (clipId == NULL)
    {
        if (_defaultClip)
            _defaultClip->pause();
    }
    else
    {
        AnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->pause();
    }
}

bool Animation::targets(AnimationTarget* target) const
{
    for (std::vector<Animation::Channel*>::const_iterator itr = _channels.begin(); itr != _channels.end(); ++itr)
    {
        GP_ASSERT(*itr);
        if ((*itr)->_target == target)
        {
            return true;
        }
    }
    return false;
}

void Animation::createDefaultClip()
{
    _defaultClip = new AnimationClip("default_clip", this, 0.0f, _duration);
}

void Animation::createClips(Properties* animationProperties, unsigned int frameCount)
{
    GP_ASSERT(animationProperties);

    Properties* pClip = animationProperties->getNextNamespace();

    while (pClip != NULL && std::strcmp(pClip->getNamespace(), "clip") == 0)
    {
        int begin = pClip->getInt("begin");
        int end = pClip->getInt("end");

        AnimationClip* clip = createClip(pClip->getId(), ((float)begin / (frameCount-1)) * _duration, ((float)end / (frameCount-1)) * _duration);

        const char* repeat = pClip->getString("repeatCount");
        if (repeat)
        {
            if (strcmp(repeat, ANIMATION_INDEFINITE_STR) == 0)
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

        clip->setLoopBlendTime(pClip->getFloat("loopBlendTime")); // returns zero if not specified

        pClip = animationProperties->getNextNamespace();
    }
}

void Animation::addClip(AnimationClip* clip)
{
    if (_clips == NULL)
        _clips = new std::vector<AnimationClip*>;

    GP_ASSERT(clip);
    _clips->push_back(clip);
}

AnimationClip* Animation::findClip(const char* id) const
{
    if (_clips)
    {
        AnimationClip* clip = NULL;
        size_t clipCount = _clips->size();
        for (size_t i = 0; i < clipCount; i++)
        {
            clip = _clips->at(i);
            GP_ASSERT(clip);
            if (clip->_id.compare(id) == 0)
            {
                return clip;
            }
        }
    }
    return NULL;
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, unsigned int type)
{
    GP_ASSERT(target);
    GP_ASSERT(keyTimes);
    GP_ASSERT(keyValues);

    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    GP_ASSERT(propertyComponentCount > 0);

    Curve* curve = Curve::create(keyCount, propertyComponentCount);
    GP_ASSERT(curve);
    if (target->_targetType == AnimationTarget::TRANSFORM)
        setTransformRotationOffset(curve, propertyId);

    unsigned int lowest = keyTimes[0];
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
    if (keyCount > 1) {
        i = keyCount - 1;
        normalizedKeyTimes[i] = 1.0f;
        curve->setPoint(i, normalizedKeyTimes[i], keyValues + pointOffset, (Curve::InterpolationType) type);
    }

    SAFE_DELETE_ARRAY(normalizedKeyTimes);

    Channel* channel = new Channel(this, target, propertyId, curve, duration);
    curve->release();
    addChannel(channel);
    return channel;
}

Animation::Channel* Animation::createChannel(AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned int* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, unsigned int type)
{
    GP_ASSERT(target);
    GP_ASSERT(keyTimes);
    GP_ASSERT(keyValues);

    unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    GP_ASSERT(propertyComponentCount > 0);

    Curve* curve = Curve::create(keyCount, propertyComponentCount);
    GP_ASSERT(curve);
    if (target->_targetType == AnimationTarget::TRANSFORM)
        setTransformRotationOffset(curve, propertyId);

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

    SAFE_DELETE_ARRAY(normalizedKeyTimes);

    Channel* channel = new Channel(this, target, propertyId, curve, duration);
    curve->release();
    addChannel(channel);
    return channel;
}

void Animation::addChannel(Channel* channel)
{
    GP_ASSERT(channel);
    _channels.push_back(channel);

    if (channel->_duration > _duration)
        _duration = channel->_duration;
}

void Animation::removeChannel(Channel* channel)
{
    std::vector<Animation::Channel*>::iterator itr = _channels.begin();
    while (itr != _channels.end())
    {
        Animation::Channel* chan = *itr;
        if (channel == chan)
        {
            _channels.erase(itr);
            return;
        }
        else
        {
            itr++;
        }
    }
}

void Animation::setTransformRotationOffset(Curve* curve, unsigned int propertyId)
{
    GP_ASSERT(curve);

    switch (propertyId)
    {
    case Transform::ANIMATE_ROTATE:
    case Transform::ANIMATE_ROTATE_TRANSLATE:
        curve->setQuaternionOffset(ANIMATION_ROTATE_OFFSET);
        return;
    case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
        curve->setQuaternionOffset(ANIMATION_SRT_OFFSET);
        return;
    }

    return;
}

Animation* Animation::clone(Channel* channel, AnimationTarget* target)
{
    GP_ASSERT(channel);

    Animation* animation = new Animation(getId());

    Animation::Channel* channelCopy = new Animation::Channel(*channel, animation, target);
    animation->addChannel(channelCopy);
    // Release the animation because a newly created animation has a ref count of 1 and the channels hold the ref to animation.
    animation->release();
    GP_ASSERT(animation->getRefCount() == 1);

    // Clone the clips
    
    if (_defaultClip)
    {
        animation->_defaultClip = _defaultClip->clone(animation);
    }
    
    if (_clips)
    {
        for (std::vector<AnimationClip*>::iterator it = _clips->begin(); it != _clips->end(); ++it)
        {
            AnimationClip* newClip = (*it)->clone(animation);
            animation->addClip(newClip);
        }
    }
    return animation;
}

}
