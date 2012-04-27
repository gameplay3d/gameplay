#include "Base.h"
#include "Node.h"
#include "AudioBuffer.h"
#include "AudioController.h"
#include "AudioSource.h"
#include "Game.h"
#include "Node.h"

namespace gameplay
{


#ifndef __ANDROID__
AudioSource::AudioSource(AudioBuffer* buffer, ALuint source) 
    : _alSource(source), _buffer(buffer), _looped(true), _gain(1.0f), _pitch(1.0f), _node(NULL)
{
    alSourcei(_alSource, AL_BUFFER, buffer->_alBuffer);
    alSourcei(_alSource, AL_LOOPING, _looped);
    alSourcef(_alSource, AL_PITCH, _pitch);
    alSourcef(_alSource, AL_GAIN, _gain);
    alSourcefv(_alSource, AL_VELOCITY, (const ALfloat*)&_velocity);
}
#else
AudioSource::AudioSource(AudioBuffer* buffer, const SLObjectItf& player)
    : _playerObject(player), _playerDoppler(NULL), _playerLocation(NULL), _playerPlay(NULL), _playerPitch(NULL),
    _playerSeek(NULL), _playerVolume(NULL), _buffer(buffer), _looped(true), _gain(1.0f), _pitch(1.0f), _node(NULL)
{
    // Get the different interfaces for the OpenSL audio player that we need.
    SLresult result = (*_playerObject)->GetInterface(_playerObject, SL_IID_3DDOPPLER, &_playerDoppler);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get 3D doppler interface for OpenSL audio player.");
    }
    
    result = (*_playerObject)->GetInterface(_playerObject, SL_IID_3DLOCATION, &_playerLocation);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get 3D location interface for OpenSL audio player.");
    }

    result = (*_playerObject)->GetInterface(_playerObject, SL_IID_PLAY, &_playerPlay);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get play interface for OpenSL audio player.");
    }

    result = (*_playerObject)->GetInterface(_playerObject, SL_IID_PITCH, &_playerPitch);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get rate pitch interface for OpenSL audio player.");
    }

    result = (*_playerObject)->GetInterface(_playerObject, SL_IID_SEEK, &_playerSeek);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get seek interface for OpenSL audio player.");
    }

    result = (*_playerObject)->GetInterface(_playerObject, SL_IID_VOLUME, &_playerVolume);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::AudioSource() - Failed to get volume interface for OpenSL audio player.");
    }

    // Get the max volume level (used to convert from our API's parameter to OpenSL's expected units).
    if (_playerVolume)
    {
        result = (*_playerVolume)->GetMaxVolumeLevel(_playerVolume, &_maxVolume);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::AudioSource() - Failed to get the max volume level for OpenSL audio player (needed for parameter conversion).");
        }
    }

    setLooped(_looped);
    setPitch(_pitch);
    setGain(_gain);
    setVelocity(_velocity);
}
#endif

AudioSource::~AudioSource()
{
#ifndef __ANDROID__
    if (_alSource)
    {
        alDeleteSources(1, &_alSource);
        _alSource = 0;
    }
#else
    if (_playerObject)
    {
        (*_playerObject)->Destroy(_playerObject);
        _playerObject = NULL;
        _playerDoppler = NULL;
        _playerLocation = NULL;
        _playerPlay = NULL;
        _playerPitch = NULL;
        _playerSeek = NULL;
        _playerVolume = NULL;
    }
#endif

    SAFE_RELEASE(_buffer);
}

AudioSource* AudioSource::create(const char* path)
{
    assert(path);

    // Load from a .audio file.
    std::string pathStr = path;
    if (pathStr.find(".audio") != pathStr.npos)
    {
        Properties* properties = Properties::create(path);
        assert(properties);
        if (properties == NULL)
        {
            return NULL;
        }

        AudioSource* audioSource = create(properties->getNextNamespace());
        SAFE_DELETE(properties);
        return audioSource;
    }

    // Create an audio buffer from this path.
    AudioBuffer* buffer = AudioBuffer::create(path);
    if (buffer == NULL)
        return NULL;

#ifndef __ANDROID__
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
#else
    AudioController* audioController = Game::getInstance()->getAudioController();
    SLDataLocator_OutputMix locator = {SL_DATALOCATOR_OUTPUTMIX, audioController->_outputMixObject};

    SLDataSource dataSource = {&buffer->_data, &buffer->_mime};
    SLDataSink dataSink = {&locator, NULL};

    SLObjectItf player;
    const SLInterfaceID interfaces[] = {SL_IID_3DDOPPLER, SL_IID_3DLOCATION, SL_IID_PLAY, SL_IID_PITCH, SL_IID_SEEK, SL_IID_VOLUME};
    const SLboolean required[] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE};
    SLresult result = (*audioController->_engineEngine)->CreateAudioPlayer(audioController->_engineEngine, &player, &dataSource, &dataSink, 6, interfaces, required);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::create - Failed to create OpenSL audio player.");
        return NULL;
    }

    result = (*player)->Realize(player, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
    {
        WARN("AudioSource::create - Failed to realize OpenSL audio player.");
    }

    return new AudioSource(buffer, player);
#endif
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
#ifndef __ANDROID__
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
#else
    if (_playerPlay != NULL)
    {
        SLuint32 state;
        SLresult result = (*_playerPlay)->GetPlayState(_playerPlay, &state);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::getState() failed to get player state.");
        }

        switch (state)
        {
            case SL_PLAYSTATE_PLAYING:
                return PLAYING;
            case SL_PLAYSTATE_PAUSED:
                return PAUSED;
            case SL_PLAYSTATE_STOPPED:
                return STOPPED;
            default:
                return INITIAL;
        }
    }
