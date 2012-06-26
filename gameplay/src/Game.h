#ifndef GAME_H_
#define GAME_H_

#include <queue>

#include "Keyboard.h"
#include "Touch.h"
#include "Mouse.h"
#include "AudioController.h"
#include "AnimationController.h"
#include "PhysicsController.h"
#include "AudioListener.h"
#include "Rectangle.h"
#include "Vector4.h"
#include "TimeListener.h"
#include "Gamepad.h"

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
     * Gets the total absolute running time (in milliseconds) since Game::run().
     * 
     * @return The total absolute running time (in milliseconds).
     */
    static double getAbsoluteTime();

    /**
     * Gets the total game time (in milliseconds). This is the total accumulated game time (unpaused).
     *
     * You would typically use things in your game that you want to stop when the game is paused.
     * This includes things such as game physics and animation.
     * 
     * @return The total game time (in milliseconds).
     */
    static double getGameTime();

    /**
     * Gets the game state.
     *
     * @return The current game state.
     */
    inline State getState() const;

    /**
     * Returns the game configuration object.
     *
     * This method returns a Properties object containing the contents
     * of the game.config file.
     *
     * @return The game conifguration Properties object.
     */
    Properties* getConfig() const;

    /**
     * Called to initialize the game, and begin running the game.
     * 
     * @return Zero for normal termination, or non-zero if an error occurred.
     */
    int run();

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
    inline unsigned int getFrameRate() const;

    /**
     * Gets the game window width.
     * 
     * @return The game window width.
     */
    inline unsigned int getWidth() const;

    /**
     * Gets the game window height.
     * 
     * @return The game window height.
     */
    inline unsigned int getHeight() const;

    /**
     * Gets the game current viewport.
     *
     * The default viewport is Rectangle(0, 0, Game::getWidth(), Game::getHeight()).
     */
    inline const Rectangle& getViewport() const;

    /**
     * Set the game current viewport.
     *
     * The x, y, width and height of the viewport must all be positive.
     *
     * viewport The custom viewport to be set on the game.
     */
    void setViewport(const Rectangle& viewport);

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
    inline AudioController* getAudioController() const;

    /**
     * Gets the animation controller for managing control of animations
     * associated with the game.
     * 
     * @return The animation controller for this game.
     */
    inline AnimationController* getAnimationController() const;

    /**
     * Gets the physics controller for managing control of physics
     * associated with the game.
     * 
     * @return The physics controller for this game.
     */
    inline PhysicsController* getPhysicsController() const;

    /**
     * Gets the audio listener for 3D audio.
     * 
     * @return The audio listener for this game.
     */
    AudioListener* getAudioListener();

    /**
     * Menu callback on menu events.
     */
    virtual void menu();
    
    /**
     * Shows or hides the virtual keyboard (if supported).
     *
     * @param display true when virtual keyboard needs to be displayed; false otherwise.
     */
     inline void displayKeyboard(bool display);
     
    /**
     * Keyboard callback on keyPress events.
     *
     * @param evt The key event that occured.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Touch callback on touch events.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @see Touch::TouchEvent
     */
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Mouse callback on mouse events. If the game does not consume the mouse move event or left mouse click event
     * then it is interpreted as a touch event instead.
     *
     * @param evt The mouse event that occurred.
     * @param x The x position of the mouse in pixels. Left edge is zero.
     * @param y The y position of the mouse in pixels. Top edge is zero.
     * @param wheelDelta The number of mouse wheel ticks. Positive is up (forward), negative is down (backward).
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Gamepad callback on gamepad events.
     *
     * @param evt The gamepad event that occurred.
     * @param gamepad the gamepad the event occurred on
     * @param index The joystick or button index that triggered the event.
     */
    virtual void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int index);

    /**
     * Sets muli-touch is to be enabled/disabled. Default is disabled.
     *
     * @param enabled true sets multi-touch is enabled, false to be disabled.
     */
    inline void setMultiTouch(bool enabled);

    /**
     * Is multi-touch mode enabled.
     *
     * @return true is multi-touch is enabled.
     */
    inline bool isMultiTouch() const;

    /**
     * Gets the current accelerometer values.
     *
     * @param pitch The pitch angle returned (in degrees). If NULL then not returned.
     * @param roll The roll angle returned (in degrees). If NULL then not returned.
     */
    inline void getAccelerometerValues(float* pitch, float* roll);

    /**
     * Schedules a time event to be sent to the given TimeListener a given number of game milliseconds from now.
     * Game time stops while the game is paused. A time offset of zero will fire the time event in the next frame.
     * 
     * @param timeOffset The number of game milliseconds in the future to schedule the event to be fired.
     * @param timeListener The TimeListener that will receive the event.
     * @param cookie The cookie data that the time event will contain.
     */
    void schedule(float timeOffset, TimeListener* timeListener, void* cookie = 0);

    /** 
     * Creates a Gamepad object from a .form file.
     *
     * @param playerIndex
     * @param formPath
     */
    Gamepad* createGamepad(const char* gamepadFormPath);

    /**
     * Gets the gamepad for the specified player index.
     *
     * @param playerIndex The player index to get the gamepad for (0 <= playerIndex <= 3) 
     */
    inline Gamepad* getGamepad(unsigned int playerIndex = 0) const;

    /**
     * Gets the number of gamepad's connected to the game.
     * 
     * @return the number of gamepad's connected to the game.
     */
    inline unsigned int getGamepadCount() const;

