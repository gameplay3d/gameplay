#include "Test.h"
#include "TestsGame.h"

const Game::State& Test::UNINITIALIZED = Game::UNINITIALIZED;
const Game::State& Test::RUNNING = Game::RUNNING;
const Game::State& Test::PAUSED = Game::PAUSED;

const Game::ClearFlags& Test::CLEAR_COLOR = Game::CLEAR_COLOR;
const Game::ClearFlags& Test::CLEAR_DEPTH = Game::CLEAR_DEPTH;
const Game::ClearFlags& Test::CLEAR_STENCIL = Game::CLEAR_STENCIL;
const Game::ClearFlags& Test::CLEAR_COLOR_DEPTH = Game::CLEAR_COLOR_DEPTH;
const Game::ClearFlags& Test::CLEAR_COLOR_STENCIL = Game::CLEAR_COLOR_STENCIL;
const Game::ClearFlags& Test::CLEAR_DEPTH_STENCIL = Game::CLEAR_DEPTH_STENCIL;
const Game::ClearFlags& Test::CLEAR_COLOR_DEPTH_STENCIL = Game::CLEAR_COLOR_DEPTH_STENCIL;

Test::Test()
{
}

Test::~Test()
{
}

bool Test::isVsync()
{
    return Game::isVsync();
}

void Test::setVsync(bool enable)
{
    Game::setVsync(enable);
}

long Test::getAbsoluteTime()
{
    return Game::getAbsoluteTime();
}

long Test::getGameTime()
{
    return Game::getGameTime();
}

Game::State Test::getState() const
{
    return Game::getInstance()->getState();
}

int Test::run()
{
    return Game::getInstance()->run();
}

void Test::pause()
{
    Game::getInstance()->pause();
}

void Test::resume()
{
    Game::getInstance()->resume();
}

void Test::exit()
{
    Game::getInstance()->exit();
}

void Test::frame()
{
    Game::getInstance()->frame();
}

unsigned int Test::getFrameRate() const
{
    return Game::getInstance()->getFrameRate();
}

const Rectangle& Test::getViewport() const
{
    return Game::getInstance()->getViewport();
}

void Test::setViewport(const Rectangle& viewport)
{
	Game::getInstance()->setViewport(viewport);
}

unsigned int Test::getWidth() const
{
    return Game::getInstance()->getWidth();
}

unsigned int Test::getHeight() const
{
    return Game::getInstance()->getHeight();
}

float Test::getAspectRatio() const
{
    return Game::getInstance()->getAspectRatio();
}

void Test::clear(Game::ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, clearColor, clearDepth, clearStencil);
}

void Test::clear(Game::ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, red, green, blue, alpha, clearDepth, clearStencil);
}

AudioController* Test::getAudioController() const
{
    return Game::getInstance()->getAudioController();
}

AnimationController* Test::getAnimationController() const
{
    return Game::getInstance()->getAnimationController();
}

PhysicsController* Test::getPhysicsController() const
{
    return Game::getInstance()->getPhysicsController();
}

void Test::displayKeyboard(bool display)
{
    Game::getInstance()->displayKeyboard(display);
}

void Test::keyEvent(Keyboard::KeyEvent evt, int key)
{    
}

void Test::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool Test::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

bool Test::hasMouse()
{
    return Game::getInstance()->hasMouse();
}

bool Test::isMouseCaptured()
{
    return Game::getInstance()->isMouseCaptured();
}

void Test::setMouseCaptured(bool captured)
{
    Game::getInstance()->setMouseCaptured(captured);
}

void Test::setMultiTouch(bool enabled)
{
    Game::getInstance()->setMultiTouch(enabled);
}

bool Test::isMultiTouch() const
{
    return Platform::isMultiTouch();
}

void Test::getAccelerometerValues(float* pitch, float* roll)
{
    Game::getInstance()->getAccelerometerValues(pitch, roll);
}

void Test::schedule(long timeOffset, TimeListener* timeListener, void* cookie)
{
    Game::getInstance()->schedule(timeOffset, timeListener, cookie);
}

bool Test::isGestureSupported(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureSupported(evt);
}

void Test::registerGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->registerGesture(evt);
}

void Test::unregisterGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->unregisterGesture(evt);
}

bool Test::isGestureRegistered(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureRegistered(evt);
}

void Test::gestureSwipeEvent(int x, int y, int direction)
{
}

void Test::gesturePinchEvent(int x, int y, float scale)
{
}

void Test::gestureTapEvent(int x, int y)
{
}

void Test::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
}

void Test::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->start();
    font->drawText(buffer, x, y, color, font->getSize());
    font->finish();
}