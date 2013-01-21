#ifndef TEST_H_
#define TEST_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Base class for all of the tests. 
 *
 * The goal is to provide an interface that matches the Game class.
 * This way you can easily copy the code from your test into a game.
 */
class Test
{
    friend class TestsGame;

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
    void getAccelerometerValues(float* pitch, float* roll);
    void schedule(long timeOffset, TimeListener* timeListener, void* cookie = 0);
	void enableScriptCamera(bool enable);
    void setScriptCameraSpeed(float normal, float fast);

    // gestures
    bool isGestureSupported(Gesture::GestureEvent evt);
    void registerGesture(Gesture::GestureEvent evt);
    void unregisterGesture(Gesture::GestureEvent evt);
    bool isGestureRegistered(Gesture::GestureEvent evt);
    virtual void gestureSwipeEvent(int x, int y, int direction);
    virtual void gesturePinchEvent(int x, int y, float scale);
    virtual void gestureTapEvent(int x, int y);

    // gamepad
    virtual void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);
    unsigned int getGamepadCount() const;
    Gamepad* getGamepad(unsigned int index) const;

protected:

    Test();
    virtual ~Test();
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void render(float elapsedTime) = 0;
    static void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

private:

    Test(const Test&); 
    Test& operator=(const Test&);
};

#endif
