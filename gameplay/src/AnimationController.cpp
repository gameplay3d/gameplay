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
    assert(keyCount >= 2 && keyTimes && keyValues && target);

    Animation* animation = new Animation(id, target, propertyId, keyCount, keyTimes, keyValues, type);

    addAnimation(animation);
    
    return animation;
}

Animation* AnimationController::createAnimation(const char* id, AnimationTarget* target, int propertyId, unsigned int keyCount, unsigned long* keyTimes, float* keyValues, float* keyInValue, float* keyOutValue, Curve::InterpolationType type)
{
    assert(target && keyCount >= 2 && keyTimes && keyValues && keyInValue && keyOutValue);
    Animation* animation = new Animation(id, target, propertyId, keyCount, keyTimes, keyValues, keyInValue, keyOutValue, type);

    addAnimation(animation);

    return animation;
}

Animation* AnimationController::createAnimation(const char* id, AnimationTarget* target, const char* animationFile)
{
    assert(target && animationFile);
    
    Properties* p = Properties::create(animationFile);
    assert(p);

    Animation* animation = createAnimation(id, target, p->getNextNamespace());

    SAFE_DELETE(p);

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
        SAFE_RELEASE(clip);
    }
    _runningClips.clear();

    _state = IDLE;
}

Animation* AnimationController::createAnimation(const char* id, AnimationTarget* target, Properties* animationProperties)
{
    assert(target && animationProperties);
    assert(std::strcmp(animationProperties->getNamespace(), "animation") == 0);
    
    const char* propertyIdStr = animationProperties->getString("property");
    assert(propertyIdStr);
    
    // Get animation target property id
    int propertyId = AnimationTarget::getPropertyId(target->_targetType, propertyIdStr);
    assert(propertyId != -1);
    
    unsigned int keyCount = animationProperties->getInt("keyCount");
    assert(keyCount > 0);

    const char* keyTimesStr = animationProperties->getString("keyTimes");
    assert(keyTimesStr);
    
    const char* keyValuesStr = animationProperties->getString("keyValues");
    assert(keyValuesStr);
    
    const char* curveStr = animationProperties->getString("curve");
    assert(curveStr);
    
    char delimeter = ' ';
    unsigned int startOffset = 0;
    unsigned int endOffset = std::string::npos;
    
    unsigned long* keyTimes = new unsigned long[keyCount];
    for (unsigned int i = 0; i < keyCount; i++)
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
    
    int componentCount = target->getAnimationPropertyComponentCount(propertyId);
    assert(componentCount > 0);
    
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
    if(keyOutStr)
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
        animation = createAnimation(id, target, propertyId, keyCount, keyTimes, keyValues, keyIn, keyOut, (Curve::InterpolationType)curve);
    }
    else
    {
        animation = createAnimation(id, target, propertyId, keyCount, keyTimes, keyValues, (Curve::InterpolationType) curve);
    }

    SAFE_DELETE(keyOut);
    SAFE_DELETE(keyIn);
    SAFE_DELETE(keyValues);
    SAFE_DELETE(keyTimes);

    Properties* pClip = animationProperties->getNextNamespace();
    if (pClip && std::strcmp(pClip->getNamespace(), "clip") == 0)
    {
        int frameCount = animationProperties->getInt("frameCount");
        assert(frameCount > 0);
        animation->createClips(animationProperties, (unsigned int) frameCount);
    }

    return animation;
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
    stopAllAnimations();
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
            SAFE_RELEASE(clip);
            clipIter = _runningClips.erase(clipIter);
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
            Animation* animation = *itr;
            _animations.erase(itr);
            SAFE_RELEASE(animation);
            return;
        }
        itr++;
    }
}

void AnimationController::destroyAllAnimations()
{
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