protected:

    /**
     * Constructor.
     */
    Game();

    /**
     * Initialize callback that is called just before the first frame when the game starts.
     */
    virtual void initialize() = 0;

    /**
     * Finalize callback that is called when the game on exits.
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
    virtual void update(float elapsedTime) = 0;

    /**
     * Render callback for handling rendering routines.
     *
     * Called just after update, once per frame when game is running.
     * Ideal for all rendering code.
     *
     * @param elapsedTime The elapsed game time.
     */
    virtual void render(float elapsedTime) = 0;

    /**
     * Renders a single frame once and then swaps it to the display.
     *
     * This is useful for rendering splash screens.
     */
    template <class T>
    void renderOnce(T* instance, void (T::*method)(void*), void* cookie);

    /**
     * Updates the game's internal systems (audio, animation, physics) once.
     * 
     * Note: This does not call the user-defined Game::update() function.
     *
     * This is useful for rendering animated splash screens.
     */
    void updateOnce();

private:

    /**
     * TimeEvent represents the event that is sent to TimeListeners as a result of 
     * calling Game::schedule().
     */
    class TimeEvent
    {
    public:

        TimeEvent(double time, TimeListener* timeListener, void* cookie);
        // The comparator is used to determine the order of time events in the priority queue.
        bool operator<(const TimeEvent& v) const;
        
        /**
         * The game time.
         * @see Game::getGameTime()
         */
        double time;
        TimeListener* listener;
        void* cookie;
    };

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

    /**
     * Fires the time events that were scheduled to be called.
     * 
     * @param frameTime The current game frame time. Used to determine which time events need to be fired.
     */
    void fireTimeEvents(double frameTime);

    /**
     * Loads the game configuration.
     */
    void loadConfig();

    /**
     * Loads a gamepad from the configuration file.
     */
    void loadGamepad();

    bool _initialized;                          // If game has initialized yet.
    State _state;                               // The game state.
    static double _pausedTimeLast;              // The last time paused.
    static double _pausedTimeTotal;             // The total time paused.
    double _frameLastFPS;                       // The last time the frame count was updated.
    unsigned int _frameCount;                   // The current frame count.
    unsigned int _frameRate;                    // The current frame rate.
    unsigned int _width;                        // The game's display width.
    unsigned int _height;                       // The game's display height.
    Rectangle _viewport;                        // the games's current viewport.
    Vector4 _clearColor;                        // The clear color value last used for clearing the color buffer.
    float _clearDepth;                          // The clear depth value last used for clearing the depth buffer.
    int _clearStencil;                          // The clear stencil value last used for clearing the stencil buffer.
    Properties* _properties;                    // Game configuration properties object.
    AnimationController* _animationController;  // Controls the scheduling and running of animations.
    AudioController* _audioController;          // Controls audio sources that are playing in the game.
    PhysicsController* _physicsController;      // Controls the simulation of a physics scene and entities.
    AudioListener* _audioListener;              // The audio listener in 3D space.
    unsigned int _gamepadCount;
    Gamepad** _gamepads;
    std::priority_queue<TimeEvent, std::vector<TimeEvent>, std::less<TimeEvent> >* _timeEvents; // Contains the scheduled time events.

    // Note: Do not add STL object member variables on the stack; this will cause false memory leaks to be reported.

    friend class ScreenDisplayer;
};

}

#include "Game.inl"

#endif
