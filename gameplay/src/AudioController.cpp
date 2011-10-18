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
    : _device(NULL), _context(NULL)
{
}

AudioController::~AudioController()
{
}

void AudioController::initialize()
{    
    // Open the audio device.
    _device = alcOpenDevice(NULL);
    if (!_device)
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to open device. ALc Error: %d", alcGetError(_device));

    // Create the audio context.
    _context = alcCreateContext(_device, NULL);
    ALCenum alcError = alcGetError(_device);
    if (_context == NULL)
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to create context. ALc Error: %d", alcError);

    // Make the context current.
    alcMakeContextCurrent(_context);
    alcError = alcGetError(_device);
    if ( alcError != ALC_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioController::initialize() error. Unable to make context current. ALc Error: %d", alcError);
        // no sense in going forward if we couldn't create a device or context
        return;  
    }
}

void AudioController::finalize()
{
    _context = alcGetCurrentContext();
    _device = alcGetContextsDevice(_context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(_context);

    if (_device)
    {
       alcCloseDevice(_device);
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
