#include "SceneRendererSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Scene", "SceneRenderer", SceneRendererSample, 3);
#endif

SceneRendererSample::SceneRendererSample() : _font(NULL), _scene(NULL), _visibleSet(NULL)
{
}

void SceneRendererSample::initialize()
{
    setMultiTouch(true);

    _font = Font::create("res/common/arial.gpb");
    _scene = Scene::load("res/common/sample.scene");
    _visibleSet = VisibleSetDefault::create(_scene);
    _renderer = SceneRendererForward::create();

    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
}

void SceneRendererSample::finalize()
{
    SAFE_RELEASE(_font);
}

void SceneRendererSample::update(float elapsedTime)
{
}

void SceneRendererSample::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    _renderer->render(_visibleSet);

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