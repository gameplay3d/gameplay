#include "Base.h"
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"


namespace gameplay
{

std::list<AudioSource*> AudioController::_playingSources;

AudioController::AudioController() 
    : _alcDevice(NULL), _alcContext(NULL)
{
}

AudioController::~AudioController()
{
}

void AudioController::initialize()
{    
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
}

void AudioController::finalize()
{
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
}

void AudioController::pause()
{
    std::list<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, pause it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        source = *itr;
        if (source->getState() == AudioSource::PLAYING)
        {
            source->pause();
        }
        itr++;
    }
}

void AudioController::resume()
{
    std::list<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, resume it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        source = *itr;
        if (source->getState() == AudioSource::PAUSED)
        {
            source->play();
        }
        itr++;
    }
}

void AudioController::update(long elapsedTime)
{
    AudioListener* listener = AudioListener::getInstance();
    if (listener)
    {
        alListenerf(AL_GAIN, listener->getGain());
        alListenerfv(AL_ORIENTATION, (ALfloat*)&listener->getOrientationForward());
        alListenerfv(AL_VELOCITY, (ALfloat*)&listener->getVelocity());
        alListenerfv(AL_POSITION, (ALfloat*)&listener->getPosition());
    }
}

}
