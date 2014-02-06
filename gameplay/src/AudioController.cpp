#include "Base.h"
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"
#include "tthread/tinythread.h"

namespace gameplay
{

std::auto_ptr< tthread::thread > AudioController::_streamingThread;
tthread::mutex AudioController::_streamingQueueMutex;
bool AudioController::_streamingThreadActive = true;


AudioController::AudioController() 
    : _alcDevice(NULL), _alcContext(NULL), _pausingSource(NULL)
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
}

void AudioController::finalize()
{
    GP_ASSERT( _streamedSources.empty() );

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

void AudioController::addPlayingSource(AudioSource * source)
{
    if (_playingSources.find(source) == _playingSources.end())
    {
        _playingSources.insert(source);

        if (source->isStreamed())
        {
            GP_ASSERT( _streamedSources.find(source) == _streamedSources.end() );
            bool startThread = _streamedSources.empty() && _streamingThread.get() == NULL;
            _streamingQueueMutex.lock();
            _streamedSources.insert(source);
            _streamingQueueMutex.unlock();

            if (startThread)
                _streamingThread.reset(new tthread::thread(&streamingThreadProc, this));
        }
    }
}

void AudioController::removePlayingSource(AudioSource * source)
{
    if (_pausingSource != source)
    {
        std::set<AudioSource*>::iterator iter = _playingSources.find(source);
        if (iter != _playingSources.end())
        {
            _playingSources.erase(iter);

            if (source->isStreamed())
            {
                GP_ASSERT( _streamedSources.find(source) != _streamedSources.end() );
                _streamingQueueMutex.lock();
                _streamedSources.erase(source);
                _streamingQueueMutex.unlock();
            }
        }
    }
}

void AudioController::streamingThreadProc(void* arg)
{
    AudioController * _this = (AudioController *) arg;

    while(_streamingThreadActive)
    {
        _streamingQueueMutex.lock();
        
        std::for_each(
            _this->_streamedSources.begin(),
            _this->_streamedSources.end(),
            std::mem_fun(&AudioSource::streamDataIfNeeded));
        
        _streamingQueueMutex.unlock();

        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(100));
    }
}

}
