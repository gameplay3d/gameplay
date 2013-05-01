#include "Game.h"
#include "Platform.h"

namespace gameplay
{

inline Game::State Game::getState() const
{
    return _state;
}

inline bool Game::isInitialized() const
{
    return _initialized;
}

inline unsigned int Game::getFrameRate() const
{
    return _frameRate;
}

inline unsigned int Game::getWidth() const
{
    return _width;
}

inline unsigned int Game::getHeight() const
{
    return _height;
}

inline float Game::getAspectRatio() const
{
    return (float)_width / (float)_height;
}

inline const Rectangle& Game::getViewport() const
{
    return _viewport;
}

inline AnimationController* Game::getAnimationController() const
{
    return _animationController;
}

inline AudioController* Game::getAudioController() const
{
    return _audioController;
}

inline PhysicsController* Game::getPhysicsController() const
{
    return _physicsController;
}

inline ScriptController* Game::getScriptController() const
{
    return _scriptController;
}
inline AIController* Game::getAIController() const
{
    return _aiController;
}

template <class T>
void Game::renderOnce(T* instance, void (T::*method)(void*), void* cookie)
{
    GP_ASSERT(instance);
    (instance->*method)(cookie);
    Platform::swapBuffers();
}

inline bool Game::hasMouse()
{
    return Platform::hasMouse();
}

inline bool Game::isMouseCaptured()
{
    return Platform::isMouseCaptured();
}

inline void Game::setMouseCaptured(bool captured)
{
    Platform::setMouseCaptured(captured);
}

inline void Game::setCursorVisible(bool visible)
{
    Platform::setCursorVisible(visible);
}

inline bool Game::isCursorVisible()
{
    return Platform::isCursorVisible();
}

inline void Game::setMultiSampling(bool enabled)
{
    Platform::setMultiSampling(enabled);
}

inline bool Game::isMultiSampling() const
{
    return Platform::isMultiSampling();
}

inline void Game::setMultiTouch(bool enabled)
{
    Platform::setMultiTouch(enabled);
}

inline bool Game::isMultiTouch() const
{
    return Platform::isMultiTouch();
}

inline bool Game::canExit() const
{
    return Platform::canExit();
}

inline bool Game::hasAccelerometer() const
{
	return Platform::hasAccelerometer();
}

inline void Game::getAccelerometerValues(float* pitch, float* roll)
{
    Platform::getAccelerometerValues(pitch, roll);
}

inline void Game::getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    return Platform::getRawSensorValues(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
}

inline unsigned int Game::getGamepadCount() const
{
    return Gamepad::getGamepadCount();
}

inline Gamepad* Game::getGamepad(unsigned int index, bool preferPhysical) const
{
    return Gamepad::getGamepad(index, preferPhysical);
}

inline void Game::displayKeyboard(bool display)
{
    Platform::displayKeyboard(display);
}

inline bool Game::launchURL(const char* url) const
{
    return Platform::launchURL(url);
}

}
