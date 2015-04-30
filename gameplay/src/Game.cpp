#include "Base.h"
#include "Game.h"
#include "Platform.h"
#include "RenderState.h"
#include "FileSystem.h"
#include "FrameBuffer.h"
#include "SceneLoader.h"
#include "ControlFactory.h"
#include "Theme.h"
#include "Form.h"

/** @script{ignore} */
GLenum __gl_error_code = GL_NO_ERROR;
/** @script{ignore} */
ALenum __al_error_code = AL_NO_ERROR;

namespace gameplay
{

static Game* __gameInstance = NULL;
double Game::_pausedTimeLast = 0.0;
double Game::_pausedTimeTotal = 0.0;

/**
* @script{ignore}
*/
class GameScriptTarget : public ScriptTarget
{
    friend class Game;

    GP_SCRIPT_EVENTS_START();
    GP_SCRIPT_EVENT(initialize, "");
    GP_SCRIPT_EVENT(finalize, "");
    GP_SCRIPT_EVENT(update, "f");
    GP_SCRIPT_EVENT(render, "f");
    GP_SCRIPT_EVENT(resizeEvent, "ii");
    GP_SCRIPT_EVENT(keyEvent, "[Keyboard::KeyEvent]i");
    GP_SCRIPT_EVENT(touchEvent, "[Touch::TouchEvent]iiui");
    GP_SCRIPT_EVENT(mouseEvent, "[Mouse::MouseEvent]iii");
    GP_SCRIPT_EVENT(gestureSwipeEvent, "iii");
    GP_SCRIPT_EVENT(gesturePinchEvent, "iif");
    GP_SCRIPT_EVENT(gestureTapEvent, "ii");
    GP_SCRIPT_EVENT(gestureLongTapevent, "iif");
    GP_SCRIPT_EVENT(gestureDragEvent, "ii");
    GP_SCRIPT_EVENT(gestureDropEvent, "ii");
    GP_SCRIPT_EVENT(gamepadEvent, "[Gamepad::GamepadEvent]<Gamepad>");
    GP_SCRIPT_EVENTS_END();

public:

    GameScriptTarget()
    {
        GP_REGISTER_SCRIPT_EVENTS();
    }

    const char* getTypeName() const
    {
        return "GameScriptTarget";
    }
};

Game::Game()
    : _initialized(false), _state(UNINITIALIZED), _pausedCount(0),
      _frameLastFPS(0), _frameCount(0), _frameRate(0), _width(0), _height(0),
      _clearDepth(1.0f), _clearStencil(0), _properties(NULL),
      _animationController(NULL), _audioController(NULL),
      _physicsController(NULL), _aiController(NULL), _audioListener(NULL),
      _timeEvents(NULL), _scriptController(NULL), _scriptTarget(NULL)
{
    GP_ASSERT(__gameInstance == NULL);

    __gameInstance = this;
    _timeEvents = new std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >();
}

Game::~Game()
{
    SAFE_DELETE(_scriptTarget);
	SAFE_DELETE(_scriptController);

    // Do not call any virtual functions from the destructor.
    // Finalization is done from outside this class.
    SAFE_DELETE(_timeEvents);
#ifdef GP_USE_MEM_LEAK_DETECTION
    Ref::printLeaks();
    printMemoryLeaks();
#endif

    __gameInstance = NULL;
}

Game* Game::getInstance()
{
    GP_ASSERT(__gameInstance);
    return __gameInstance;
}

void Game::initialize()
{
    // stub
}

void Game::finalize()
{
    // stub
}

void Game::update(float elapsedTime)
{
    // stub
}

void Game::render(float elapsedTime)
{
    // stub
}

double Game::getAbsoluteTime()
{
    return Platform::getAbsoluteTime();
}

double Game::getGameTime()
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

int Game::run()
{
    if (_state != UNINITIALIZED)
        return -1;

    loadConfig();

    _width = Platform::getDisplayWidth();
    _height = Platform::getDisplayHeight();

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

    setViewport(Rectangle(0.0f, 0.0f, (float)_width, (float)_height));
    RenderState::initialize();
    FrameBuffer::initialize();

    _animationController = new AnimationController();
    _animationController->initialize();

    _audioController = new AudioController();
    _audioController->initialize();

    _physicsController = new PhysicsController();
    _physicsController->initialize();

    _aiController = new AIController();
    _aiController->initialize();

    _scriptController = new ScriptController();
    _scriptController->initialize();

    // Load any gamepads, ui or physical.
    loadGamepads();

    // Set script handler
    if (_properties)
    {
        const char* scriptPath = _properties->getString("script");
        if (scriptPath)
        {
            _scriptTarget = new GameScriptTarget();
            _scriptTarget->addScript(scriptPath);
        }
        else
        {
            // Use the older scripts namespace for loading individual global script callback functions.
            Properties* sns = _properties->getNamespace("scripts", true);
            if (sns)
            {
                _scriptTarget = new GameScriptTarget();

                // Define a macro to simplify defining the following script callback registrations
                #define GP_REG_GAME_SCRIPT_CB(e) if (sns->exists(#e)) _scriptTarget->addScriptCallback(GP_GET_SCRIPT_EVENT(GameScriptTarget, e), sns->getString(#e))

                // Register all supported script callbacks if they are defined
                GP_REG_GAME_SCRIPT_CB(initialize);
                GP_REG_GAME_SCRIPT_CB(finalize);
                GP_REG_GAME_SCRIPT_CB(update);
                GP_REG_GAME_SCRIPT_CB(render);
                GP_REG_GAME_SCRIPT_CB(resizeEvent);
                GP_REG_GAME_SCRIPT_CB(keyEvent);
                GP_REG_GAME_SCRIPT_CB(touchEvent);
                GP_REG_GAME_SCRIPT_CB(mouseEvent);
                GP_REG_GAME_SCRIPT_CB(gestureSwipeEvent);
                GP_REG_GAME_SCRIPT_CB(gesturePinchEvent);
                GP_REG_GAME_SCRIPT_CB(gestureTapEvent);
                GP_REG_GAME_SCRIPT_CB(gestureLongTapevent);
                GP_REG_GAME_SCRIPT_CB(gestureDragEvent);
                GP_REG_GAME_SCRIPT_CB(gestureDropEvent);
                GP_REG_GAME_SCRIPT_CB(gamepadEvent);
            }
        }
    }

    _state = RUNNING;

    return true;
}

void Game::shutdown()
{
    // Call user finalization.
    if (_state != UNINITIALIZED)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);

