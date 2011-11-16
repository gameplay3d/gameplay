/*
 * AnimationClip.cpp
 */

#include "Base.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Game.h"
#include "Quaternion.h"

namespace gameplay
{

AnimationClip::AnimationClip(const char* id, Animation* animation, unsigned long startTime, unsigned long endTime)
    : _id(id), _animation(animation), _startTime(startTime), _endTime(endTime), _duration(_endTime - _startTime), _repeatCount(1.0f), _activeDuration(_duration * _repeatCount), _speed(1.0f), _isPlaying(false), 
      _timeStarted(0), _elapsedTime(0), _runningTime(0), _channelPriority(NULL), _crossFadeClip(NULL), _crossFadeFrom(NULL), _crossFadeStart(0), _crossFadeInDuration(0), _crossFadeOutDuration(0), _blendWeight(1.0f), _isFadingOut(false), _isFadingIn(false),  
      _beginListeners(NULL), _endListeners(NULL)
{
    assert(0 <= startTime && startTime <= animation->_duration && 0 <= endTime && endTime <= animation->_duration);
    
    unsigned int channelCount = _animation->_channels.size();
    _channelPriority = new unsigned int[channelCount];
    
    for (unsigned int i = 0; i < channelCount; i++)
    {
        _values.push_back(new AnimationValue(_animation->_channels[i]->_curve->getComponentCount()));
        _channelPriority[i] = 0;
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
    _timeStarted = Game::getGameTime();
}

void AnimationClip::stop()
{
    _animation->_controller->unschedule(this);
    if (_isPlaying)
    {
        _isPlaying = false;
        onEnd();
    }
}

void AnimationClip::crossFade(AnimationClip* clip, unsigned long duration)
{
    if (_crossFadeClip)
        _crossFadeClip->release();

    _crossFadeClip = clip;
    clip->addRef();

    _crossFadeClip->_isFadingOut = false;
    _crossFadeClip->_isFadingIn = true;
    _crossFadeClip->_crossFadeInDuration = duration;
    _crossFadeClip->_crossFadeFrom = this;
    //_crossFadeClip->_inBlendWeight = 0.0f;
    _isFadingOut = true;
    _isFadingIn = false;

    if (_crossFadeFrom)
    {
        _crossFadeFrom->release();
        _crossFadeFrom = NULL;
    }

    _crossFadeOutDuration = duration;
    _crossFadeStart = (Game::getGameTime() - _timeStarted);

    //if (!_crossFadeClip->_isPlaying)
        _crossFadeClip->play();
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
        onBegin();
        _elapsedTime = Game::getGameTime() - _timeStarted;
        _runningTime = _elapsedTime * _speed;
    }
    else
    {
        // Update elapsed time.
        _elapsedTime += elapsedTime;
        _runningTime += elapsedTime * 0.25;
    }

    float percentComplete = 0.0f;

    // Check to see if clip is complete.
    if (_repeatCount != REPEAT_INDEFINITE && ((_speed >= 0 && _runningTime >= (long) _activeDuration) || (_speed < 0 && _runningTime <= 0)))
    {
        _isPlaying = false;
        if (_speed >= 0)
        {
            percentComplete = _activeDuration % _duration; // Get's the fractional part of the final repeat.
            if (percentComplete == 0.0f)
                percentComplete = _duration;
        }
        else
        {
            percentComplete = 0.0f; // If we are negative speed, the end value should be 0.
        }
    }
    else
    {
        // Gets portion/fraction of the repeat.
        percentComplete = (float) (_runningTime % _duration);
    }

    // Add back in start time, and divide by the total animation's duration to get the actual percentage complete
    percentComplete = (float)(_startTime + percentComplete) / (float) _animation->_duration;

    if (_isFadingIn)
    {
        if ((unsigned long) _elapsedTime < _crossFadeInDuration)
        {
            _blendWeight = (float) _elapsedTime / (float) _crossFadeInDuration;
        }
        else
        {
            _blendWeight = 1.0f;
            _isFadingIn = false;
        }

        if (_isFadingOut)
        {
            unsigned long elapsedFade = (_elapsedTime - _crossFadeStart) ;//* _speed;
            if (elapsedFade < _crossFadeOutDuration)
            {
                _blendWeight = (float) (_crossFadeOutDuration - elapsedFade) / (float) _crossFadeOutDuration;
                //_crossFadeClip->_blendWeight *= (1.0f - _blendWeight);// * _inBlendWeight;
            }
            else
            {
                _blendWeight = 0.0f;
                _isFadingOut = false;
                //_crossFadeClip->_blendWeight = 1.0f;
                //_crossFadeClip->_isFadingIn = false;
                _crossFadeClip->release();
                _crossFadeClip = NULL;
                _isPlaying = false;
            }
        }
    }
    else if (_isFadingOut)
    {
        unsigned long elapsedFade = (_elapsedTime - _crossFadeStart) ;//* _speed;
        if (elapsedFade < _crossFadeOutDuration)
        {
            _blendWeight = (float) (_crossFadeOutDuration - elapsedFade) / (float) _crossFadeOutDuration;
            //_crossFadeClip->_blendWeight *= (1.0f - _blendWeight);// * _inBlendWeight;
        }
        else
        {
            _blendWeight = 0.0f;
            _isFadingOut = false;
            //_crossFadeClip->_blendWeight = 1.0f;
            //_crossFadeClip->_isFadingIn = false;
            _crossFadeClip->release();
            _crossFadeClip = NULL;
            _isPlaying = false;
        }

        /*if (_isFadingIn)
        {
            if ((unsigned long) _elapsedTime < _crossFadeInDuration)
            {
                _blendWeight *= (float) _elapsedTime / (float) _crossFadeInDuration;
            }
            else
            {
                _blendWeight *= 1.0f;
                _isFadingIn = false;
            }
        }*/
    }

    /*float fadingOutBlendWeight = 0.0f;

    if (_crossFadeFrom)
        fadingOutBlendWeight = _crossFadeFrom->_blendWeight + _blendWeight;*/

    // Evaluate this clip.
    Animation::Channel* channel = NULL;
    AnimationValue* value = NULL;
    unsigned int channelCount = _animation->_channels.size();
    for (unsigned int i = 0; i < channelCount; i++)
    {
        channel = _animation->_channels[i];
        value = _values[i];

        // Get the current value.
        channel->_target->getAnimationPropertyValue(channel->_propertyId, value);

        unsigned int activeAnimationCount = channel->_target->getActiveAnimationCount();
        // My channel priority has changed if my priority is greater than the active animation count.
        if (_channelPriority[i] > activeAnimationCount)
        {
            // Readjust channel priority.
            _channelPriority[i] -= (_channelPriority[i] - activeAnimationCount);
        }

        if (_blendWeight != 0.0f)
        {
            // Evaluate point on Curve.
            //channel->_curve->evaluate(percentComplete, value->_currentValue);
            channel->_curve->evaluate(percentComplete, value->_interpolatedValue);

            if (channel->_curve->_quaternionOffsetsCount == 0)
            {
                if (_channelPriority[i] == 1)
                {
                    unsigned int componentCount = value->_componentCount;
                    for (unsigned int j = 0; j < componentCount; j++)
                    {
                        if (_blendWeight != 1.0f)
                            value->_interpolatedValue[j] *= _blendWeight;

                        value->_currentValue[j] = value->_interpolatedValue[j];
                    }
                }
                else
                {
                    unsigned int componentCount = value->_componentCount;
                    for (unsigned int j = 0; j < componentCount; j++)
                    {
                        if (_blendWeight != 1.0f)
                            value->_interpolatedValue[j] *= _blendWeight;

                        value->_currentValue[j] += value->_interpolatedValue[j];
                    }
                }
            }
            else
            {   //We have Quaternions!!!
                unsigned int j = 0;
                unsigned int quaternionOffsetIndex = 0;
                unsigned int quaternionOffset = 0;

                if (_channelPriority[i] == 1)
                {
                    do {
                        quaternionOffset = channel->_curve->_quaternionOffsets[quaternionOffsetIndex];
                        while (j < quaternionOffset)
                        {
                            if (_blendWeight != 1.0f)
                                value->_interpolatedValue[j] *= _blendWeight;

                            value->_currentValue[j] = value->_interpolatedValue[j];
                            j++;
                        }

                        // We are at the index for a quaternion component. Handle the next for components as a whole quaternion.

                        Quaternion* interpolatedQuaternion = (Quaternion*) (value->_interpolatedValue + j);
                        Quaternion* currentQuaternion = (Quaternion*) (value->_currentValue + j);

                        // If we have a blend weight, we apply it by slerping from the identity to our interpolated value at the given weight.
                        if (_blendWeight != 1.0f)
                            Quaternion::slerp(Quaternion::identity(), *interpolatedQuaternion, _blendWeight, interpolatedQuaternion);
                    
                        // Add in contribution.
                        currentQuaternion->set(*interpolatedQuaternion);
                    
                        // Increase by 4.
                        j += 4;
                        quaternionOffsetIndex++;
                    } while (quaternionOffsetIndex < channel->_curve->_quaternionOffsetsCount);

                    unsigned int componentCount = value->_componentCount;
                    // Handle remaining scalar values.
                    while (j < componentCount)
                    {
                        if (_blendWeight != 1.0f)
                            value->_interpolatedValue[j] *= _blendWeight;

                        value->_currentValue[j] = value->_interpolatedValue[j];
                        j++;
                    }
                }
                else
                {
                    do {
                        quaternionOffset = channel->_curve->_quaternionOffsets[quaternionOffsetIndex];
                        while (j < quaternionOffset)
                        {
                            if (_blendWeight != 1.0f)
                                value->_interpolatedValue[j] *= _blendWeight;

                            value->_currentValue[j] += value->_interpolatedValue[j];
                            j++;
                        }

                        // We are at the index for a quaternion component. Handle the next for components as a whole quaternion.

                        Quaternion* interpolatedQuaternion = (Quaternion*) (value->_interpolatedValue + j);
                        Quaternion* currentQuaternion = (Quaternion*) (value->_currentValue + j);

                        // If we have a blend weight, we apply it by slerping from the identity to our interpolated value at the given weight.
                        if (_blendWeight != 1.0f)
                            Quaternion::slerp(Quaternion::identity(), *interpolatedQuaternion, _blendWeight, interpolatedQuaternion);
                    
                        // Add in contribution.
                        //Quaternion::multiply(*, *currentQuaternion, currentQuaternion);
                        currentQuaternion->multiply(*interpolatedQuaternion);
                    
                        // Increase by 4.
                        j += 4;
                        quaternionOffsetIndex++;
                    } while (quaternionOffsetIndex < channel->_curve->_quaternionOffsetsCount);

                    unsigned int componentCount = value->_componentCount;
                    // Handle remaining scalar values.
                    while (j < componentCount)
                    {
                        if (_blendWeight != 1.0f)
                            value->_interpolatedValue[j] *= _blendWeight;

                        value->_currentValue[j] += value->_interpolatedValue[j];
                        j++;
                    }
                }
            }
        }
        else if (_channelPriority[i] == 1)
        {
            memset(value->_currentValue, 0.0f, value->_componentCount);
        }
        
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

    Animation::Channel* channel = NULL;
    AnimationValue* value = NULL;
    AnimationTarget* target = NULL;
    unsigned int channelCount = _animation->_channels.size();
    // Sets the starting value.
    for (unsigned int i = 0; i < channelCount; i++)
    {
        channel = _animation->_channels[i];
        value = _values[i];
        target = channel->_target;

        target->increaseActiveAnimationCount();
        _channelPriority[i] = target->getActiveAnimationCount();
    }

    // Notify begin listeners.. if any.
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
    Animation::Channel* channel = NULL;
    AnimationValue* value = NULL;
    AnimationTarget* target = NULL;
    unsigned int channelCount = _animation->_channels.size();
    for (unsigned int i = 0; i < channelCount; i++)
    {
        channel = _animation->_channels[i];
        value = _values[i];
        target = channel->_target;
        // Reset the clips channel priority, and decrease the active animation count on the target.
        _channelPriority[i] = 0;
        target->decreaseActiveAnimationCount();
    }

    _blendWeight = 1.0f;
    _isFadingIn = false;
    _isFadingOut = false;
    _timeStarted = 0;

    // Notify end listeners if any.
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
