#include "Base.h"
#include "AudioListener.h"

namespace gameplay
{

AudioListener::AudioListener()
{
}

AudioListener::~AudioListener()
{
}

float AudioListener::getGain() const
{
    return 0.0f;
}

void AudioListener::setGain(float gain)
{
}

void AudioListener::onPositionChanged(const Vector3& position)
{
}

void AudioListener::onVelocityChanged(const Vector3& velocity)
{
}

void AudioListener::onOrientationChanged(const Vector3& forward, const Vector3& up)
{
}

}
