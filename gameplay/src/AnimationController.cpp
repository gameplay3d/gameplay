/*
 * AnimationController.cpp
 */

#include "Base.h"
#include "AnimationController.h"
#include "Game.h"
#include "Curve.h"

namespace gameplay
{

AnimationController::AnimationController()
    : _state(IDLE), _animations(NULL)
{
}

AnimationController::~AnimationController()
{
    destroyAllAnimations();
}

Animation* AnimationController::createAnimation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, Curve::InterpolationType type)
{
    assert(type != Curve::BEZIER && type != Curve::HERMITE);
    assert(id && keyCount >= 2 && keyTimes && keyValues);
    Animation* animation = getAnimation(id);

    if (animation != NULL)
        return NULL;

    animation = new Animation(id, target, propertyId, keyCount, keyTimes, keyValues, type);

    addAnimation(animation);

    target->addAnimation(animation);

    return animation;
}

Animation* AnimationController::createAnimation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, Curve::InterpolationType type)
{
    assert(id && keyCount >= 2 && keyTimes && keyValues && keyInValue && keyOutValue);
    Animation* animation = getAnimation(id);

    if (animation != NULL)
        return NULL;
    
    animation = new Animation(id, target, propertyId, keyCount, keyTimes, keyValues, keyInValue, keyOutValue, type);

    addAnimation(animation);

    target->addAnimation(animation);

    return animation;
}

Animation* AnimationController::createAnimationFromTo(const char* id, AnimationTarget* target, int propertyId, float* from, float* to, Curve::InterpolationType type, unsigned long duration)
{
    const unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    float* keyValues = new float[2 * propertyComponentCount];

    memcpy(keyValues, from, sizeof(float) * propertyComponentCount);
    memcpy(keyValues + propertyComponentCount, to, sizeof(float) * propertyComponentCount);

    unsigned long* keyTimes = new unsigned long[2];
    keyTimes[0] = 0;
    keyTimes[1] = duration;

    Animation* animation = createAnimation(id, target, propertyId, 2, keyTimes, keyValues, type);

    SAFE_DELETE_ARRAY(keyTimes);
    
    return animation;
}

Animation* AnimationController::createAnimationFromBy(const char* id, AnimationTarget* target, int propertyId, float* from, float* by, Curve::InterpolationType type, unsigned long duration)
{
    const unsigned int propertyComponentCount = target->getAnimationPropertyComponentCount(propertyId);
    float* keyValues = new float[2 * propertyComponentCount];

    memcpy(keyValues, from, sizeof(float) * propertyComponentCount);
    memcpy(keyValues + propertyComponentCount, by, sizeof(float) * propertyComponentCount);

    unsigned long* keyTimes = new unsigned long[2];
    keyTimes[0] = 0;
    keyTimes[1] = duration;

    Animation* animation = createAnimation(id, target, propertyId, 2, keyTimes, keyValues, type);

    SAFE_DELETE_ARRAY(keyTimes);

    return animation;
}

Animation* AnimationController::getAnimation(const char* id) const
{
    unsigned int animationCount = _animations.size();
    for (unsigned int i = 0; i < animationCount; i++)
    {
        if (_animations.at(i)->_id.compare(id) == 0)
        {
            return _animations.at(i);
        }
    }
    return NULL;
}

void AnimationController::stopAllAnimations() 
{
    std::list<AnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        AnimationClip* clip = *clipIter;
        clip->_isPlaying = false;
        clip->onEnd();
        clipIter = _runningClips.erase(clipIter);
        SAFE_RELEASE(clip);
    }
    _runningClips.clear();

    _state = IDLE;
}

AnimationController::State AnimationController::getState() const
{
    return _state;
}

void AnimationController::initialize()
{
    _state = IDLE;
}

void AnimationController::finalize()
{
    _state = PAUSED;
}

void AnimationController::resume()
{
    if (_runningClips.empty())
        _state = IDLE;
    else
        _state = RUNNING;
}

void AnimationController::pause()
{
    _state = PAUSED;
}

void AnimationController::schedule(AnimationClip* clip)
{
    if (_runningClips.empty())
    {
        _state = RUNNING;
    }
    
    if (clip->_isPlaying)
    {
        _runningClips.remove(clip);
        clip->_isPlaying = false;
        clip->onEnd();
    }
    else
    {
        clip->addRef();
    }

    _runningClips.push_back(clip);
}

void AnimationController::unschedule(AnimationClip* clip)
{
    if (clip->_isPlaying)
    {
        _runningClips.remove(clip);
        SAFE_RELEASE(clip);
    }

    if (_runningClips.empty())
        _state = IDLE;
}

void AnimationController::update(long elapsedTime)
{
    if (_state != RUNNING)
        return;

    std::list<AnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        AnimationClip* clip = (*clipIter);
        if (clip->update(elapsedTime))
        {
            clipIter = _runningClips.erase(clipIter);
            SAFE_RELEASE(clip);
        }
        else
        {
            clipIter++;
        }
    }
    
    if (_runningClips.empty())
        _state = IDLE;
}

void AnimationController::addAnimation(Animation* animation)
{
    _animations.push_back(animation);
}

void AnimationController::destroyAnimation(Animation* animation)
{
    std::vector<Animation*>::iterator itr = _animations.begin();

    while (itr != _animations.end())
    {
        if (animation == *itr)
        {
            _animations.erase(itr);
            return;
        }
        itr++;
    }
}

void AnimationController::destroyAllAnimations()
{
    stopAllAnimations();

    std::vector<Animation*>::iterator itr = _animations.begin();
    
    while (itr != _animations.end())
    {
        Animation* animation = *itr;
        SAFE_RELEASE(animation);
        itr++;
    }

    _animations.clear();
}

}
