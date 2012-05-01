#include "Base.h"
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"


namespace gameplay
{

#ifndef __ANDROID__
AudioController::AudioController() 
    : _alcDevice(NULL), _alcContext(NULL), _pausingSource(NULL)
{
}
#else
AudioController::AudioController() 
    : _engineObject(NULL), _engineEngine(NULL), _outputMixObject(NULL), _listenerObject(NULL),
    _listenerDoppler(NULL), _listenerLocation(NULL), _pausingSource(NULL)
{
}
#endif

AudioController::~AudioController()
{
}

void AudioController::initialize()
{
#ifndef __ANDROID__
    _alcDevice = alcOpenDevice (NULL);
    if (!_alcDevice)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to open OpenAL device.\n");
        return;  
    }
        
    _alcContext = alcCreateContext(_alcDevice, NULL);
    ALCenum alcErr = alcGetError(_alcDevice);
    if (!_alcContext || alcErr != ALC_NO_ERROR)
    {
        alcCloseDevice (_alcDevice);
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to create OpenAL context. Error: %d\n", alcErr);
        return;
    }
    
    alcMakeContextCurrent(_alcContext);
    alcErr = alcGetError(_alcDevice);
    if (alcErr != ALC_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to make OpenAL context current. Error: %d\n", alcErr);
    }
#else
    // Create the engine.
    SLresult result = slCreateEngine(&_engineObject, 0, NULL, 0, NULL, NULL);
    if (result != SL_RESULT_SUCCESS)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to create OpenSL engine.");
        return;
    }

    // Realize the engine.
    result = (*_engineObject)->Realize(_engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to realize OpenSL engine.");
        return;
    }

    // Get the engine interface in order to create other objects later on.
    result = (*_engineObject)->GetInterface(_engineObject, SL_IID_ENGINE, &_engineEngine);
    if (result != SL_RESULT_SUCCESS)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to retrieve OpenSL engine interface.");
        return;
    }

    // Create the output mix.
    result = (*_engineEngine)->CreateOutputMix(_engineEngine, &_outputMixObject, 0, NULL, NULL);
    if (result != SL_RESULT_SUCCESS)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to create OpenSL output mix.");
        return;
    }

    // Realize the output mix.
    result = (*_outputMixObject)->Realize(_outputMixObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
    {
        LOG_ERROR("AudioController::initialize() error. Unable to realize OpenSL output mix.");
        return;
    }

    // Load the listener and its supported interfaces.
    if (!_listenerObject)
    {
        const SLInterfaceID interfaces[3] = {SL_IID_3DDOPPLER, SL_IID_3DLOCATION};
        const SLboolean required[3] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE};
        SLresult result = (*_engineEngine)->CreateListener(_engineEngine, &_listenerObject, 2, interfaces, required);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN_VARG("AudioController: failed to create listener (%u).", result);
            return;
        }

        result = (*_listenerObject)->Realize(_listenerObject, SL_BOOLEAN_FALSE);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioController: failed to realize listener.");
            return;
        }

        // Get the doppler interface in order to set the listener's velocity.
        result = (*_listenerObject)->GetInterface(_listenerObject, SL_IID_3DDOPPLER, &_listenerDoppler);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioController: Unable to retrieve listener doppler interface.");
            return;
        }

        // Get the location interface in order to set the listener's position and orientation.
        result = (*_listenerObject)->GetInterface(_listenerObject, SL_IID_3DLOCATION, &_listenerLocation);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioController: Unable to retrieve listener location interface.");
            return;
        }
    }
#endif
}

void AudioController::finalize()
{
#ifndef __ANDROID__
    alcMakeContextCurrent(NULL);
    if (_alcContext)
    {
        alcDestroyContext(_alcContext);
        _alcContext = NULL;
    }
    if (_alcDevice)
    {
        alcCloseDevice(_alcDevice);
        _alcDevice = NULL;
    }
#else
    if (_outputMixObject != NULL)
    {
        (*_outputMixObject)->Destroy(_outputMixObject);
        _outputMixObject = NULL;
    }

    if (_engineObject != NULL)
    {
        (*_engineObject)->Destroy(_engineObject);
        _engineObject = NULL;
        _engineEngine = NULL;
    }
#endif
}

void AudioController::pause()
{
    std::set<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, pause it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        source = *itr;
        _pausingSource = source;
        source->pause();
        _pausingSource = NULL;
        itr++;
    }
}

void AudioController::resume()
{
#ifndef __ANDROID__    
    alcMakeContextCurrent(_alcContext);
#endif
    std::set<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, resume it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        source = *itr;
        source->resume();
        itr++;
    }
}

void AudioController::update(long elapsedTime)
{
    AudioListener* listener = AudioListener::getInstance();
    if (listener)
    {
#ifndef __ANDROID__
        alListenerf(AL_GAIN, listener->getGain());
        alListenerfv(AL_ORIENTATION, (ALfloat*)listener->getOrientation());
        alListenerfv(AL_VELOCITY, (ALfloat*)&listener->getVelocity());
        alListenerfv(AL_POSITION, (ALfloat*)&listener->getPosition());
#else
        if (_listenerObject)
        {
            SLVec3D f;
            f.x = listener->getOrientationForward().x;
            f.y = listener->getOrientationForward().y;
            f.z = listener->getOrientationForward().z;
            SLVec3D a;
            a.x = listener->getOrientationUp().x;
            a.y = listener->getOrientationUp().y;
            a.z = listener->getOrientationUp().z;
            SLresult result = (*_listenerLocation)->SetOrientationVectors(_listenerLocation, &f, &a);
            if (result != SL_RESULT_SUCCESS)
            {
                WARN("AudioController: Unable to set listener orientation.");
            }

            SLVec3D p;
            p.x = listener->getPosition().x;
            p.y = listener->getPosition().y;
            p.z = listener->getPosition().z;
            result = (*_listenerLocation)->SetLocationCartesian(_listenerLocation, &p);
            if (result != SL_RESULT_SUCCESS)
            {
                WARN("AudioController: Unable to set listener location.");
            }

            SLVec3D v;
            v.x = listener->getVelocity().x;
            v.y = listener->getVelocity().y;
            v.z = listener->getVelocity().z;
            result = (*_listenerDoppler)->SetVelocityCartesian(_listenerDoppler, &v);
            if (result != SL_RESULT_SUCCESS)
            {
                WARN("AudioController: Unable to set listener velocity.");
            }
        }
#endif
    }
}

}
