#pragma once

#include "Serializable.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Input.h"

namespace gameplay
{

class Graphics;

/**
 * Defines the game class.
 */
class Game
{
public:

    /**
     * The game states.
     */
    enum State
    {
        STATE_UNINITIALIZED,
        STATE_SPLASH,
        STATE_LOADING,
        STATE_RUNNING,
        STATE_PAUSED
    };

    /**
     * Defines a splash screen.
     */
    class SplashScreen
    {
    public:
        std::string url;
        float duration;
    };
    
    /**
     * Constructor.
     */
    Game();

    /**
     * Destructor.
     */
    ~Game();

    /**
     * Gets the single instance of the game.
     * 
     * @return The single instance of the game.
     */
    static Game* getInstance();

    /**
     * Gets the game state.
     *
     * @return The current game state.
     */
    State getState() const;

    /**
     * Gets the total absolute running time (in milliseconds) since platform startup.
     *
     * @return The total absolute running time (in milliseconds) since platform startup. 
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
     * Gets the current frame rate.
     *
     * @return The current frame rate.
     */
	size_t getFrameRate() const;

	/**
     * Gets the number of frame that have passed.
     *
     * @return The number of frame that have passed.
     */
	size_t getFrameCount() const;

    /**
     * Gets the game window width.
     *
     * @return The game window width.
     */
	size_t getWidth() const;

    /**
     * Gets the game window height.
     *
     * @return The game window height.
     */
	size_t getHeight() const;

    /**
     * Gets the aspect ratio of the window (width / height).
     *
     * @return The aspect ratio of the window (width / height).
     */
    float getAspectRatio() const;

    /**
     * Pauses the game.
     */
    void pause();

    /**
     * Resumes the game after being paused.
     */
    void resume();

    /**
     * Exits the game.
     */
    int exit();

    /**
     * Event occurs every frame from the platform.
     */
    void frame();

	/**
	 *
	 */
	void showSplashScreens(std::vector<SplashScreen> splashScreens);

	/**
	 * Loads the scene and any resources that are needed.
	 *
	 * @param url The url the the scene to be set and loaded. 
     * @param showLoadingScene true shows the loading screen until scene is loaded.
     * @see Game::setLoadingScene
	 */
	void loadScene(const std::string& url, bool showLoadingScene = true);

    /**
     * Unloads the game scene and any resource that are no longer needed.
     *
     * @param scene The scene to be unloaded.
     */
    void unloadScene(std::shared_ptr<SceneObject> scene);

    /**
     * Sets the current scene to be active and shown on screen.
     *
     * @param scene The scene to be set active and shown on screen.
     */
    void setScene(std::shared_ptr<SceneObject> scene);

	/**
	 * Gets the current game scene to be active and shown on screen.
	 *
	 * @return The current game scene to be active and shown on screen.
	 */
	std::shared_ptr<SceneObject> getScene() const;

    /**
     * Sets the active camera the current scene will use to view the scene.
     *
     * The active camera by default is the first object with a camera
     * attached called "mainCamera".
     *
     * The camera is not activated if the camera component is not
     * attached to an scene object in the active scene.
     *
     * @param camera The camera the scene will use to view the scene.
     */
    void setCamera(std::shared_ptr<Camera> camera);

    /**
     * Gets the active camera the current scene will use to view the scene.
     *
     * @return The camera the scene will use to view the scene.
     */
    std::shared_ptr<Camera> getCamera() const;

    /**
     * Event occurs after the platform starts up and prior to first frame event.
     */
    virtual void onInitialize();

    /**
     * Event occurs when the game is about to exit.
     */
    virtual void onFinalize();

    /**
     * Event occurs when the platform window has been resized.
     *
     * @param width The new game window width.
     * @param height The new game window height.
     */
    virtual void onResize(size_t width, size_t height);

    /**
     * Event occurs every frame before the scene is updated.
	 *
	 * @param elapsedTime The time elapsed (in seconds) since the last onUpdate call.
     */
    virtual void onUpdate(float elapsedTime);

    /**
     * Event occurs when the scene has completed loading all the scenes resources.
     *
     * @param scene The scene that has completed loading.
     */
    virtual void onLoad(std::shared_ptr<SceneObject> scene);

	virtual void onGamepadConnection(uint32_t controllerIndex, bool connected);

	virtual void onGamepadAxis(uint32_t controllerIndex, Input::GamepadAxis axis, int value);

	virtual void onMouseMotion(int mx, int my, int mz);

	virtual void onMousePress(int mx, int my, int mz, Input::MouseButton button, bool down);

	virtual void onKeyChar(char chr);

	virtual void onKeyPress(Input::Key key, uint8_t keyModifiers, bool down);

	virtual void onDropFile(std::string file);

    /**
     * Game configuration.
     */
    class Config : public Serializable
    {
    public:

        /**
         * Constructor
         */
        Config();

        /**
         * Destructor
         */
        ~Config();

        /**
         * @see Serializable::getClassName
         */
        std::string getClassName();

        /**
         * @see Serializable::onSerialize
         */
        void onSerialize(Serializer* serializer);

        /**
         * @see Serializable::onDeserialize
         */
        void onDeserialize(Serializer* serializer);

        /**
         * @see Serializer::Activator::createObject
         */
        static std::shared_ptr<Serializable> createObject();

        std::string title;
		int width;
		int height;
		bool fullscreen;
		bool vsync;
		size_t multisampling;
		bool validation;
		std::string homePath;
        std::vector<SplashScreen> splashScreens;
		std::string mainScene;
    };

    /**
     * Gets the game configuration
     *
     * @return The game configuration.
     */
    std::shared_ptr<Game::Config> getConfig();

	/**
	 * Gets the game graphics
	 */
	std::shared_ptr<Graphics> getGraphics();

	Game* next;

private:

    Game(const Game& copy);
    void initializeSplash();
    void initializeLoading();
    void onSplash(float elapsedTime);
    void onLoading(float elapsedTime);
    double updateFrameRate();

    std::shared_ptr<Game::Config> _config;
    State _state;
	size_t _width;
	size_t _height;
	static std::chrono::time_point<std::chrono::high_resolution_clock> _timeStart;
	static double _pausedTimeLast;
	static double _pausedTimeTotal;
	size_t _pausedCount;
	double _frameLastFPS;
	size_t _frameCount;
	size_t _frameRate;
    std::queue<SplashScreen> _splashScreens;
    std::map<std::string, std::shared_ptr<SceneObject>> _scenesLoaded;
    std::shared_ptr<SceneObject> _sceneLoading;
	std::shared_ptr<SceneObject> _scene;
    std::shared_ptr<Camera> _camera;
	std::shared_ptr<Graphics> _graphics;
};

}
