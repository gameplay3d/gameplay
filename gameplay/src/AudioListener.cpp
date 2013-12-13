#include "Base.h"
#include "Node.h"
#include "AudioListener.h"
#include "Game.h"

namespace gameplay
{

AudioListener::AudioListener()
    : _gain(1.0f), _camera(NULL)
{
}

AudioListener::~AudioListener()
{
	// Call setCamera() to release camera and cause transform listener
	// to be removed.
	setCamera(NULL);
}

AudioListener* AudioListener::getInstance()
{
    return Game::getInstance()->getAudioListener();
}

float AudioListener::getGain() const 
{ 
    return _gain; 
}

void AudioListener::setGain(float gain)
{
    _gain = gain;
}

const Vector3& AudioListener::getPosition() const 
{ 
    return _position; 
}

void AudioListener::setPosition(const Vector3& position)
{
    _position = position;
}

void AudioListener::setPosition(float x, float y, float z)
{
    _position.set(x, y, z);
}

const Vector3& AudioListener::getVelocity() const 
{ 
    return _velocity; 
}

void AudioListener::setVelocity(const Vector3& velocity)
{
    _velocity = velocity;
}

void AudioListener::setVelocity(float x, float y, float z)
{
    _velocity.set(x, y, z);
}

const float* AudioListener::getOrientation() const
{
    return (const float*)&_orientation[0];
}

const Vector3& AudioListener::getOrientationForward() const 
{ 
    return _orientation[0]; 
}

const Vector3& AudioListener::getOrientationUp() const 
{ 
    return _orientation[1]; 
}

void AudioListener::setOrientation(const Vector3& forward, const Vector3& up)
{
    _orientation[0].x = forward.x;
    _orientation[0].y = forward.y;
    _orientation[0].z = forward.z;

    _orientation[1].x = up.x;
    _orientation[1].y = up.y;
    _orientation[1].z = up.z;
}

void AudioListener::setOrientation(float forwardX, float forwardY, float forwardZ, float upX, float upY, float upZ)
{
    _orientation[0].set(forwardX, forwardY, forwardZ);
    _orientation[1].set(upX, upY, upZ);
}

Camera* AudioListener::getCamera() const
{
    return _camera;
}

void AudioListener::setCamera(Camera* camera)
{
    if (_camera == camera)
        return;

    // Disconnect our current camera.
    if (_camera)
    {
        _camera->removeListener(this);
        SAFE_RELEASE(_camera);
    }

    // Connect the new camera.
    _camera = camera;
    if (_camera)
    {
        _camera->addRef();
        _camera->addListener(this);
    }
}

void AudioListener::cameraChanged(Camera* camera)
{
    if (_camera != camera)
        setCamera(camera);
   
    if (_camera)
    {
        Node* node = camera->getNode();
        if (node)
        {
            setPosition(node->getTranslationWorld());
            Vector3 up;
            node->getWorldMatrix().getUpVector(&up);
            setOrientation(node->getForwardVectorWorld(), up);
        }
        else
        {
            setPosition(Vector3::zero());
            setOrientation(Vector3::unitY(), -Vector3::unitZ());
        }
    }
}

}
