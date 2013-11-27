#include "SceneRendererSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Scene", "SceneRenderer", SceneRendererSample, 3);
#endif

SceneRendererSample::SceneRendererSample() : _font(NULL), _scene(NULL)
{
}

void SceneRendererSample::initialize()
{
    setMultiTouch(true);

    _font = Font::create("res/common/arial.gpb");
    _scene = Scene::load("res/common/sample.scene");
    _renderer = SceneRendererForward::create();

    // Test de-activating a node.
    Node* pipe = _scene->findNode("pipe");
    pipe->setActive(false);
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    // TODO: Remove this automatically Do this in the forward renderer
    _scene->visit(this, &SceneRendererSample::initializeMaterials);
}

bool SceneRendererSample::initializeMaterials(Node* node)
{
    // TODO: Remove this automatically Do this in the forward renderer
    Model* model = node->getModel();
    if (model)
    {
        Material* material = model->getMaterial();
        // For this sample we will only bind a single light to each object in the scene.
        MaterialParameter* colorParam = material->getParameter("u_directionalLightColor[0]");
        colorParam->setValue(Vector3(0.75f, 0.75f, 0.75f));
        MaterialParameter* directionParam = material->getParameter("u_directionalLightDirection[0]");
        directionParam->setValue(Vector3(0, -1, 0));
    }
    return true;
}

void SceneRendererSample::finalize()
{
    SAFE_RELEASE(_font);
}

void SceneRendererSample::update(float elapsedTime)
{
    _scene->update(elapsedTime);
}

void SceneRendererSample::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    _renderer->render(_scene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SceneRendererSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
            return;
        }
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

void SceneRendererSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
}