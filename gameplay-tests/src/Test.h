#ifndef Test_H_
#define Test_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Base class for all of the tests. The goal is to provide an interface that matches the Game class.
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
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    float getAspectRatio() const;
    void clear(Game::ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil);
    void clear(Game::ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil);
    AudioController* getAudioController() const;
    AnimationController* getAnimationController() const;
    PhysicsController* getPhysicsController() const;
    void displayKeyboard(bool display);
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    void setMultiTouch(bool enabled);
    bool isMultiTouch() const;
    void getAccelerometerValues(float* pitch, float* roll);
    void schedule(long timeOffset, TimeListener* timeListener, void* cookie = 0);

protected:

    Test();

    virtual ~Test();

    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void render(float elapsedTime) = 0;

    static void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

private:

    Test(const Test&);            // Hidden copy constructor.
    Test& operator=(const Test&); // Hidden copy assignment operator.

private:
    
};

#endif
