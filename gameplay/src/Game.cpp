#include "Base.h"
#include "Game.h"
#include "FileSystem.h"
#include "Serializable.h"
#include "SerializerJson.h"
#include "Graphics.h"

namespace gameplay
{
static const int  GAME_SPLASH_SCREEN_DURATION = 2.0f;

static Game* __gameInstance = nullptr;
std::chrono::time_point<std::chrono::high_resolution_clock> Game::_timeStart = std::chrono::high_resolution_clock::now();
double Game::_pausedTimeLast = 0.0;
double Game::_pausedTimeTotal = 0.0;
std::shared_ptr<SceneObject> Game::_sceneLoadingDefault = std::make_shared<SceneObject>();


Game::Game() : 
	_config(nullptr),
	_state(Game::STATE_UNINITIALIZED),
    _width(GP_GRAPHICS_WIDTH),
    _height(GP_GRAPHICS_HEIGHT),
	_mouseCapture(false),
	_cursorVisible(false),
	_pausedCount(0),
	_frameLastFPS(0),
	_frameCount(0),
	_frameRate(0),
    _sceneLoading(nullptr),
	_scene(nullptr),
    _camera(nullptr)
{
	__gameInstance = this;
}

Game::~Game()
{
	__gameInstance = nullptr;
}

Game* Game::getInstance()
{
    return __gameInstance;
}

Game::State Game::getState() const
{
    return _state;
}

double Game::getAbsoluteTime()
{
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = now - _timeStart;
	return diff.count();
}

double Game::getGameTime()
{
   return getAbsoluteTime() - _pausedTimeTotal;
}

size_t Game::getFrameRate() const
{
    return _frameRate;
}

size_t Game::getWidth() const
{
    return _width;
}

size_t Game::getHeight() const
{
    return _height;
}

float Game::getAspectRatio() const
{
    return (float)_width / (float)_height;
}

void Game::pause()
{
    if (_state == Game::STATE_RUNNING)
    {
        _state = Game::STATE_PAUSED;
        _pausedTimeLast = getAbsoluteTime();
    }
    ++_pausedCount;
}

void Game::resume()
{
    if (_state == Game::STATE_PAUSED)
    {
        --_pausedCount;
        if (_pausedCount == 0)
        {
            _state = Game::STATE_RUNNING;
            _pausedTimeTotal += getAbsoluteTime() - _pausedTimeLast;
        }
    }
}

void Game::exit()
{
	if (_state != Game::STATE_UNINITIALIZED)
    {
        // Signal the platform that we are about to shutdown the game.
        getPlatform()->shutdown();
        // Call user finalize
        onFinalize();
        // Finalize Sub-Systems
        _state = Game::STATE_UNINITIALIZED;
    }
    std::exit(0);
}

void Game::showSplashScreens(std::vector<SplashScreen> splashScreens)
{
}

void Game::setLoadingScene(const std::string& url)
{
    loadScene(url, false);
}

std::shared_ptr<SceneObject> Game::getLoadingScene() const
{
    return _sceneLoading;
}

void Game::loadScene(const std::string& url, bool showLoading)
{
    // Unload any previous scene
    if (_scene.get() && (_scene != _sceneLoading) && (_scene != _sceneLoadingDefault))
        _scene->unload();

    // Set the loading scene and change states
    _scene = _sceneLoading;
    _state = Game::STATE_LOADING;

    // 
	//_scene = dynamic_cast<SceneObject*>(loadAsset(_config->sceneUrl.c_str()));
	//_scene->onInitialize();
}

void Game::unloadScene(std::shared_ptr<SceneObject> scene)
{
}

void Game::setScene(std::shared_ptr<SceneObject> scene)
{
    _scene = scene;
}

std::shared_ptr<SceneObject> Game::getScene() const
{
	return _scene;
}

void Game::setCamera(std::shared_ptr<Camera> camera)
{
    _camera = camera;
}

std::shared_ptr<Camera> Game::getCamera() const
{
    return _camera;
}

void Game::onInitialize()
{
	_config = getConfig();
	FileSystem::setHomePath(_config->homePath);
}

void Game::onFinalize()
{
    if (_scene.get())
        _scene->onFinalize();
}

void Game::onSceneLoad(std::shared_ptr<SceneObject> scene)
{
}

void Game::onResize(size_t width, size_t height)
{
    _width = width;
    _height = height;
}

bool Game::isMouseCapture()
{    
	return _mouseCapture;
}

void Game::setMouseCapture(bool capture)
{
	_mouseCapture = capture;
}

bool Game::isCursorVisible()
{
	return _cursorVisible;
}

void Game::setCursorVisible(bool visible)
{
	_cursorVisible = visible;
}

void Game::onUpdate(float elapsedTime)
{
}

void Game::onRender(float elapsedTime)
{
	// Deferred rendering here
	//Graphics* graphics = Graphics::getGraphics();
	



}

void Game::onFrame()
{
	static double lastFrameTime = Game::getGameTime();
    float elapsedTime = (float)(Game::getGameTime() - lastFrameTime);

	switch (_state)
	{
		case Game::STATE_UNINITIALIZED:
		{
            initializeSplash();
            initializeLoading();
			_state = Game::STATE_SPLASH;
			break;
		}
        case Game::STATE_SPLASH:
        {
            onSplash(elapsedTime);
            if (_splashScreens.empty())
                _state = Game::STATE_LOADING;
            lastFrameTime = updateFrameRate();
            break;
        }
		case Game::STATE_LOADING:
		{
            onLoading(elapsedTime);
            //if (_scene.get() && _scene->isLoaded())
				_state = Game::STATE_RUNNING;
            lastFrameTime = updateFrameRate();
			break;
		}
		case Game::STATE_RUNNING:
		{
			onUpdate(elapsedTime);
			onRender(elapsedTime);
            lastFrameTime = updateFrameRate();
			break;
		}
		case Game::STATE_PAUSED:
			break;
	}
}

void Game::initializeSplash()
{
}

void Game::initializeLoading()
{
}

void Game::onSplash(float elapsedTime)
{
}

void Game::onLoading(float elapsedTime)
{
    
}

double Game::updateFrameRate()
{
    ++_frameCount;
    double now = Game::getGameTime();
	if ((now - _frameLastFPS) >= 1000)
	{
		_frameRate = _frameCount;
		_frameCount = 0;
		_frameLastFPS = now;
	}
    return now;
}

std::shared_ptr<Game::Config> Game::getConfig()
{
    if (!_config)
    {
        Serializer* reader = Serializer::createReader(GP_ENGINE_CONFIG);
        if (reader)
        {
            std::shared_ptr<Serializable> config = reader->readObject(nullptr);
            _config = std::static_pointer_cast<Game::Config>(config);
            reader->close();
             GP_SAFE_DELETE(reader);
        }
		else
		{
			_config = std::make_shared<Game::Config>();
			Serializer* writer = SerializerJson::createWriter(GP_ENGINE_CONFIG);
			writer->writeObject(nullptr, _config);
			writer->close();
            GP_SAFE_DELETE(writer);
		}
    }
    return _config;
}

Game::Config::Config() :
    title(""),
	graphics(GP_GRAPHICS),
	width(GP_GRAPHICS_WIDTH),
	height(GP_GRAPHICS_HEIGHT),
	fullscreen(GP_GRAPHICS_FULLSCREEN),
	vsync(GP_GRAPHICS_VSYNC),
	multisampling(GP_GRAPHICS_MULTISAMPLING),
	validation(GP_GRAPHICS_VALIDATION),
	homePath(GP_ENGINE_HOME_PATH),
    loadingScene("loading.scene"),
	mainScene("main.scene")
{
}

Game::Config::~Config()
{
}

std::shared_ptr<Serializable> Game::Config::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Game::Config>());
}