        Platform::signalShutdown();

		// Call user finalize
        finalize();

        // Call script finalize
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, finalize));

        // Destroy script target so no more script events are fired
        SAFE_DELETE(_scriptTarget);

		// Shutdown scripting system first so that any objects allocated in script are released before our subsystems are released
		_scriptController->finalize();

        unsigned int gamepadCount = Gamepad::getGamepadCount();
        for (unsigned int i = 0; i < gamepadCount; i++)
        {
            Gamepad* gamepad = Gamepad::getGamepad(i, false);
            SAFE_DELETE(gamepad);
        }

        _animationController->finalize();
        SAFE_DELETE(_animationController);

        _audioController->finalize();
        SAFE_DELETE(_audioController);

        _physicsController->finalize();
        SAFE_DELETE(_physicsController);
        _aiController->finalize();
        SAFE_DELETE(_aiController);
        
        ControlFactory::finalize();

        Theme::finalize();

        // Note: we do not clean up the script controller here
        // because users can call Game::exit() from a script.

        SAFE_DELETE(_audioListener);

        FrameBuffer::finalize();
        RenderState::finalize();

        SAFE_DELETE(_properties);

		_state = UNINITIALIZED;
    }
}

void Game::pause()
{
    if (_state == RUNNING)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);
        _state = PAUSED;
        _pausedTimeLast = Platform::getAbsoluteTime();
        _animationController->pause();
        _audioController->pause();
        _physicsController->pause();
        _aiController->pause();
    }

    ++_pausedCount;
}

void Game::resume()
{
    if (_state == PAUSED)
    {
        --_pausedCount;

        if (_pausedCount == 0)
        {
            GP_ASSERT(_animationController);
            GP_ASSERT(_audioController);
            GP_ASSERT(_physicsController);
            GP_ASSERT(_aiController);
            _state = RUNNING;
            _pausedTimeTotal += Platform::getAbsoluteTime() - _pausedTimeLast;
            _animationController->resume();
            _audioController->resume();
            _physicsController->resume();
            _aiController->resume();
        }
    }
}

void Game::exit()
{
    // Only perform a full/clean shutdown if GP_USE_MEM_LEAK_DETECTION is defined.
	// Every modern OS is able to handle reclaiming process memory hundreds of times
	// faster than it would take us to go through every pointer in the engine and
	// release them nicely. For large games, shutdown can end up taking long time,
    // so we'll just call ::exit(0) to force an instant shutdown.

#ifdef GP_USE_MEM_LEAK_DETECTION

    // Schedule a call to shutdown rather than calling it right away.
	// This handles the case of shutting down the script system from
	// within a script function (which can cause errors).
	static ShutdownListener listener;
	schedule(0, &listener);

#else

    // End the process immediately without a full shutdown
    ::exit(0);

#endif
}


