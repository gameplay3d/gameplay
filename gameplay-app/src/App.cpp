#include "App.h"

namespace gameplay
{

App app;


App::App()
{
}

App::~App()
{
}

void App::onInitialize()
{
	Game::onInitialize();

    // Set the asset path for saving our assets created
    FileSystem::setAssetPath("./");

    /* Quick test
    // Create a test scene
    auto test = std::make_shared<SceneObject>();

    // Create a camera object
    auto cameraObject = std::make_shared<SceneObject>();
    cameraObject->setName("mainCamera");
    cameraObject->setLocalPosition(Vector3(0, 0, -10));
    cameraObject->lookAt(Vector3::zero());
    test->addChild(cameraObject);
        
    // Attach a perpective mode camera component
    auto camera = std::make_shared<Camera>();
    camera->setMode(Camera::MODE_PERSPECTIVE);
    camera->setFieldOfView(46.0f);
    camera->setAspectRatio(this->getAspectRatio());
    camera->setNearPlane(2.0f);
    camera->setFarPlane(2000.0f);
    cameraObject->attachComponent(camera);

    // Set this camera as the main camera
    this->setCamera(camera);

     // Create a light object
    auto lightObject = std::make_shared<SceneObject>();
    lightObject->setName("directionalLight");
    lightObject->setLocalEulerAngles(Vector3(0, 45, 0));
    test->addChild(lightObject);

    // Attach a directional light component
    auto light = std::make_shared<Light>();
    light->setType(Light::TYPE_DIRECTIONAL);
    light->setColor(Vector3::fromColorString("#ff0000"));
    lightObject->attachComponent(light);
    
    auto writer = SerializerJson::createWriter("test.scene");
    writer->writeObject(nullptr, test);
    writer->close();
    GP_SAFE_DELETE(writer);

    // Load a second test scene from the test.scene file
    // TODO


    setScene(test);
    */
}

void App::onFinalize()
{
	Game::onFinalize();
}

void App::onResize(size_t width, size_t height)
{
	Game::onResize(width, height);
}

void App::onUpdate(float elapsedTime)
{
	Game::onUpdate(elapsedTime);
}

void App::onRender(float elapsedTime)
{
	Game::onRender(elapsedTime);
}

void App::onGamepadEvent(Platform::GamepadEvent evt, size_t gamepadIndex)
{
	Game::onGamepadEvent(evt, gamepadIndex);
}

void App::onKeyEvent(Platform::KeyboardEvent evt, int key)
{
	Game::onKeyEvent(evt, key);
}

void App::onMouseEvent(Platform::MouseEvent evt, int x, int y, int wheelDelta)
{
	Game::onMouseEvent(evt, x, y, wheelDelta);
}

void App::onTouchEvent(Platform::TouchEvent evt, int x, int y, size_t touchIndex)
{
	Game::onTouchEvent(evt, x, y, touchIndex);
}

}