std::string Game::Config::getClassName()
{
    return "gameplay::Game::Config";
}

void Game::Config::onSerialize(Serializer* serializer)
{
    serializer->writeString("title", title.c_str(), "");
	serializer->writeString("graphics", graphics.c_str(), "");
    serializer->writeInt("width", width, 0);
    serializer->writeInt("height", height, 0);
    serializer->writeBool("fullscreen", fullscreen, false);
	serializer->writeBool("vsync", vsync, false);
	serializer->writeInt("multisampling", (unsigned int)multisampling, 0);
	serializer->writeBool("validation", validation, GP_GRAPHICS_VALIDATION);
	serializer->writeString("homePath", homePath.c_str(), GP_ENGINE_HOME_PATH);
    serializer->writeStringList("splashScreens", splashScreens.size());
    for (size_t i = 0; i < splashScreens.size(); i++)
    {
        std::string splash = std::string(splashScreens[i].url) + ":" + std::to_string(splashScreens[i].duration);
        serializer->writeString(nullptr, splash.c_str(), "");
    }
    serializer->writeString("loadingScene", loadingScene.c_str(), "");
	serializer->writeString("mainScene", mainScene.c_str(), "");
}

void Game::Config::onDeserialize(Serializer* serializer)
{
    serializer->readString("title", title, "");
	serializer->readString("graphics", graphics, GP_GRAPHICS);
    width = serializer->readInt("width", 0);
    height = serializer->readInt("height", 0);
	fullscreen = serializer->readBool("fullscreen", false);
	vsync = serializer->readBool("vsync", false);
    multisampling = serializer->readInt("multisampling", 0);
	serializer->readString("homePath", homePath, "");
    size_t splashScreensCount = serializer->readStringList("splashScreens");
    for (size_t i = 0; i < splashScreensCount; i++)
    {
        std::string splash;
        serializer->readString(nullptr, splash, ""); 
        if (splash.length() > 0)
        {
            SplashScreen splashScreen;
            size_t semiColonIdx = splash.find(':');
            if (semiColonIdx == std::string::npos)
            {
                splashScreen.url = splash;
                splashScreen.duration = GAME_SPLASH_SCREEN_DURATION;
            }
            else
            {
                splashScreen.url = splash.substr(0, semiColonIdx);
                std::string durationStr = splash.substr(semiColonIdx + 1, splash.length() - semiColonIdx);
                try
                {
                    splashScreen.duration = std::stof(durationStr);
                }
                catch (...)
                {
                    splashScreen.duration = GAME_SPLASH_SCREEN_DURATION;
                }
            }
            splashScreens.push_back(splashScreen);
        }
    }
    serializer->readString("loadingScene", loadingScene, "");
	serializer->readString("mainScene", mainScene, "");
}

}
