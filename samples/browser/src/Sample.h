#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Base class for all of the samples. 
 *
 * The goal is to provide an interface that matches the Game class.
 * This way you can easily copy the code from your sample into a game.
 */
class Sample
{
    friend class SamplesGame;

public:

    static const Game::State& UNINITIALIZED;
    static const Game::State& RUNNING;
    static const Game::State& PAUSED;
    static const Game::ClearFlags& CLEAR_COLOR;
    static const Game::ClearFlags& CLEAR_DEPTH;
    static const Game::ClearFlags& CLEAR_STENCIL;
    static const Game::ClearFlags& CLEAR_COLOR_DEPTH;
    static const Game::ClearFlags& CLEAR_COLOR_STENCIL;
    static const Game::ClearFlags& CLEAR_DEPTH_STENCIL;
    static const Game::ClearFlags& CLEAR_COLOR_DEPTH_STENCIL;

    static bool isVsync();
    static void setVsync(bool enable);
    static long getAbsoluteTime();
    static long getGameTime();
    Game::State getState() const;
    int run();
    void pause();
    void resume();
    void exit();
    void frame();
    unsigned int getFrameRate() const;
    const Rectangle& getViewport() const;
	void setViewport(const Rectangle& viewport);
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    float getAspectRatio() const;
    void clear(Game::ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil);
    void clear(Game::ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil);
    AudioController* getAudioController() const;
    AnimationController* getAnimationController() const;
    PhysicsController* getPhysicsController() const;
    ScriptController* getScriptController() const;
    void displayKeyboard(bool display);
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    bool hasMouse();
    bool isMouseCaptured();
    void setMouseCaptured(bool captured);
    void setMultiTouch(bool enabled);
    bool isMultiTouch() const;
    bool hasAccelerometer() const;
    void getAccelerometerValues(float* pitch, float* roll);
    void getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ);
    void schedule(long timeOffset, TimeListener* timeListener, void* cookie = 0);
	void enableScriptCamera(bool enable);
    void setScriptCameraSpeed(float normal, float fast);
    bool isGestureSupported(Gesture::GestureEvent evt);
    void registerGesture(Gesture::GestureEvent evt);
    void unregisterGesture(Gesture::GestureEvent evt);
    bool isGestureRegistered(Gesture::GestureEvent evt);
    virtual void gestureSwipeEvent(int x, int y, int direction);
    virtual void gesturePinchEvent(int x, int y, float scale);
    virtual void gestureTapEvent(int x, int y);
    virtual void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);
    unsigned int getGamepadCount() const;
    Gamepad* getGamepad(unsigned int index, bool preferPhysical = true) const;

protected:

    Sample();
    virtual ~Sample();
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void render(float elapsedTime) = 0;
    static void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

private:

    Sample(const Sample&); 
    Sample& operator=(const Sample&);
};

#endif
