#include "Base.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Game.h"
#include "Quaternion.h"

namespace gameplay
{

AnimationClip::AnimationClip(const char* id, Animation* animation, unsigned long startTime, unsigned long endTime)
    : _id(id), _animation(animation), _startTime(startTime), _endTime(endTime), _duration(_endTime - _startTime), _repeatCount(1.0f), 
      _activeDuration(_duration * _repeatCount), _speed(1.0f), _isPlaying(false), _timeStarted(0), _elapsedTime(0), 
      _crossFadeToClip(NULL), _crossFadeOutElapsed(0), _crossFadeOutDuration(0), _blendWeight(1.0f), _isFadingOutStarted(false), 
      _isFadingOut(false), _isFadingIn(false), _beginListeners(NULL), _endListeners(NULL), _listeners(NULL), _listenerItr(NULL)
{
    assert(0 <= startTime && startTime <= animation->_duration && 0 <= endTime && endTime <= animation->_duration);
    
    unsigned int channelCount = _animation->_channels.size();    
    for (unsigned int i = 0; i < channelCount; i++)
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

    SAFE_RELEASE(_crossFadeToClip);
    SAFE_DELETE(_beginListeners);
    SAFE_DELETE(_endListeners);
    SAFE_DELETE(_listeners);
    SAFE_DELETE(_listenerItr);
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

void AnimationClip::setBlendWeight(float blendWeight)
{
    _blendWeight = blendWeight;
}

float AnimationClip::getBlendWeight() const
{
    return _blendWeight;
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
    assert(clip);

    // If I already have a clip I'm fading to and it's not the same as the given clip release it.
    // Assign the new clip and increase it's ref count.
    // TODO: Do I need to anything else to the crossFade clip here before releasing?
    if (_crossFadeToClip)
        SAFE_RELEASE(_crossFadeToClip);
        
    _crossFadeToClip = clip;
    _crossFadeToClip->addRef();
        
    // Set the crossfade clip to fading in, and initialize it's blend weight to zero.
    _crossFadeToClip->_isFadingIn = true;
    _crossFadeToClip->_blendWeight = 0.0f;
    
    // Set this clip to fade out, set the fade duration and reset the fade elapsed time.
    _isFadingOut = true;
    _crossFadeOutElapsed = 0;
    _crossFadeOutDuration = duration;
    _isFadingOutStarted = true;
    
    // If this clip is currently not playing, we should start playing it.
    if (!_isPlaying)
        play();

    // Start playing the cross fade clip.
    _crossFadeToClip->play(); 
}

void AnimationClip::addListener(AnimationClip::Listener* listener, unsigned long eventTime)
{
    assert(listener);
    assert(eventTime < _duration);

    listener->_listenerTime = eventTime;

    if (!_listeners)
    {
        _listeners = new std::list<Listener*>;
        _listeners->push_front(listener);

        _listenerItr = new std::list<Listener*>::iterator;
        if (_isPlaying)
            *_listenerItr = _listeners->begin();
    }
    else
    {
        for (std::list<Listener*>::iterator itr = _listeners->begin(); itr != _listeners->begin(); itr++)
        {
            if (eventTime < (*itr)->_listenerTime)
            {
                itr = _listeners->insert(itr, listener);

                // If playing, update the iterator if we need to.
                // otherwise, it will just be set the next time the clip gets played.
                if (_isPlaying)
                {
                    unsigned long currentTime = _elapsedTime % _duration;
                    if ((_speed >= 0.0f && currentTime < eventTime && (*_listenerItr == _listeners->end() || eventTime < (**_listenerItr)->_listenerTime)) || 
                        (_speed <= 0 && currentTime > eventTime && (*_listenerItr == _listeners->begin() || eventTime > (**_listenerItr)->_listenerTime)))
                        *_listenerItr = itr;
                }
                return;
            }
        }
    }
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

bool AnimationClip::update(unsigned long elapsedTime, std::list<AnimationTarget*>* activeTargets)
{
    if (!_isPlaying)
        onBegin();
    else 
        _elapsedTime += elapsedTime * _speed;

    unsigned long currentTime = 0L;
    // Check to see if clip is complete.
    if (_repeatCount != REPEAT_INDEFINITE && ((_speed >= 0.0f && _elapsedTime >= (long) _activeDuration) || (_speed <= 0.0f && _elapsedTime <= 0L)))
    {
        _isPlaying = false;
        if (_speed >= 0.0f)
        {
            currentTime = _activeDuration % _duration; // Get's the fractional part of the final repeat.
            if (currentTime == 0L)
                currentTime = _duration;
        }
        else
        {
            currentTime = 0L; // If we are negative speed, the end value should be 0.
        }
    }
    else
    {
        // Gets portion/fraction of the repeat.
        currentTime = _elapsedTime % _duration;
    }

    // Notify any listeners of Animation events.
    if (_listeners)
    {
        if (_speed >= 0.0f)
        {
            while (*_listenerItr != _listeners->end() && currentTime >= (**_listenerItr)->_listenerTime)
            {
                (**_listenerItr)->animationEvent(this, Listener::DEFAULT);
                ++*_listenerItr;
            }
        }
        else
        {
            while (*_listenerItr != _listeners->begin() && currentTime <= (**_listenerItr)->_listenerTime)
            {
                (**_listenerItr)->animationEvent(this, Listener::DEFAULT);
                --*_listenerItr;
            }
        }
    }

    // Add back in start time, and divide by the total animation's duration to get the actual percentage complete
    float percentComplete = (float)(_startTime + currentTime) / (float) _animation->_duration;
    
    if (_isFadingOut)
    {
        if (_isFadingOutStarted) // Calculate elapsed time since the fade out begin.
        {
            _crossFadeOutElapsed = (Game::getGameTime() - _crossFadeToClip->_timeStarted) * _speed; 
            _isFadingOutStarted = false;
        }
        else
        {
            // continue tracking elapsed time.
            _crossFadeOutElapsed += elapsedTime * abs(_speed);
        }

        if (_crossFadeOutElapsed < _crossFadeOutDuration)
        {
            // Calculate this clip's blend weight.
            float tempBlendWeight = (float) (_crossFadeOutDuration - _crossFadeOutElapsed) / (float) _crossFadeOutDuration;
            
            // If this clip is fading in, adjust the crossfade clip's weight to be a percentage of your current blend weight
            if (_isFadingIn)
            {
                _crossFadeToClip->_blendWeight = (1.0f - tempBlendWeight) * _blendWeight;
                _blendWeight -= _crossFadeToClip->_blendWeight;
            }
            else
            {
                // Just set the blend weight.
                _crossFadeToClip->_blendWeight = (1.0f - tempBlendWeight);
                _blendWeight = tempBlendWeight;
            }
        }
        else
        {   // Fade is done.
            // Set the crossFadeToClip's blend weight to 1
            _crossFadeToClip->_blendWeight = 1.0f;
            
            // Adjust the crossfade clip's blend weight if this clip is also fading in.
            if (_isFadingIn)
                _crossFadeToClip->_blendWeight *= _blendWeight;

            // The crossfade clip is no longer fading in
            _crossFadeToClip->_isFadingIn = false;

            // Release the crossfade clip, mark ourselves as done and set our blend weight to 0.
            SAFE_RELEASE(_crossFadeToClip);
            _blendWeight = 0.0f; 
            _isFadingOut = false;
            _isPlaying = false;
        }
    }
    
    // Evaluate this clip.
    Animation::Channel* channel = NULL;
    AnimationValue* value = NULL;
    AnimationTarget* target = NULL;
    unsigned int channelCount = _animation->_channels.size();
    for (unsigned int i = 0; i < channelCount; i++)
    {
        channel = _animation->_channels[i];
        target = channel->_target;
        value = _values[i];

        // If the target's _animationPropertyBitFlag is clear, we can assume that this is the first
        // animation channel to act on the target and we can add the target to the list of
        // active targets stored by the AnimationController.
        if (target->_animationPropertyBitFlag == 0x00)
            activeTargets->push_front(target);

        // Evaluate the point on Curve
        channel->_curve->evaluate(percentComplete, value->_value);
        // Set the animation value on the target property.
        target->setAnimationPropertyValue(channel->_propertyId, value, _blendWeight);
    }

    // When ended. Probably should move to it's own method so we can call it when the clip is ended early.
    if (!_isPlaying)
    {
        onEnd();
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

    return !_isPlaying;
}

void AnimationClip::onBegin()
{
    // Initialize animation to play.
    _isPlaying = true;

    if (_speed >= 0)
    {
        _elapsedTime = 0;

        if (_listeners)
            *_listenerItr = _listeners->begin(); 
    }
    else
    {
        _elapsedTime = _activeDuration;

        if (_listeners)
            *_listenerItr = _listeners->end();
    }

    _elapsedTime += (Game::getGameTime() - _timeStarted) * _speed;

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
    _blendWeight = 1.0f;
    _timeStarted = 0;
}

}
