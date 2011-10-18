/*
 * CharacterGame
 */

#include "CharacterGame.h"

CharacterGame game;

CharacterGame::CharacterGame()
    : _clips(NULL), _animationState(0)
{
}

CharacterGame::~CharacterGame()
{
    SAFE_DELETE_ARRAY(_clips);
}

void CharacterGame::touch(int x, int y, int touchEvent)
{
    switch (touchEvent)
    {
    case Input::TOUCHEVENT_PRESS:
    {
        Animation* animation = Game::getInstance()->getAnimationController()->getAnimation(ANIMATION_ID);
        assert(animation);
        AnimationClip* clip = NULL;
        switch (_animationState)
        {
            case 0:
                _clips[2]->stop();
                _clips[0]->play();
                break;
            case 1:
                _clips[0]->stop();
                _clips[1]->play();
                break;
            case 2:
                _clips[1]->stop();
                _clips[2]->play();
                break;
        }
        _animationState = (_animationState + 1) % 3;
    }
        break;
    case Input::TOUCHEVENT_RELEASE:
        break;
    case Input::TOUCHEVENT_MOVE:
        break;
    default:
        break;
    };
}

void CharacterGame::loadCharacterAnimations()
{
    const float FRAME_RATE = 24.0f;

    Animation* animation = Game::getInstance()->getAnimationController()->getAnimation(ANIMATION_ID);
    assert(animation);

    _clips = new AnimationClip*[3];
    _clips[0] = animation->createClip("right arm", (61.0f / FRAME_RATE) * 1000L, (120.0f / FRAME_RATE) * 1000L);
    _clips[0]->setRepeatCount(ANIMATION_REPEAT_COUNT_INDEFINITE);
    _clips[0]->setSpeed(1.0f);

    _clips[1] = animation->createClip("right leg", (121.0f / FRAME_RATE) * 1000L, (180.0f / FRAME_RATE) * 1000L);
    _clips[1]->setRepeatCount(ANIMATION_REPEAT_COUNT_INDEFINITE);       
    _clips[1]->setSpeed(2.0f);

    _clips[2] = animation->createClip("left leg", (181.0f / FRAME_RATE) * 1000L, (240.0f / FRAME_RATE) * 1000L);
    _clips[2]->setRepeatCount(3.0f);
    _clips[2]->setSpeed(3.0f);
}

void CharacterGame::initialize()
{
    // Initialize GL state
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Load mesh from file
    Package* meshPackage = Package::create("res/meshes/Seymour.gpb");

    _scene = meshPackage->loadScene();

    Camera* camera = _scene->getActiveCamera();
    if (!camera)
    {
        createDefaultCamera(_scene);
    }
    
    _scene->visit(this, &CharacterGame::getModelNode);
    assert(_modelNode);

    Vector3 lightDirection(0.0f, 0.0f, 1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformNormal(&lightDirection);

    Model* model = _modelNode->getModel();
    assert(model);
    MeshSkin* skin = model->getSkin();
    assert(skin);

    // Load material
    Material* meshMaterial = Material::createMaterial("res/shaders/skinning.vsh", "res/shaders/skinning.fsh", "#define JOINT_COUNT 35");
    meshMaterial->getParameter("u_diffuseTexture")->setValue("res/textures/seymour-diffuse.png", true);
    meshMaterial->getParameter("u_lightDirection")->setValue(lightDirection);
    meshMaterial->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    meshMaterial->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    meshMaterial->getParameter("u_specularExponent")->setValue(50.0f);
    // Bind the skins matrix palette to the skinning shader.
    meshMaterial->getParameter("u_matrixPalette")->bindValue(skin, &MeshSkin::getMatrixPalette, &MeshSkin::getMatrixPaletteSize);
    model->setMaterial(meshMaterial);

    loadCharacterAnimations();

    // Load the font
    Package* fontPackage = Package::create("res/fonts/arial16.gpb");
    _font = fontPackage->loadFont("arial16");

    SAFE_RELEASE(meshMaterial);
    SAFE_RELEASE(fontPackage);
    SAFE_RELEASE(meshPackage);
}

void CharacterGame::finalize()
{
}

void CharacterGame::update(long elapsedTime)
{
}

void CharacterGame::render(long elapsedTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawModel);

    /* Draw the FPS
    char fps[3];
    sprintf(fps, "%u", Game::getFrameRate()); 
    _font->begin();
    _font->drawText(fps, 5, 5, Color::red());
    _font->end();
    */
}

void CharacterGame::drawModel(Node* node, long cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        model->draw();
    }
}

void CharacterGame::getModelNode(Node* node, long cookie)
{
    if (!_modelNode)
    {
        Model* model = node->getModel();
        if (model)
        {
            _modelNode = node;
        }
    }
}

void CharacterGame::createDefaultCamera(Scene* scene)
{
    // create the camera and node for it
    Node* node = scene->addNode("camera");
    Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 1.0f, 100.0f);
    node->setCamera(camera);
    node->translate(0, 5.0f, 20.0f);
    scene->setActiveCamera(camera);
}