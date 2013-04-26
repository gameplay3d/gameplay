#include "Sample.h"
#include "SamplesGame.h"

const Game::State& Sample::UNINITIALIZED = Game::UNINITIALIZED;
const Game::State& Sample::RUNNING = Game::RUNNING;
const Game::State& Sample::PAUSED = Game::PAUSED;

const Game::ClearFlags& Sample::CLEAR_COLOR = Game::CLEAR_COLOR;
const Game::ClearFlags& Sample::CLEAR_DEPTH = Game::CLEAR_DEPTH;
const Game::ClearFlags& Sample::CLEAR_STENCIL = Game::CLEAR_STENCIL;
const Game::ClearFlags& Sample::CLEAR_COLOR_DEPTH = Game::CLEAR_COLOR_DEPTH;
const Game::ClearFlags& Sample::CLEAR_COLOR_STENCIL = Game::CLEAR_COLOR_STENCIL;
const Game::ClearFlags& Sample::CLEAR_DEPTH_STENCIL = Game::CLEAR_DEPTH_STENCIL;
const Game::ClearFlags& Sample::CLEAR_COLOR_DEPTH_STENCIL = Game::CLEAR_COLOR_DEPTH_STENCIL;

Sample::Sample()
{
}

Sample::~Sample()
{
	enableScriptCamera(false);
}

bool Sample::isVsync()
{
    return Game::isVsync();
}

void Sample::setVsync(bool enable)
{
    Game::setVsync(enable);
}

long Sample::getAbsoluteTime()
{
    return Game::getAbsoluteTime();
}

long Sample::getGameTime()
{
    return Game::getGameTime();
}

Game::State Sample::getState() const
{
    return Game::getInstance()->getState();
}

int Sample::run()
{
    return Game::getInstance()->run();
}

void Sample::pause()
{
    Game::getInstance()->pause();
}

void Sample::resume()
{
    Game::getInstance()->resume();
}

void Sample::exit()
{
    Game::getInstance()->exit();
}

void Sample::frame()
{
    Game::getInstance()->frame();
}

unsigned int Sample::getFrameRate() const
{
    return Game::getInstance()->getFrameRate();
}

const Rectangle& Sample::getViewport() const
{
    return Game::getInstance()->getViewport();
}

void Sample::setViewport(const Rectangle& viewport)
{
	Game::getInstance()->setViewport(viewport);
}

unsigned int Sample::getWidth() const
{
    return Game::getInstance()->getWidth();
}

unsigned int Sample::getHeight() const
{
    return Game::getInstance()->getHeight();
}

float Sample::getAspectRatio() const
{
    return Game::getInstance()->getAspectRatio();
}

void Sample::clear(Game::ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, clearColor, clearDepth, clearStencil);
}

void Sample::clear(Game::ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, red, green, blue, alpha, clearDepth, clearStencil);
}

AudioController* Sample::getAudioController() const
{
    return Game::getInstance()->getAudioController();
}

AnimationController* Sample::getAnimationController() const
{
    return Game::getInstance()->getAnimationController();
}

PhysicsController* Sample::getPhysicsController() const
{
    return Game::getInstance()->getPhysicsController();
}

ScriptController* Sample::getScriptController() const
{
	return Game::getInstance()->getScriptController();
}

void Sample::displayKeyboard(bool display)
{
    Game::getInstance()->displayKeyboard(display);
}

void Sample::keyEvent(Keyboard::KeyEvent evt, int key)
{    
}

void Sample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool Sample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

bool Sample::hasMouse()
{
    return Game::getInstance()->hasMouse();
}

bool Sample::isMouseCaptured()
{
    return Game::getInstance()->isMouseCaptured();
}

void Sample::setMouseCaptured(bool captured)
{
    Game::getInstance()->setMouseCaptured(captured);
}

void Sample::setMultiTouch(bool enabled)
{
    Game::getInstance()->setMultiTouch(enabled);
}

bool Sample::isMultiTouch() const
{
    return Game::getInstance()->isMultiTouch();
}

bool Sample::hasAccelerometer() const
{
    return Game::getInstance()->hasAccelerometer();
}

void Sample::getAccelerometerValues(float* pitch, float* roll)
{
    Game::getInstance()->getAccelerometerValues(pitch, roll);
}

void Sample::getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    Game::getInstance()->getRawSensorValues(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
}

void Sample::schedule(long timeOffset, TimeListener* timeListener, void* cookie)
{
    Game::getInstance()->schedule(timeOffset, timeListener, cookie);
}

void Sample::enableScriptCamera(bool enable)
{
	Game::getInstance()->getScriptController()->executeFunction<void>("camera_setActive", "b", enable);
}

void Sample::setScriptCameraSpeed(float normal, float fast)
{
    Game::getInstance()->getScriptController()->executeFunction<void>("camera_setSpeed", "ff", normal, fast);
}

bool Sample::isGestureSupported(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureSupported(evt);
}

void Sample::registerGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->registerGesture(evt);
}

void Sample::unregisterGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->unregisterGesture(evt);
}

bool Sample::isGestureRegistered(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureRegistered(evt);
}

void Sample::gestureSwipeEvent(int x, int y, int direction)
{
}

void Sample::gesturePinchEvent(int x, int y, float scale)
{
}

void Sample::gestureTapEvent(int x, int y)
{
}

void Sample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
}

unsigned int Sample::getGamepadCount() const
{
    return Game::getInstance()->getGamepadCount();
}

Gamepad* Sample::getGamepad(unsigned int index, bool preferPhysical) const
{
    return Game::getInstance()->getGamepad(index, preferPhysical);
}

void Sample::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->start();
    font->drawText(buffer, x, y, color, font->getSize());
    font->finish();
}