void Game::frame()
{
    if (!_initialized)
    {
        // Perform lazy first time initialization
        initialize();
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, initialize));
        _initialized = true;

        // Fire first game resize event
        Platform::resizeEventInternal(_width, _height);
    }

	static double lastFrameTime = Game::getGameTime();
	double frameTime = getGameTime();

    // Fire time events to scheduled TimeListeners
    fireTimeEvents(frameTime);

    if (_state == Game::RUNNING)
    {
        GP_ASSERT(_animationController);
        GP_ASSERT(_audioController);
        GP_ASSERT(_physicsController);
        GP_ASSERT(_aiController);

        // Update Time.
        float elapsedTime = (frameTime - lastFrameTime);
        lastFrameTime = frameTime;

        // Update the scheduled and running animations.
        _animationController->update(elapsedTime);

        // Update the physics.
        _physicsController->update(elapsedTime);

        // Update AI.
        _aiController->update(elapsedTime);

        // Update gamepads.
        Gamepad::updateInternal(elapsedTime);

        // Application Update.
        update(elapsedTime);

        // Update forms.
        Form::updateInternal(elapsedTime);

        // Run script update.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, update), elapsedTime);

        // Audio Rendering.
        _audioController->update(elapsedTime);

        // Graphics Rendering.
        render(elapsedTime);

        // Run script render.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, render), elapsedTime);

        // Update FPS.
        ++_frameCount;
        if ((Game::getGameTime() - _frameLastFPS) >= 1000)
        {
            _frameRate = _frameCount;
            _frameCount = 0;
            _frameLastFPS = getGameTime();
        }
    }
	else if (_state == Game::PAUSED)
    {
        // Update gamepads.
        Gamepad::updateInternal(0);

        // Application Update.
        update(0);

        // Update forms.
        Form::updateInternal(0);

        // Script update.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, update), 0);

        // Graphics Rendering.
        render(0);

        // Script render.
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, render), 0);
    }
}

void Game::renderOnce(const char* function)
{
    _scriptController->executeFunction<void>(function, NULL);
    Platform::swapBuffers();
}

void Game::updateOnce()
{
    GP_ASSERT(_animationController);
    GP_ASSERT(_audioController);
    GP_ASSERT(_physicsController);
    GP_ASSERT(_aiController);

    // Update Time.
    static double lastFrameTime = getGameTime();
    double frameTime = getGameTime();
    float elapsedTime = (frameTime - lastFrameTime);
    lastFrameTime = frameTime;

    // Update the internal controllers.
    _animationController->update(elapsedTime);
    _physicsController->update(elapsedTime);
    _aiController->update(elapsedTime);
    _audioController->update(elapsedTime);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, update), elapsedTime);
}

void Game::setViewport(const Rectangle& viewport)
{
    _viewport = viewport;
    glViewport((GLuint)viewport.x, (GLuint)viewport.y, (GLuint)viewport.width, (GLuint)viewport.height);
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

void Game::clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil)
{
    clear(flags, Vector4(red, green, blue, alpha), clearDepth, clearStencil);
}

AudioListener* Game::getAudioListener()
{
    if (_audioListener == NULL)
    {
        _audioListener = new AudioListener();
    }
    return _audioListener;
}

void Game::keyEvent(Keyboard::KeyEvent evt, int key)
{
    // stub
}

void Game::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // stub
}

bool Game::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    // stub
    return false;
}

void Game::resizeEvent(unsigned int width, unsigned int height)
{
    // stub
}

bool Game::isGestureSupported(Gesture::GestureEvent evt)
{
    return Platform::isGestureSupported(evt);
}

void Game::registerGesture(Gesture::GestureEvent evt)
{
    Platform::registerGesture(evt);
}

void Game::unregisterGesture(Gesture::GestureEvent evt)
{
    Platform::unregisterGesture(evt);
}

bool Game::isGestureRegistered(Gesture::GestureEvent evt)
{
    return Platform::isGestureRegistered(evt);
}

void Game::gestureSwipeEvent(int x, int y, int direction)
{
    // stub
}

void Game::gesturePinchEvent(int x, int y, float scale)
{
    // stub
}

void Game::gestureTapEvent(int x, int y)
{
    // stub
}

void Game::gestureLongTapEvent(int x, int y, float duration)
{
    // stub
}

void Game::gestureDragEvent(int x, int y)
{
    // stub
}

void Game::gestureDropEvent(int x, int y)
{
    // stub
}

void Game::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    // stub
}

void Game::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    keyEvent(evt, key);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, keyEvent), evt, key);
}

void Game::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    touchEvent(evt, x, y, contactIndex);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, touchEvent), evt, x, y, contactIndex);
}

