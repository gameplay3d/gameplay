/*
 * AudioController.cpp
 */

#include "Base.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"
#include "AudioController.h"

namespace gameplay
{

std::list<AudioSource*> AudioController::_playingSources;

AudioController::AudioController()
{
}

AudioController::~AudioController()
{
}

void AudioController::initialize()
{    
    alutInit(0, 0);

    ALenum errorID = alutGetError();
    if ( errorID != ALUT_ERROR_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to initialize alut: %s\n", alutGetErrorString(errorID));
        return;  
    }
}

void AudioController::finalize()
{
    alutExit();
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
