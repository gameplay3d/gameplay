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

    /* Quick test
    // Create a test scene
    auto test = std::make_shared<SceneObject>();

    // Create a main camera
    auto cameraObject = std::make_shared<SceneObject>();
    cameraObject->setName("mainCamera");
    cameraObject->setLocalPosition(Vector3(0, 0, -10));
    cameraObject->lookAt(Vector3::zero());
    test->addChild(cameraObject);
    auto camera = std::make_shared<Camera>();
    camera->setMode(Camera::MODE_PERSPECTIVE);
    camera->setFieldOfView(45.0f);
    camera->setClipPlaneNear(2.0f);
    camera->setClipPlaneFar(2000.0f);
    cameraObject->attachComponent(camera);

    // Set this camera as the main camera
    this->setCamera(camera);

     // Create a direcional light
    auto lightObject = std::make_shared<SceneObject>();
    lightObject->setName("directionalLight");
    lightObject->setLocalEulerAngles(Vector3(0, 45, 0));
    test->addChild(lightObject);
    auto light = std::make_shared<Light>();
    light->setType(Light::TYPE_DIRECTIONAL);
    light->setColor(Vector3::fromColorString("#ff0000"));
    lightObject->attachComponent(light);

	// Create a model
    auto modelObject = std::make_shared<SceneObject>();
    cameraObject->setName("box");
    cameraObject->setLocalPosition(Vector3(0, 0, 0));
    test->addChild(modelObject);
	auto model = std::shared_ptr<Model>();
	model->load("box.fbx");
	modelObject->attachComponent(model);
    
    auto writer = SerializerJson::createWriter("test.scene");
    writer->writeObject(nullptr, test);
    writer->close();
    GP_SAFE_DELETE(writer);
	

    setScene(test);*/
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

}
