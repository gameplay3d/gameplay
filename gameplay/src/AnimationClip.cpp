/*
 * AnimationClip.cpp
 */

#include "Base.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Game.h"

namespace gameplay
{

AnimationClip::AnimationClip(const char* id, Animation* animation, unsigned long startTime, unsigned long endTime)
    : _id(id), _animation(animation), _startTime(startTime), _endTime(endTime), _elapsedTime(0), _channelCount(animation->_channels.size()), _repeatCount(1.0f), _speed(1.0f), _isPlaying(false), _beginListeners(NULL), _endListeners(NULL)
{
    assert(0 <= startTime && startTime <= animation->_duration && 0 <= endTime && endTime <= animation->_duration);

    _duration = (_endTime - _startTime);
    _activeDuration = _duration * _repeatCount;
    
    for (unsigned int i = 0; i < _channelCount; i++)
    {
        _values.push_back(new AnimationValue(_animation->_channels[i]->_curve->getComponentCount()));
    }
}

AnimationClip::~AnimationClip()
{
    std::vector<AnimationValue*>::iterator valueIter = _values.begin();
    while (valueIter != _values.end())
    {
        SAFE_DELETE(*valueIter);
        valueIter++;
    }
    _values.clear();

    if (_beginListeners)
    {
        std::vector<Listener*>::iterator bIter = _beginListeners->begin();
        while (bIter != _beginListeners->end())
        {
            SAFE_DELETE(*bIter);
            bIter++;
        }
        _beginListeners->clear();
        SAFE_DELETE(_beginListeners);
    }

    if (_beginListeners)
    {
        std::vector<Listener*>::iterator eIter = _endListeners->begin();
        while (eIter != _endListeners->end())
        {
            SAFE_DELETE(*eIter);
            eIter++;
        }
        _endListeners->clear();
        SAFE_DELETE(_endListeners);
    }
}

const char* AnimationClip::getID() const
{
    return _id.c_str();
}

Animation* AnimationClip::getAnimation() const
{
    return _animation;
}

unsigned long AnimationClip::getStartTime() const
{
    return _startTime;
}

unsigned long AnimationClip::getEndTime() const
{
    return _endTime;
}

unsigned long AnimationClip::getElaspedTime() const
{
    return _elapsedTime;
}

void AnimationClip::setRepeatCount(float repeatCount)
{
    assert(repeatCount == ANIMATION_REPEAT_COUNT_INDEFINITE || repeatCount > 0.0f);

    _repeatCount = repeatCount;

    if (repeatCount == ANIMATION_REPEAT_COUNT_INDEFINITE)
    {
        _activeDuration = _duration;
    }
    else
    {
        _activeDuration = _repeatCount * _duration;
    }
}

float AnimationClip::getRepeatCount() const
{
    return _repeatCount;
}

void AnimationClip::setSpeed(float speed)
{
    _speed = speed;
}

float AnimationClip::getSpeed() const
{
    return _speed;
}

bool AnimationClip::isPlaying() const
{
    return _isPlaying;
}

void AnimationClip::play()
{
    _animation->_controller->schedule(this);
}

void AnimationClip::stop()
{
    _animation->_controller->unschedule(this);
    _isPlaying = false;
}

void AnimationClip::addBeginListener(AnimationClip::Listener* listener)
{
    if (!_beginListeners)
        _beginListeners = new std::vector<Listener*>;

    _beginListeners->push_back(listener);
}

void AnimationClip::addEndListener(AnimationClip::Listener* listener)
{
    if (!_endListeners)
        _endListeners = new std::vector<Listener*>;

    _endListeners->push_back(listener);
}

bool AnimationClip::update(unsigned long elapsedTime)
{
    if (!_isPlaying)
    {
        // Initialize animation to play.
        _isPlaying = true;
        _elapsedTime = 0;
        onBegin();
    }

    // Update elapsed time.
    if (_speed < 0.0f && _repeatCount != ANIMATION_REPEAT_COUNT_INDEFINITE)
        _elapsedTime -= elapsedTime * _speed;
    else
        _elapsedTime += elapsedTime * _speed;
    
    bool animationComplete = false;
    float percentComplete = 0.0f;

    // Check to see if clip is complete.
    if (_repeatCount != ANIMATION_REPEAT_COUNT_INDEFINITE && _elapsedTime >= _activeDuration)
    {
        percentComplete = _repeatCount;
        animationComplete = true;
        _isPlaying = false;
    }
    else
    {
        percentComplete = (float) _elapsedTime / _duration;
    }

    // Get out fractional part of percent complete (can ignore repeats!)
    // Add in start time, so we evaluate the correct part of the curve.
    float intPart;
    percentComplete = (std::min)((float)(_startTime + std::modf(percentComplete, &intPart) * _duration) / _animation->_duration, 1.0f);

    // Evaluate this clip.
    for (unsigned int i = 0; i < _channelCount; i++)
    {
        AnimationValue* value = _values.at(i);

        // Evaluate point on curve.
        _animation->_channels[i]->_curve->evaluate(percentComplete, value->_currentValue);
        
        // Set the animation value on the target property.
        _animation->_channels[i]->_target->setAnimationPropertyValue(_animation->_channels[i]->_propertyId, value);
    }

    if (animationComplete)
        onEnd();

    return animationComplete;
}

void AnimationClip::onBegin()
{
    if (_beginListeners)
    {
        std::vector<Listener*>::iterator listener = _beginListeners->begin();
        while (listener != _beginListeners->end())
        {
            (*listener)->animationEvent(this, Listener::BEGIN);
            listener++;
        }
    }
}

void AnimationClip::onEnd()
{
    if (_endListeners)
    {
        std::vector<Listener*>::iterator listener = _endListeners->begin();
        while (listener != _endListeners->end())
        {
            (*listener)->animationEvent(this, Listener::END);
            listener++;
        }
    }
}

}
