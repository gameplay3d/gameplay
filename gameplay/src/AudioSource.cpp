#include "Base.h"
#include "AudioSource.h"

namespace gameplay
{

AudioSource::AudioSource()
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::play()
{
}

void AudioSource::pause()
{
}

void AudioSource::resume()
{
}

void AudioSource::stop()
{
}

void AudioSource::rewind()
{
}

AudioSource::State AudioSource::getState() const
{
    return AudioSource::STATE_INITIAL;
}

bool AudioSource::isLooped() const
{
    return false;
}

void AudioSource::setLooped(bool looped)
{
}

float AudioSource::getGain() const
{
    return 0.0f;
}

void AudioSource::setGain(float gain)
{
}

float AudioSource::getPitch() const
{
    return 0.0f;
}

void AudioSource::setPitch(float pitch)
{
}

void AudioSource::onPositionChanged(const Vector3& position)
{
}

void AudioSource::onVelocityChanged(const Vector3& velocity)
{
}

Component::TypeId AudioSource::getTypeId()
{
	return Component::TYPEID_AUDIO_SOURCE;
}

std::string AudioSource::getClassName()
{
	return "gameplay::AudioSource";
}

void AudioSource::onSerialize(Serializer * serializer)
{
}

void AudioSource::onDeserialize(Serializer * serializer)
{
}

std::shared_ptr<Serializable> AudioSource::createObject()
{
	return std::shared_ptr<AudioSource>();
}

}
