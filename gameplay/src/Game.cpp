#include "Base.h"
#include "Game.h"
#include "Activator.h"
#include "FileSystem.h"
#include "SerializerJson.h"
#include "Graphics.h"
#include "Light.h"

namespace gameplay
{
#define SPLASH_DURATION     2.0f

static Game* __gameInstance = nullptr;
std::chrono::time_point<std::chrono::high_resolution_clock> Game::_timeStart = std::chrono::high_resolution_clock::now();
double Game::_pausedTimeLast = 0.0;
double Game::_pausedTimeTotal = 0.0;

Game::Game() :
    _config(nullptr),
    _state(State::eUninitialized),
    _width(GP_GRAPHICS_WIDTH),
    _height(GP_GRAPHICS_HEIGHT),
    _pausedCount(0),
    _frameLastFPS(0),
    _frameCount(0),
    _frameRate(0),
    _sceneLoading(nullptr),
    _scene(nullptr)
{
    __gameInstance = this;

    Activator::getActivator()->registerSystemTypes();
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

size_t Game::getFrameCount() const
{
    return _frameCount;
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
    if (_state == State::eRunning)
    {
        _state = State::ePaused;
        _pausedTimeLast = getAbsoluteTime();
    }
    ++_pausedCount;
}

void Game::resume()
{
    if (_state == State::ePaused)
    {
        --_pausedCount;
        if (_pausedCount == 0)
        {
            _state = State::eRunning;
            _pausedTimeTotal += getAbsoluteTime() - _pausedTimeLast;
        }
    }
}

int Game::exit()
{
    if (_state != State::eUninitialized)
    {
        onFinalize();
        _state = State::eUninitialized;
    }
    return 0;
}

void Game::frame()
{
    static double lastFrameTime = Game::getGameTime();
    float elapsedTime = (float)(Game::getGameTime() - lastFrameTime);
    
    _graphics->render(elapsedTime);

    switch (_state)
    {
    case State::eUninitialized:
        {
            initializeSplash();
            initializeLoading();
            _state = State::eSplash;
            break;
        }
    case State::eSplash:
        {
            onSplash(elapsedTime);
            if (_splashScreens.empty())
            {
                _state = State::eLoading;
            }
            lastFrameTime = updateFrameRate();
            break;
        }
    case State::eLoading:
        {
            onLoading(elapsedTime);
            //if (_scene.get() && _scene->isLoaded())
            _state = State::eRunning;
            lastFrameTime = updateFrameRate();
            break;
        }
    case State::eRunning:
        {
            onUpdate(elapsedTime);
            lastFrameTime = updateFrameRate();
            break;
        }
    case State::ePaused:
    default:
        break;
    }
}

void Game::showSplashScreens(std::vector<SplashScreen> splashScreens)
{
}

void Game::loadScene(const std::string& url, bool showLoading)
{
    // Unload any previous scene
    if (_scene.get() && (_scene != _sceneLoading))
    {
        // todo: AssetManager::beginLoading();
    }

    // Set the loading scene and change states
    _scene = _sceneLoading;
    _state = State::eLoading;
}

void Game::unloadScene(std::shared_ptr<SceneObject> scene)
{
}

void Game::setScene(std::shared_ptr<Scene> scene)
{
    _scene = scene;
}

std::shared_ptr<Scene> Game::getScene() const
{
    return _scene;
}

void Game::onInitialize()
{
    _config = getConfig();
    FileSystem::setHomePath(_config->homePath);

    _graphics = std::make_shared<Graphics>();
    _graphics->initialize();
}

void Game::onFinalize()
{
}

void Game::onResize(size_t width, size_t height)
{
    _width = width;
    _height = height;
    _graphics->resize(width, height);
}

void Game::onUpdate(float elapsedTime)
{
}

void Game::onLoad(std::shared_ptr<SceneObject> scene)
{
}

void Game::onGamepadConnection(uint32_t controllerIndex, bool connected)
{
}

void Game::onGamepadAxis(uint32_t controllerIndex, Input::GamepadAxis axis, int value)
{
}

void Game::onMouseMotion(int mx, int my)
{
}

void Game::onMouseWheel(int value)
{
}

void Game::onMousePress(int mx, int my, int mz, Input::MouseButton button, bool down)
{
}

void Game::onKeyChar(char chr)
{
}

void Game::onKeyPress(Input::Key key, Input::KeyModifiers keyModifiers, bool down)
{
}

void Game::onDropFile(std::string file)
{
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
    if (_config)
    {
        return _config;
    }

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
    return _config;
}

std::shared_ptr<Graphics> Game::getGraphics()
{
    return _graphics;
}

Game::Config::Config() :
    title(""),
    width(GP_GRAPHICS_WIDTH),
    height(GP_GRAPHICS_HEIGHT),
    fullscreen(GP_GRAPHICS_FULLSCREEN),
    vsync(GP_GRAPHICS_VSYNC),
    multisampling(GP_GRAPHICS_MULTISAMPLING),
    validation(GP_GRAPHICS_VALIDATION),
    homePath(GP_ENGINE_HOME_PATH),
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
    serializer->writeInt("width", width, 0);
    serializer->writeInt("height", height, 0);
    serializer->writeBool("fullscreen", fullscreen, false);
    serializer->writeBool("vsync", vsync, false);
    serializer->writeInt("multisampling", (uint32_t)multisampling, 0);
    serializer->writeBool("validation", validation, false);
    serializer->writeString("homePath", homePath.c_str(), GP_ENGINE_HOME_PATH);
    serializer->writeStringList("splashScreens", splashScreens.size());
    for (size_t i = 0; i < splashScreens.size(); i++)
    {
        std::string splash = std::string(splashScreens[i].url) + ":" + std::to_string(splashScreens[i].duration);
        serializer->writeString(nullptr, splash.c_str(), "");
    }
    serializer->writeString("mainScene", mainScene.c_str(), "");
}

void Game::Config::onDeserialize(Serializer* serializer)
{
    serializer->readString("title", title, "");
    width = serializer->readInt("width", 0);
    height = serializer->readInt("height", 0);
    fullscreen = serializer->readBool("fullscreen", false);
    vsync = serializer->readBool("vsync", false);
    multisampling = serializer->readInt("multisampling", 0);
    validation = serializer->readBool("validation", false);
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
                splashScreen.duration = SPLASH_DURATION;
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
                    splashScreen.duration = SPLASH_DURATION;
                }
            }
            splashScreens.push_back(splashScreen);
        }
    }
    serializer->readString("mainScene", mainScene, "");
}

}
