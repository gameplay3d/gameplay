#include "Game.h"
#include "Platform.h"

namespace gameplay
{

inline Game::State Game::getState() const
{
    return _state;
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

template <class T>
void Game::renderOnce(T* instance, void (T::*method)(void*), void* cookie)
{
    GP_ASSERT(instance);
    (instance->*method)(cookie);
    Platform::swapBuffers();
}

inline void Game::setMultiTouch(bool enabled)
{
    Platform::setMultiTouch(enabled);
}

inline bool Game::isMultiTouch() const
{
    return Platform::isMultiTouch();
}

inline void Game::getAccelerometerValues(float* pitch, float* roll)
{
    Platform::getAccelerometerValues(pitch, roll);
}

inline void Game::displayKeyboard(bool display)
{
    Platform::displayKeyboard(display);
}

inline Gamepad* Game::getGamepad(unsigned int playerIndex) const
{
    GP_ASSERT(playerIndex < _gamepadCount);

    if (_gamepads)
        return _gamepads[playerIndex];
    else
        return NULL;
}

inline unsigned int Game::getGamepadCount() const
{
    return _gamepadCount;
}

}
