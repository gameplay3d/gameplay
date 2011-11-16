/*
 * Game.h
 */

#ifndef GAME_H_
#define GAME_H_

#include "Input.h"
#include "AudioController.h"
#include "AnimationController.h"
#include "Vector4.h"

namespace gameplay
{

/**
 * Defines the basic game initialization, logic and platform delegates.
 */
class Game
{
public:

    /**
     * The game states.
     */
    enum State
    {
        UNINITIALIZED,
        RUNNING,
        PAUSED
    };

    /**
     * Flags used when clearing the active frame buffer targets.
     */
    enum ClearFlags
    {
        CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
        CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
        CLEAR_STENCIL = GL_STENCIL_BUFFER_BIT,
        CLEAR_COLOR_DEPTH = CLEAR_COLOR | CLEAR_DEPTH,
        CLEAR_COLOR_STENCIL = CLEAR_COLOR | CLEAR_STENCIL,
        CLEAR_DEPTH_STENCIL = CLEAR_DEPTH | CLEAR_STENCIL,
        CLEAR_COLOR_DEPTH_STENCIL = CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL
    };

    /**
     * Destructor.
     */
    virtual ~Game();

    /**
     * Gets the single instance of the game.
     * 
     * @return The single instance of the game.
     */
    static Game* getInstance();

    /**
     * Gets the total absolute running time (in milliseconds) since Game::run().
     * 
     * @return The total absolute running time (in milliseconds).
     */
    static long getAbsoluteTime();

    /**
     * Gets whether vertical sync is enabled for the game display.
     * 
     * @return true if vsync is enabled; false if not.
     */
    static bool isVsync();

    /**
     * Sets whether vertical sync is enabled for the game display.
     *
     * @param enable true if vsync is enabled; false if not.
     */
    static void setVsync(bool enable);

    /**
     * Gets the total game time (in milliseconds). This is the total accumulated game time (unpaused).
     *
     * You would typically use things in your game that you want to stop when the game is paused.
     * This includes things such as game physics and animation.
     * 
     * @return The total game time (in milliseconds).
     */
    static long getGameTime();

    /**
     * Gets the game state.
     *
     * @return The current game state.
     */
    State getState() const;

    /**
     * Call this method to initialize the game, and begin running the game.
     *
     * @param width The width of the game window to run at.
     * @param height The height of the game window to run at.
     * 
     * @return Zero for normal termination, or non-zero if an error occurred.
     */
    int run(int width = -1, int height = -1);

    /**
     * Pauses the game after being run.
     */
    void pause();

    /**
     * Resumes the game after being paused.
     */
    void resume();

    /**
     * Exits the game.
     */
    void exit();

    /**
     * Platform frame delagate.
     *
     * This is called every frame from the platform.
     * This in turn calls back on the user implemented game methods: update() then render()
     */
    void frame();

    /**
     * Gets the current frame rate.
     * 
     * @return The current frame rate.
     */
    unsigned int getFrameRate() const;

    /**
     * Gets the game window width.
     * 
     * @return The game window width.
     */
    unsigned int getWidth() const;

    /**
     * Gets the game window height.
     * 
     * @return The game window height.
     */
    unsigned int getHeight() const;

    /**
     * Clears the specified resource buffers to the specified clear values. 
     *
     * @param flags The flags indicating which buffers to be cleared.
     * @param clearColor The color value to clear to when the flags includes the color buffer.
     * @param clearDepth The depth value to clear to when the flags includes the color buffer.
     * @param clearStencil The stencil value to clear to when the flags includes the color buffer.
     */
    void clear(ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil);

    /**
     * Gets the audio controller for managing control of audio
     * associated with the game.
     *
     * @return The audio controller for this game.
     */
    const AudioController* getAudioController() const;

    /**
     * Gets the animation controller for managing control of animations
     * associated with the game.
     * 
     * @return The animation controller for this game.
     */
    AnimationController* getAnimationController();

    /**
     * Menu callback on menu events.
     */
    virtual void menu();

    /**
     * Input callback on keyChar events.
     *
     * @param key The key code pressed.
     */
    virtual void keyChar(char key);

    /**
     * Input callback on keyPress events.
     *
     * @param key The key code pressed.
     * @param keyEvent The key event that occured.
     * 
     * @see Input::Key
     * @see Input::KeyEvent
     */
    virtual void keyPress(int key, int keyEvent);

    /**
     * Input callback on touch events.
     *
     * @param x The x position of the touch.
     * @param y The y position of the touch.
     * @param touchEvent The touch event that occurred.
     * 
     * @see Input::TouchEvent
     */
    virtual void touch(int x, int y, int touchEvent);

protected:

    /**
     * Constructor.
     */
    Game();

    /**
     * Initializes the game on startup.
     */
    virtual void initialize() = 0;

    /**
     * Finalizes the game on exit.
     */
    virtual void finalize() = 0;

    /**
     * Update callback for handling update routines.
     *
     * Called just before render, once per frame when game is running.
     * Ideal for non-render code and game logic such as input and animation.
     *
     * @param elapsedTime The elapsed game time.
     */
    virtual void update(long elapsedTime) = 0;

    /**
     * Render callback for handling rendering routines.
     *
     * Called just after update, once per frame when game is running.
     * Ideal for all rendering code.
     *
     * @param elapsedTime The elapsed game time.
     */
    virtual void render(long elapsedTime) = 0;

private:

    /**
     * Constructor.
     *
     * @param copy The game to copy.
     */
    Game(const Game& copy);

    /**
     * Starts core game.
     */
    bool startup();

    /**
     * Shuts down the game.
     */
    void shutdown();

    static long _pausedTimeLast;                // The last time paused.
    static long _pausedTimeTotal;               // The total time paused.
    State _state;                               // The game state.
    long _frameLastFPS;                         // The last time the frame count was updated.
    unsigned int _frameCount;                   // The current frame count.
    unsigned int _frameRate;                    // The current frame rate.
    unsigned int _width;                        // The game's display width.
    unsigned int _height;                       // The game's display height.
    Vector4 _clearColor;                        // The clear color value last used for clearing the color buffer.
    float _clearDepth;                          // The clear depth value last used for clearing the depth buffer.
    int _clearStencil;                          // The clear stencil value last used for clearing the stencil buffer.
    AnimationController* _animationController;  // Controls the scheduling and running of animations.
    AudioController* _audioController;          // Controls audio sources that are playing in the game.
};

}

#endif
