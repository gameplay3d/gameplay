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
    (instance->*method)(cookie);
    Platform::swapBuffers();
}

inline void Game::setMultiTouch(bool enabled)
{
    Platform::setMultiTouch(enabled);
}

inline bool isMultiTouch()
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

template <typename T> void SplashDisplayer::run(T* instance, void (T::*method) (void*), void* cookie, long time)
{
    _time = time;
    Game::getInstance()->renderOnce(instance, method, cookie);
    _startTime = Game::getInstance()->getGameTime();
}

inline SplashDisplayer::~SplashDisplayer()
{
    long elapsedTime = Game::getInstance()->getGameTime() - _startTime;
    if (elapsedTime < _time)
        Platform::sleep(_time - (Game::getInstance()->getGameTime() - _startTime));
}

}