#include "Base.h"
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"

namespace gameplay
{

AudioController::AudioController() 
: _alcDevice(NULL), _alcContext(NULL), _pausingSource(NULL), _streamingThreadActive(true)
{
}

AudioController::~AudioController()
{
}

void AudioController::initialize()
{
    _alcDevice = alcOpenDevice(NULL);
    if (!_alcDevice)
    {
        GP_ERROR("Unable to open OpenAL device.\n");
        return;
    }
    
    _alcContext = alcCreateContext(_alcDevice, NULL);
    ALCenum alcErr = alcGetError(_alcDevice);
    if (!_alcContext || alcErr != ALC_NO_ERROR)
    {
        alcCloseDevice(_alcDevice);
        GP_ERROR("Unable to create OpenAL context. Error: %d\n", alcErr);
        return;
    }
    
    alcMakeContextCurrent(_alcContext);
    alcErr = alcGetError(_alcDevice);
    if (alcErr != ALC_NO_ERROR)
    {
        GP_ERROR("Unable to make OpenAL context current. Error: %d\n", alcErr);
    }
    _streamingMutex.reset(new std::mutex());
}

void AudioController::finalize()
{
    GP_ASSERT(_streamingSources.empty());
    if (_streamingThread.get())
    {
        _streamingThreadActive = false;
        _streamingThread->join();
        _streamingThread.reset(NULL);
    }

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
    std::set<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, pause it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        GP_ASSERT(*itr);
        source = *itr;
        _pausingSource = source;
        source->pause();
        _pausingSource = NULL;
        itr++;
    }
}

void AudioController::resume()
{   
    alcMakeContextCurrent(_alcContext);

    std::set<AudioSource*>::iterator itr = _playingSources.begin();

    // For each source that is playing, resume it.
    AudioSource* source = NULL;
    while (itr != _playingSources.end())
    {
        GP_ASSERT(*itr);
        source = *itr;
        source->resume();
        itr++;
    }
}

void AudioController::update(float elapsedTime)
{
    AudioListener* listener = AudioListener::getInstance();
    if (listener)
    {
        AL_CHECK( alListenerf(AL_GAIN, listener->getGain()) );
        AL_CHECK( alListenerfv(AL_ORIENTATION, (ALfloat*)listener->getOrientation()) );
        AL_CHECK( alListenerfv(AL_VELOCITY, (ALfloat*)&listener->getVelocity()) );
        AL_CHECK( alListenerfv(AL_POSITION, (ALfloat*)&listener->getPosition()) );
    }
}

void AudioController::addPlayingSource(AudioSource* source)
{
    if (_playingSources.find(source) == _playingSources.end())
    {
        _playingSources.insert(source);

        if (source->isStreamed())
        {
            GP_ASSERT(_streamingSources.find(source) == _streamingSources.end());
            bool startThread = _streamingSources.empty() && _streamingThread.get() == NULL;
            _streamingMutex->lock();
            _streamingSources.insert(source);
            _streamingMutex->unlock();

            if (startThread)
                _streamingThread.reset(new std::thread(&streamingThreadProc, this));
        }
    }
}

void AudioController::removePlayingSource(AudioSource* source)
{
    if (_pausingSource != source)
    {
        std::set<AudioSource*>::iterator iter = _playingSources.find(source);
        if (iter != _playingSources.end())
        {
            _playingSources.erase(iter);
 
            if (source->isStreamed())
            {
                GP_ASSERT(_streamingSources.find(source) != _streamingSources.end());
                _streamingMutex->lock();
                _streamingSources.erase(source);
                _streamingMutex->unlock();
            }
        }
    } 
}

void AudioController::streamingThreadProc(void* arg)
{
    AudioController* controller = (AudioController*)arg;

    while (controller->_streamingThreadActive)
    {
        controller->_streamingMutex->lock();

        std::for_each(controller->_streamingSources.begin(), controller->_streamingSources.end(), std::mem_fn(&AudioSource::streamDataIfNeeded));
        
        controller->_streamingMutex->unlock();
   
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

}