bool Game::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (mouseEvent(evt, x, y, wheelDelta))
        return true;

    if (_scriptTarget)
        return _scriptTarget->fireScriptEvent<bool>(GP_GET_SCRIPT_EVENT(GameScriptTarget, mouseEvent), evt, x, y, wheelDelta);

    return false;
}

void Game::resizeEventInternal(unsigned int width, unsigned int height)
{
    // Update the width and height of the game
    if (_width != width || _height != height)
    {
        _width = width;
        _height = height;
        resizeEvent(width, height);
        if (_scriptTarget)
            _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, resizeEvent), width, height);
    }
}

void Game::gestureSwipeEventInternal(int x, int y, int direction)
{
    gestureSwipeEvent(x, y, direction);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureSwipeEvent), x, y, direction);
}

void Game::gesturePinchEventInternal(int x, int y, float scale)
{
    gesturePinchEvent(x, y, scale);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gesturePinchEvent), x, y, scale);
}

void Game::gestureTapEventInternal(int x, int y)
{
    gestureTapEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureTapEvent), x, y);
}

void Game::gestureLongTapEventInternal(int x, int y, float duration)
{
    gestureLongTapEvent(x, y, duration);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureLongTapevent), x, y, duration);
}

void Game::gestureDragEventInternal(int x, int y)
{
    gestureDragEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureDragEvent), x, y);
}

void Game::gestureDropEventInternal(int x, int y)
{
    gestureDropEvent(x, y);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gestureDropEvent), x, y);
}

void Game::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    gamepadEvent(evt, gamepad);
    if (_scriptTarget)
        _scriptTarget->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(GameScriptTarget, gamepadEvent), evt, gamepad);
}

void Game::getArguments(int* argc, char*** argv) const
{
    Platform::getArguments(argc, argv);
}

void Game::schedule(float timeOffset, TimeListener* timeListener, void* cookie)
{
    GP_ASSERT(_timeEvents);
    TimeEvent timeEvent(getGameTime() + timeOffset, timeListener, cookie);
    _timeEvents->push(timeEvent);
}

void Game::schedule(float timeOffset, const char* function)
{
    getScriptController()->schedule(timeOffset, function);
}

void Game::clearSchedule()
{
    SAFE_DELETE(_timeEvents);
    _timeEvents = new std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >();
}

void Game::fireTimeEvents(double frameTime)
{
    while (_timeEvents->size() > 0)
    {
        const TimeEvent* timeEvent = &_timeEvents->top();
        if (timeEvent->time > frameTime)
        {
            break;
        }
        if (timeEvent->listener)
        {
            timeEvent->listener->timeEvent(frameTime - timeEvent->time, timeEvent->cookie);
        }
        _timeEvents->pop();
    }
}

Game::TimeEvent::TimeEvent(double time, TimeListener* timeListener, void* cookie)
    : time(time), listener(timeListener), cookie(cookie)
{
}

bool Game::TimeEvent::operator<(const TimeEvent& v) const
{
    // The first element of std::priority_queue is the greatest.
    return time > v.time;
}

Properties* Game::getConfig() const
{
    if (_properties == NULL)
        const_cast<Game*>(this)->loadConfig();

    return _properties;
}

void Game::loadConfig()
{
    if (_properties == NULL)
    {
        // Try to load custom config from file.
        if (FileSystem::fileExists("game.config"))
        {
            _properties = Properties::create("game.config");

            // Load filesystem aliases.
            Properties* aliases = _properties->getNamespace("aliases", true);
            if (aliases)
            {
                FileSystem::loadResourceAliases(aliases);
            }
        }
        else
        {
            // Create an empty config
            _properties = new Properties();
        }
    }
}

void Game::loadGamepads()
{
    // Load virtual gamepads.
    if (_properties)
    {
        // Check if there are any virtual gamepads included in the .config file.
        // If there are, create and initialize them.
        _properties->rewind();
        Properties* inner = _properties->getNextNamespace();
        while (inner != NULL)
        {
            std::string spaceName(inner->getNamespace());
            // This namespace was accidentally named "gamepads" originally but we'll keep this check
            // for backwards compatibility.
            if (spaceName == "gamepads" || spaceName == "gamepad")
            {
                if (inner->exists("form"))
                {
                    const char* gamepadFormPath = inner->getString("form");
                    GP_ASSERT(gamepadFormPath);
                    Gamepad* gamepad = Gamepad::add(gamepadFormPath);
                    GP_ASSERT(gamepad);
                }
            }

            inner = _properties->getNextNamespace();
        }
    }
}

void Game::ShutdownListener::timeEvent(long timeDiff, void* cookie)
{
	Game::getInstance()->shutdown();
}

}

