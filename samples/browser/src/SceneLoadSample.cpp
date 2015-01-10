#include "SceneLoadSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Scene Loading", SceneLoadSample, 16);
#endif

SceneLoadSample::SceneLoadSample()
    : _font(NULL), _scene(NULL), _wireFrame(false)
{
    
}

void SceneLoadSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    _scene = Scene::load("res/common/sample.scene");

    // Update the aspect ratio for our scene's camera to match the current device resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &SceneLoadSample::initializeMaterials);

}


bool SceneLoadSample::initializeMaterials(Node* node)
{
    Model* model = dynamic_cast<Model*>(node->getDrawable());
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

void SceneLoadSample::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}

void SceneLoadSample::update(float elapsedTime)
{
    
}

void SceneLoadSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &SceneLoadSample::drawScene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SceneLoadSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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

void SceneLoadSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            _wireFrame = !_wireFrame;
            break;
        }
    }
}

bool SceneLoadSample::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw(_wireFrame);
    return true;
}
