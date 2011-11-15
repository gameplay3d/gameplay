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
    : _id(id), _animation(animation), _startTime(startTime), _endTime(endTime), _elapsedTime(0), _runningTime(0), _channelCount(animation->_channels.size()), _repeatCount(1.0f), _speed(1.0f), _isPlaying(false), _beginListeners(NULL), _endListeners(NULL)
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
    // Explicitly stop this clip if it's currently playing so it gets removed from the controller
    if (_isPlaying)
    {
        stop();
    }

    std::vector<AnimationValue*>::iterator valueIter = _values.begin();
    while (valueIter != _values.end())
    {
        SAFE_DELETE(*valueIter);
        valueIter++;
    }

    SAFE_DELETE(_beginListeners);
    SAFE_DELETE(_endListeners);
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
    assert(repeatCount == REPEAT_INDEFINITE || repeatCount > 0.0f);

    _repeatCount = repeatCount;

    if (repeatCount == REPEAT_INDEFINITE)
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

void AnimationClip::setActiveDuration(unsigned long duration)
{
    if (duration == REPEAT_INDEFINITE)
    {
        _repeatCount = REPEAT_INDEFINITE;
        _activeDuration = _duration;
    }
    else
    {
        _activeDuration = _duration;
        _repeatCount = (float) _activeDuration / (float) _duration;
    }
}

unsigned long AnimationClip::getActiveDuration() const
{
    if (_repeatCount == REPEAT_INDEFINITE)
        return REPEAT_INDEFINITE;

    return _activeDuration;
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
        onBegin();

    // Update elapsed time.
    _elapsedTime += elapsedTime;
    _runningTime += elapsedTime * _speed;

    float percentComplete = 0.0f;

    // Check to see if clip is complete.
    if (_repeatCount != REPEAT_INDEFINITE && ((_speed >= 0 && _runningTime >= (long) _activeDuration) || (_speed < 0 && _runningTime <= 0)))
    {
        _isPlaying = false;
        if (_speed >= 0)
            percentComplete = _activeDuration % _duration; // Get's the fractional part of the final repeat.
        else
            percentComplete = 0.0f; // If we are negative speed, the end value should be 0.
    }
    else
    {
        // Gets portion/fraction of the repeat.
        percentComplete = (float) (_runningTime % _duration);
    }

    // Add back in start time, and divide by the total animation's duration to get the actual percentage complete
    percentComplete = (float)(_startTime + percentComplete) / (float) _animation->_duration;

    // Evaluate this clip.
    Animation::Channel* channel = NULL;
    AnimationValue* value = NULL;
    for (unsigned int i = 0; i < _channelCount; i++)
    {
        channel = _animation->_channels[i];
        value = _values.at(i);

        // Evaluate point on curve.
        channel->_curve->evaluate(percentComplete, value->_currentValue);
        
        // Set the animation value on the target property.
        channel->_target->setAnimationPropertyValue(channel->_propertyId, value);
    }

    // When ended. Probably should move to it's own method so we can call it when the clip is ended early.
    if (!_isPlaying)
    {
        onEnd();
    }

    return !_isPlaying;
}

void AnimationClip::onBegin()
{
    // Initialize animation to play.
    _isPlaying = true;
    _elapsedTime = 0;

    if (_speed > 0)
    {
        _runningTime = 0;
    }
    else
    {
        _runningTime = _activeDuration;
    }

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