#endif

    return INITIAL;
}

void AudioSource::play()
{
#ifndef __ANDROID__
    alSourcePlay(_alSource);
#else
    if (_playerPlay != NULL)
    {
        SLresult result = (*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_PLAYING);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::play() failed to set player state.");
        }
    }
#endif

    // Add the source to the controller's list of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    if (audioController->_playingSources.find(this) == audioController->_playingSources.end())
        audioController->_playingSources.insert(this);
}

void AudioSource::pause()
{
#ifndef __ANDROID__
    alSourcePause(_alSource);
#else
    if (_playerPlay != NULL)
    {
        SLresult result = (*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_PAUSED);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::pause() failed to set player state.");
        }
    }
#endif

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
#ifndef __ANDROID__
    alSourceStop(_alSource);
#else
    if (_playerPlay != NULL)
    {
        SLresult result = (*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_STOPPED);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::stop() failed to set player state.");
        }
    }
#endif

    // Remove the source from the controller's set of currently playing sources.
    AudioController* audioController = Game::getInstance()->getAudioController();
    std::set<AudioSource*>::iterator iter = audioController->_playingSources.find(this);
    if (iter != audioController->_playingSources.end())
        audioController->_playingSources.erase(iter);
}

void AudioSource::rewind()
{
#ifndef __ANDROID__
    alSourceRewind(_alSource);
#else
    if (_playerPlay != NULL)
    {
        SLresult result = (*_playerPlay)->SetMarkerPosition(_playerPlay, 0);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::rewind() failed to set player marker position.");
        }
    }
#endif
}

bool AudioSource::isLooped() const
{
    return _looped;
}

void AudioSource::setLooped(bool looped)
{
#ifndef __ANDROID__
     // Clear error state.
    alGetError();
    alSourcei(_alSource, AL_LOOPING, (looped) ? AL_TRUE : AL_FALSE);

    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioSource::setLooped Error: %d", error);
    }
#else
    if (_playerSeek)
    {
        SLresult result = (*_playerSeek)->SetLoop(_playerSeek, looped, 0, SL_TIME_UNKNOWN);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::setLooped() failed.");
        }
    }
#endif

    _looped = looped;
}

float AudioSource::getGain() const
{
    return _gain;
}

void AudioSource::setGain(float gain)
{
#ifndef __ANDROID__
    alSourcef(_alSource, AL_GAIN, gain);
#else
    if (_playerVolume)
    {
        SLmillibel volume = (gain < MATH_EPSILON) ? SL_MILLIBEL_MIN : (10.0f * log10(gain)) * 100;
        SLresult result = (*_playerVolume)->SetVolumeLevel(_playerVolume, volume);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::setGain() failed to set player gain.");
        }
    }
#endif
    _gain = gain;
}

float AudioSource::getPitch() const
{
    return _pitch;
}

void AudioSource::setPitch(float pitch)
{
#ifndef __ANDROID__
    alSourcef(_alSource, AL_PITCH, pitch);
#else
    if (_playerPitch)
    {
        SLresult result = (*_playerPitch)->SetPitch(_playerPitch, (SLpermille)(pitch * 1000));
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::setPitch() failed to set player pitch.");
        }
    }
#endif
    _pitch = pitch;
}

const Vector3& AudioSource::getVelocity() const
{
    return _velocity;
}

void AudioSource::setVelocity(const Vector3& velocity)
{
#ifndef __ANDROID__
    alSourcefv(_alSource, AL_VELOCITY, (ALfloat*)&velocity);
#else
    if (_playerDoppler)
    {
        SLVec3D v;
        v.x = velocity.x;
        v.y = velocity.y;
        v.z = velocity.z;
        SLresult result = (*_playerDoppler)->SetVelocityCartesian(_playerDoppler, &v);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::setVelocity - failed to set velocity.");
        }
    }
#endif
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
#ifndef __ANDROID__
    if (_node)
    {
        Vector3 translation = _node->getTranslationWorld();
        alSourcefv(_alSource, AL_POSITION, (const ALfloat*)&translation.x);
    }
#else
    if (_playerLocation)
    {
        SLVec3D position;
        position.x = transform->getTranslationX();
        position.y = transform->getTranslationY();
        position.z = transform->getTranslationZ();
        SLresult result = (*_playerLocation)->SetLocationCartesian(_playerLocation, &position);
        if (result != SL_RESULT_SUCCESS)
        {
            WARN("AudioSource::transformChanged - failed to update location.");
        }
    }
#endif
}

AudioSource* AudioSource::clone(NodeCloneContext &context) const
{
#ifndef __ANDROID__
    ALuint alSource = 0;
    alGenSources(1, &alSource);
    if (alGetError() != AL_NO_ERROR)
    {
        LOG_ERROR("AudioSource::createAudioSource - Error generating audio source.");
        return NULL;
    }
    AudioSource* audioClone = new AudioSource(_buffer, alSource);
#else
    // TODO: Implement cloning audio source for Android
    AudioSource* audioClone = new AudioSource(_buffer, _playerObject);

#endif
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
