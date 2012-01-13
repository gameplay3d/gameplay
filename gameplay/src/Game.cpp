#include "Base.h"
#include "Game.h"
#include "Platform.h"
#include "RenderState.h"

// Extern global variables
GLenum __gl_error_code = GL_NO_ERROR;

namespace gameplay
{

static Game* __gameInstance = NULL;
long Game::_pausedTimeLast = 0L;
long Game::_pausedTimeTotal = 0L;

Game::Game() 
    : _initialized(false), _state(UNINITIALIZED), 
      _frameLastFPS(0), _frameCount(0), _frameRate(0), 
      _clearDepth(1.0f), _clearStencil(0),
      _animationController(NULL), _audioController(NULL)
{
    assert(__gameInstance == NULL);
    __gameInstance = this;
}

Game::Game(const Game& copy)
{
}

Game::~Game()
{
    // Do not call any virtual functions from the destructor.
    // Finalization is done from outside this class.

#ifdef GAMEPLAY_MEM_LEAK_DETECTION
    Ref::printLeaks();
    printMemoryLeaks();
#endif
}

Game* Game::getInstance()
{
    return __gameInstance;
}

long Game::getAbsoluteTime()
{
    return Platform::getAbsoluteTime();
}

long Game::getGameTime()
{
    return Platform::getAbsoluteTime() - _pausedTimeTotal;
}

void Game::setVsync(bool enable)
{
    Platform::setVsync(enable);
}

bool Game::isVsync()
{
    return Platform::isVsync();
}

int Game::run(int width, int height)
{
    if (_state != UNINITIALIZED)
        return -1;

    _width = width;
    _height = height;

    // Start up game systems.
    if (!startup())
    {
        shutdown();
        return -2;
    }
    return 0;
}

bool Game::startup()
{
    if (_state != UNINITIALIZED)
        return false;

    RenderState::initialize();

    _animationController = new AnimationController();
    _animationController->initialize();

    _audioController = new AudioController();
    _audioController->initialize();

    _physicsController = new PhysicsController();
    _physicsController->initialize();

    _state = RUNNING;

    return true;
}

void Game::shutdown()
{
    // Call user finalization.
    if (_state != UNINITIALIZED)
    {
        finalize();

        _animationController->finalize();
        SAFE_DELETE(_animationController);

        _audioController->finalize();
        SAFE_DELETE(_audioController);

        _physicsController->finalize();
        SAFE_DELETE(_physicsController);

        RenderState::finalize();
    }

    _state = UNINITIALIZED;
}

void Game::pause()
{
    if (_state == RUNNING)
    {
        _state = PAUSED;
        _pausedTimeLast = Platform::getAbsoluteTime();
        _animationController->pause();
        _audioController->pause();
        _physicsController->pause();
    }
}

void Game::resume()
{
    if (_state == PAUSED)
    {
        _state = RUNNING;
        _pausedTimeTotal += Platform::getAbsoluteTime() - _pausedTimeLast;
        _animationController->resume();
        _audioController->resume();
        _physicsController->resume();
    }
}

void Game::exit()
{
    shutdown();
}

void Game::frame()
{
    if (_state != RUNNING)
    {
        return;
    }
    else
    {
        if (!_initialized)
        {
            initialize();
            _initialized = true;
        }
    }

    // Update Time.
    static long lastFrameTime = Game::getGameTime();
    long frameTime = Game::getGameTime();
    long elapsedTime = (frameTime - lastFrameTime);
    lastFrameTime = frameTime;

    // Update the scheduled and running animations.
    _animationController->update(elapsedTime);
    // Update the physics.
    _physicsController->update(elapsedTime);
    // Application Update.
    update(elapsedTime);

    // Audio Rendering.
    _audioController->update(elapsedTime);
    // Graphics Rendering.
    render(elapsedTime);

    // Update FPS.
    ++_frameCount;
    if ((Game::getGameTime() - _frameLastFPS) >= 1000)
    {
        _frameRate = _frameCount;
        _frameCount = 0;
        _frameLastFPS = Game::getGameTime();
    }
}

void Game::clear(ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil)
{
    GLbitfield bits = 0;
    if (flags & CLEAR_COLOR)
    {
        if (clearColor.x != _clearColor.x ||
            clearColor.y != _clearColor.y ||
            clearColor.z != _clearColor.z ||
            clearColor.w != _clearColor.w )
        {
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            _clearColor.set(clearColor);
        }
        bits |= GL_COLOR_BUFFER_BIT;
    }

    if (flags & CLEAR_DEPTH)
    {
        if (clearDepth != _clearDepth)
        {
            glClearDepth(clearDepth);
            _clearDepth = clearDepth;
        }
        bits |= GL_DEPTH_BUFFER_BIT;

        // We need to explicitly call the static enableDepthWrite() method on StateBlock
        // to ensure depth writing is enabled before clearing the depth buffer (and to 
        // update the global StateBlock render state to reflect this).
        RenderState::StateBlock::enableDepthWrite();
    }

    if (flags & CLEAR_STENCIL)
    {
        if (clearStencil != _clearStencil)
        {
            glClearStencil(clearStencil);
            _clearStencil = clearStencil;
        }
        bits |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(bits);
}

void Game::menu()
{
}

void Game::keyEvent(Keyboard::KeyEvent evt, int key)
{
}

void Game::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool Game::mouseEvent(Mouse::MouseEvent evt, int x, int y)
{
    return false;
}

void Game::mouseWheelEvent(int x, int y, int delta)
{
}

}
