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
      _animationController(NULL), _audioController(NULL), _physicsController(NULL), _audioListener(NULL)
{
    assert(__gameInstance == NULL);
    __gameInstance = this;
    _timeEvents = new std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >();
}

Game::Game(const Game& copy)
{
}

Game::~Game()
{
    // Do not call any virtual functions from the destructor.
    // Finalization is done from outside this class.
    delete _timeEvents;
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

    if (width == -1)
        _width = Platform::getDisplayWidth();
    else
        _width = width;
    
    if (height == -1)
        _height = Platform::getDisplayHeight();
    else
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

        SAFE_DELETE(_audioListener);

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
    if (!_initialized)
    {
        initialize();
        _initialized = true;
    }

    if (_state == Game::RUNNING)
    {
        // Update Time.
        static long lastFrameTime = Game::getGameTime();
        long frameTime = Game::getGameTime();
        long elapsedTime = (frameTime - lastFrameTime);
        lastFrameTime = frameTime;

        // Update the scheduled and running animations.
        _animationController->update(elapsedTime);

        // Fire time events to scheduled TimeListeners
        fireTimeEvents(frameTime);
    
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
    else
    {
        // Application Update.
        update(0);

        // Graphics Rendering.
        render(0);
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

AudioListener* Game::getAudioListener()
{
    if (_audioListener == NULL)
    {
        _audioListener = new AudioListener();
    }
    return _audioListener;
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

void Game::schedule(long timeOffset, TimeListener* timeListener, void* cookie)
{
    assert(timeListener);
    TimeEvent timeEvent(getGameTime() + timeOffset, timeListener, cookie);
    _timeEvents->push(timeEvent);
}

bool Game::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

void Game::updateOnce()
{
    // Update Time.
    static long lastFrameTime = Game::getGameTime();
    long frameTime = Game::getGameTime();
    long elapsedTime = (frameTime - lastFrameTime);
    lastFrameTime = frameTime;

    // Update the internal controllers.
    _animationController->update(elapsedTime);
    _physicsController->update(elapsedTime);
    _audioController->update(elapsedTime);
}

void Game::fireTimeEvents(long frameTime)
{
    while (_timeEvents->size() > 0)
    {
        const TimeEvent* timeEvent = &_timeEvents->top();
        if (timeEvent->time > frameTime)
        {
            break;
        }
        timeEvent->listener->timeEvent(frameTime - timeEvent->time, timeEvent->cookie);
        _timeEvents->pop();
    }
}

Game::TimeEvent::TimeEvent(long time, TimeListener* timeListener, void* cookie)
            : time(time), listener(timeListener), cookie(cookie)
{
}

bool Game::TimeEvent::operator<(const TimeEvent& v) const
{
    // The first element of std::priority_queue is the greatest.
    return time > v.time;
}

}
