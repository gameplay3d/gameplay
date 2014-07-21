#include "Base.h"
#include "Node.h"
#include "AudioBuffer.h"
#include "AudioController.h"
#include "AudioSource.h"
#include "Game.h"
#include "Node.h"

namespace gameplay
{

AudioSource::AudioSource(AudioBuffer* buffer, ALuint source) 
    : _alSource(source), _buffer(buffer), _looped(false), _gain(1.0f), _pitch(1.0f), _node(NULL)
{
    GP_ASSERT(buffer);

    if (isStreamed())
        AL_CHECK(alSourceQueueBuffers(_alSource, 1, &buffer->_alBufferQueue[0]));
    else
        AL_CHECK(alSourcei(_alSource, AL_BUFFER, buffer->_alBufferQueue[0]));
    
    AL_CHECK(alSourcei(_alSource, AL_LOOPING, _looped && !isStreamed()));
    
    AL_CHECK( alSourcef(_alSource, AL_PITCH, _pitch) );
    AL_CHECK( alSourcef(_alSource, AL_GAIN, _gain) );
    AL_CHECK( alSourcefv(_alSource, AL_VELOCITY, (const ALfloat*)&_velocity) );
}

AudioSource::~AudioSource()
{
    if (_alSource)
    {
        // Remove the source from the controller's set of currently playing sources
        // regardless of the source's state. E.g. when the AudioController::pause is called
        // all sources are paused but still remain in controller's set of currently 
        // playing sources. When the source is deleted afterwards, it should be removed
        // from controller's set regardless of its playing state.
        AudioController* audioController = Game::getInstance()->getAudioController();
        GP_ASSERT(audioController);
        audioController->removePlayingSource(this);

        AL_CHECK(alDeleteSources(1, &_alSource));
        _alSource = 0;
    }
    SAFE_RELEASE(_buffer);
}

AudioSource* AudioSource::create(const char* url, bool streamed)
{
    // Load from a .audio file.
    std::string pathStr = url;
    if (pathStr.find(".audio") != std::string::npos)
    {
        Properties* properties = Properties::create(url);
        if (properties == NULL)
        {
            GP_ERROR("Failed to create audio source from .audio file.");
            return NULL;
        }

        AudioSource* audioSource = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
        SAFE_DELETE(properties);
        return audioSource;
    }

    // Create an audio buffer from this URL.
    AudioBuffer* buffer = AudioBuffer::create(url, streamed);
    if (buffer == NULL)
        return NULL;

    // Load the audio source.
    ALuint alSource = 0;

    AL_CHECK( alGenSources(1, &alSource) );
    if (AL_LAST_ERROR())
    {
        SAFE_RELEASE(buffer);
        GP_ERROR("Error generating audio source.");
        return NULL;
    }
    
    return new AudioSource(buffer, alSource);
}

AudioSource* AudioSource::create(Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    GP_ASSERT(properties);
    if (!properties || !(strcmp(properties->getNamespace(), "audio") == 0))
    {
        GP_ERROR("Failed to load audio source from properties object: must be non-null object and have namespace equal to 'audio'.");
        return NULL;
    }

    std::string path;
    if (!properties->getPath("path", &path))
    {
        GP_ERROR("Audio file failed to load; the file path was not specified.");
        return NULL;
    }

    bool streamed = false;
    if (properties->exists("streamed"))
    {
        streamed = properties->getBool("streamed");
    }

    // Create the audio source.
    AudioSource* audio = AudioSource::create(path.c_str(), streamed);
    if (audio == NULL)
    {
        GP_ERROR("Audio file '%s' failed to load properly.", path.c_str());
        return NULL;
    }

    // Set any properties that the user specified in the .audio file.
    if (properties->exists("looped"))
    {
        audio->setLooped(properties->getBool("looped"));
    }
    if (properties->exists("gain"))
    {
        audio->setGain(properties->getFloat("gain"));
    }
    if (properties->exists("pitch"))
    {
        audio->setPitch(properties->getFloat("pitch"));
    }
    Vector3 v;
    if (properties->getVector3("velocity", &v))
    {
        audio->setVelocity(v);
    }

    return audio;
}

AudioSource::State AudioSource::getState() const
{
    ALint state;
    AL_CHECK( alGetSourcei(_alSource, AL_SOURCE_STATE, &state) );

    switch (state)
    {
        case AL_PLAYING: 
            return PLAYING;
        case AL_PAUSED:  
            return PAUSED;
        case AL_STOPPED: 
            return STOPPED;
        default:         
            return INITIAL;
    }
    return INITIAL;
}

bool AudioSource::isStreamed() const
{
    GP_ASSERT(_buffer);
    return _buffer->_streamed;
}

void AudioSource::play()
{
    AL_CHECK( alSourcePlay(_alSource) );

    // Add the source to the controller's list of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    GP_ASSERT(audioController);
    audioController->addPlayingSource(this);
}

void AudioSource::pause()
{
    AL_CHECK( alSourcePause(_alSource) );

    // Remove the source from the controller's set of currently playing sources
    // if the source is being paused by the user and not the controller itself.
    AudioController* audioController = Game::getInstance()->getAudioController();
    GP_ASSERT(audioController);
    audioController->removePlayingSource(this);
}

void AudioSource::resume()
{
    if (getState() == PAUSED)
    {
        play();
    }
}

void AudioSource::stop()
{
    AL_CHECK( alSourceStop(_alSource) );

    // Remove the source from the controller's set of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    GP_ASSERT(audioController);
    audioController->removePlayingSource(this);
}

void AudioSource::rewind()
{
    AL_CHECK( alSourceRewind(_alSource) );
}

bool AudioSource::isLooped() const
{
    return _looped;
}

void AudioSource::setLooped(bool looped)
{
    AL_CHECK(alSourcei(_alSource, AL_LOOPING, (looped && !isStreamed()) ? AL_TRUE : AL_FALSE));
    if (AL_LAST_ERROR())
    {
        GP_ERROR("Failed to set audio source's looped attribute with error: %d", AL_LAST_ERROR());
    }
    _looped = looped;
}

float AudioSource::getGain() const
{
    return _gain;
}

void AudioSource::setGain(float gain)
{
    AL_CHECK( alSourcef(_alSource, AL_GAIN, gain) );
    _gain = gain;
}

float AudioSource::getPitch() const
{
    return _pitch;
}

void AudioSource::setPitch(float pitch)
{
    AL_CHECK( alSourcef(_alSource, AL_PITCH, pitch) );
    _pitch = pitch;
}

const Vector3& AudioSource::getVelocity() const
{
    return _velocity;
}

void AudioSource::setVelocity(const Vector3& velocity)
{
    AL_CHECK( alSourcefv(_alSource, AL_VELOCITY, (ALfloat*)&velocity) );
    _velocity = velocity;
}

void AudioSource::setVelocity(float x, float y, float z)
{
    setVelocity(Vector3(x, y, z));
}

Node* AudioSource::getNode() const
{
    return _node;
}

void AudioSource::setNode(Node* node)
{
    if (_node != node)
    {
        // Disconnect our current transform.
        if (_node)
        {
            _node->removeListener(this);
        }

        // Connect the new node.
        _node = node;

        if (_node)
        {
            _node->addListener(this);
            // Update the audio source position.
            transformChanged(_node, 0);
        }
    }
}

void AudioSource::transformChanged(Transform* transform, long cookie)
{
    if (_node)
    {
        Vector3 translation = _node->getTranslationWorld();
        AL_CHECK( alSourcefv(_alSource, AL_POSITION, (const ALfloat*)&translation.x) );
    }
}

AudioSource* AudioSource::clone(NodeCloneContext &context) const
{
    GP_ASSERT(_buffer);

    ALuint alSource = 0;
    AL_CHECK( alGenSources(1, &alSource) );
    if (AL_LAST_ERROR())
    {
        GP_ERROR("Error generating audio source.");
        return NULL;
    }
    AudioSource* audioClone = new AudioSource(_buffer, alSource);

    _buffer->addRef();
    audioClone->setLooped(isLooped());
    audioClone->setGain(getGain());
    audioClone->setPitch(getPitch());
    audioClone->setVelocity(getVelocity());
    if (Node* node = getNode())
    {
        Node* clonedNode = context.findClonedNode(node);
        if (clonedNode)
        {
            audioClone->setNode(clonedNode);
        }
    }
    return audioClone;
}

bool AudioSource::streamDataIfNeeded()
{
    GP_ASSERT( isStreamed() );
    if( getState() != PLAYING )
        return false;

    int queuedBuffers;
    alGetSourcei(_alSource, AL_BUFFERS_QUEUED, &queuedBuffers);
 
    int buffersNeeded = std::min<int>(_buffer->_buffersNeededCount, AudioBuffer::STREAMING_BUFFER_QUEUE_SIZE);
    if (queuedBuffers < buffersNeeded)
    {
        while (queuedBuffers < buffersNeeded)
        {
            if (!_buffer->streamData(_buffer->_alBufferQueue[queuedBuffers], _looped))
                return false;
            
            AL_CHECK( alSourceQueueBuffers(_alSource, 1, &_buffer->_alBufferQueue[queuedBuffers]) );
            queuedBuffers++;
        }
    }
    else
    {
        int processedBuffers;
        alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processedBuffers);
 
        while (processedBuffers-- > 0)
        {
            ALuint bufferID;
            AL_CHECK( alSourceUnqueueBuffers(_alSource, 1, &bufferID) );
            if (!_buffer->streamData(bufferID, _looped))
                return false;
            
            AL_CHECK( alSourceQueueBuffers(_alSource, 1, &bufferID) );
        }
    }
    return true;
}

}
