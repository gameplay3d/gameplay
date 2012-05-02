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
    : _alSource(source), _buffer(buffer), _looped(true), _gain(1.0f), _pitch(1.0f), _node(NULL)
{
    alSourcei(_alSource, AL_BUFFER, buffer->_alBuffer);
    alSourcei(_alSource, AL_LOOPING, _looped);
    alSourcef(_alSource, AL_PITCH, _pitch);
    alSourcef(_alSource, AL_GAIN, _gain);
    alSourcefv(_alSource, AL_VELOCITY, (const ALfloat*)&_velocity);
}

AudioSource::~AudioSource()
{
    if (_alSource)
    {
        alDeleteSources(1, &_alSource);
        _alSource = 0;
    }
    SAFE_RELEASE(_buffer);
}

AudioSource* AudioSource::create(const char* url)
{
    assert(url);

    // Load from a .audio file.
    std::string pathStr = url;
    if (pathStr.find(".audio") != pathStr.npos)
    {
        Properties* properties = Properties::create(url);
        assert(properties);
        if (properties == NULL)
        {
            return NULL;
        }

        AudioSource* audioSource = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
        SAFE_DELETE(properties);
        return audioSource;
    }

    // Create an audio buffer from this URL.
    AudioBuffer* buffer = AudioBuffer::create(url);
    if (buffer == NULL)
        return NULL;

    // Load the audio source.
    ALuint alSource = 0;

    alGenSources(1, &alSource);
    if (alGetError() != AL_NO_ERROR)
    {
        SAFE_RELEASE(buffer);
        LOG_ERROR("AudioSource::createAudioSource - Error generating audio source.");
        return NULL;
    }
    
    return new AudioSource(buffer, alSource);
}

AudioSource* AudioSource::create(Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    assert(properties);
    if (!properties || !(strcmp(properties->getNamespace(), "audio") == 0))
    {
        WARN("Failed to load audio source from properties object: must be non-null object and have namespace equal to \'audio\'.");
        return NULL;
    }

    const char* path = properties->getString("path");
    if (path == NULL)
    {
        WARN("Audio file failed to load; the file path was not specified.");
        return NULL;
    }

    // Create the audio source.
    AudioSource* audio = AudioSource::create(path);
    if (audio == NULL)
    {
        WARN_VARG("Audio file '%s' failed to load properly.", path);
        return NULL;
    }

    // Set any properties that the user specified in the .audio file.
    if (properties->getString("looped") != NULL)
    {
        audio->setLooped(properties->getBool("looped"));
    }
    if (properties->getString("gain") != NULL)
    {
        audio->setGain(properties->getFloat("gain"));
    }
    if (properties->getString("pitch") != NULL)
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
    alGetSourcei(_alSource, AL_SOURCE_STATE, &state);

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

void AudioSource::play()
{
    alSourcePlay(_alSource);

    // Add the source to the controller's list of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    if (audioController->_playingSources.find(this) == audioController->_playingSources.end())
        audioController->_playingSources.insert(this);
}

void AudioSource::pause()
{
    alSourcePause(_alSource);

    // Remove the source from the controller's set of currently playing sources
    // if the source is being paused by the user and not the controller itself.
    AudioController* audioController = Game::getInstance()->getAudioController();
    if (audioController->_pausingSource != this)
    {
        std::set<AudioSource*>::iterator iter = audioController->_playingSources.find(this);
        if (iter != audioController->_playingSources.end())
        {
            WARN("\n\nRemoving an audio source from the list of playing sources...\n\n\n");
            audioController->_playingSources.erase(iter);
        }
    }
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
    alSourceStop(_alSource);

    // Remove the source from the controller's set of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    std::set<AudioSource*>::iterator iter = audioController->_playingSources.find(this);
    if (iter != audioController->_playingSources.end())
        audioController->_playingSources.erase(iter);
}

void AudioSource::rewind()
{
    alSourceRewind(_alSource);
}

bool AudioSource::isLooped() const
{
    return _looped;
}

void AudioSource::setLooped(bool looped)
{
    alGetError();
    alSourcei(_alSource, AL_LOOPING, (looped) ? AL_TRUE : AL_FALSE);

    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioSource::setLooped Error: %d", error);
    }
    _looped = looped;
}

float AudioSource::getGain() const
{
    return _gain;
}

void AudioSource::setGain(float gain)
{
    alSourcef(_alSource, AL_GAIN, gain);
    _gain = gain;
}

float AudioSource::getPitch() const
{
    return _pitch;
}

void AudioSource::setPitch(float pitch)
{
    alSourcef(_alSource, AL_PITCH, pitch);
    _pitch = pitch;
}

const Vector3& AudioSource::getVelocity() const
{
    return _velocity;
}

void AudioSource::setVelocity(const Vector3& velocity)
{
    alSourcefv(_alSource, AL_VELOCITY, (ALfloat*)&velocity);
    _velocity = velocity;
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
        alSourcefv(_alSource, AL_POSITION, (const ALfloat*)&translation.x);
    }
}

AudioSource* AudioSource::clone(NodeCloneContext &context) const
{
    ALuint alSource = 0;
    alGenSources(1, &alSource);
    if (alGetError() != AL_NO_ERROR)
    {
        LOG_ERROR("AudioSource::createAudioSource - Error generating audio source.");
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

}
