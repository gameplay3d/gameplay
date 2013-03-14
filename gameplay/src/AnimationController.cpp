#include "Base.h"
#include "AnimationController.h"
#include "Game.h"
#include "Curve.h"

namespace gameplay
{

AnimationController::AnimationController()
    : _state(STOPPED)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::stopAllAnimations() 
{
    std::list<AnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        AnimationClip* clip = *clipIter;
        GP_ASSERT(clip);
        clip->stop();
        clipIter++;
    }
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
    std::list<AnimationClip*>::iterator itr = _runningClips.begin();
    for ( ; itr != _runningClips.end(); itr++)
    {
        AnimationClip* clip = *itr;
        SAFE_RELEASE(clip);
    }
    _runningClips.clear();
    _state = STOPPED;
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

    GP_ASSERT(clip);
    clip->addRef();
    _runningClips.push_back(clip);
}

void AnimationController::unschedule(AnimationClip* clip)
{
    std::list<AnimationClip*>::iterator clipItr = _runningClips.begin();
    while (clipItr != _runningClips.end())
    {
        AnimationClip* rClip = (*clipItr);
        if (rClip == clip)
        {
            _runningClips.erase(clipItr);
            SAFE_RELEASE(clip);
            break;
        }
        clipItr++;
    }

    if (_runningClips.empty())
        _state = IDLE;
}

void AnimationController::update(float elapsedTime)
{
    if (_state != RUNNING)
        return;
    
    Transform::suspendTransformChanged();

    // Loop through running clips and call update() on them.
    std::list<AnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        AnimationClip* clip = (*clipIter);
        GP_ASSERT(clip);
        clip->addRef();
        if (clip->isClipStateBitSet(AnimationClip::CLIP_IS_RESTARTED_BIT))
        {   // If the CLIP_IS_RESTARTED_BIT is set, we should end the clip and 
            // move it from where it is in the running clips list to the back.
            clip->onEnd();
            clip->setClipStateBit(AnimationClip::CLIP_IS_PLAYING_BIT);
            _runningClips.push_back(clip);
            clipIter = _runningClips.erase(clipIter);
        }
        else if (clip->update(elapsedTime))
        {
            clip->release();
            clipIter = _runningClips.erase(clipIter);
        }
        else
        {
            clipIter++;
        }
        clip->release();
    }

    Transform::resumeTransformChanged();

    if (_runningClips.empty())
        _state = IDLE;
}

}
