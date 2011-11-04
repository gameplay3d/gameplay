/*
 * CharacterGame
 */

#include "CharacterGame.h"

CharacterGame game; 

CharacterGame::CharacterGame()
    : _animationState(0)
{
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::touch(int x, int y, int touchEvent)
{
    switch (touchEvent)
    {
    case Input::TOUCHEVENT_PRESS:
    {
        switch (_animationState)
        {
            case 0:
                _animation->stop("left_leg");
                _animation->play("right_arm");
                break;
            case 1:
                _animation->stop("right_arm");
                _animation->play("right_leg");
                break;
            case 2:
                _animation->stop("right_leg");
                _animation->play("left_leg");
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
    _animation = Game::getInstance()->getAnimationController()->getAnimation("movements");
    _animation->createClips("res/animations/seymour.animation");

    AnimationClip* clip = _animation->getClip("right_arm");
    clip->setActiveDuration(AnimationClip::REPEAT_INDEFINITE);
}

void CharacterGame::initialize()
{
    // Initialize the render state
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Load mesh from file
    Package* meshPackage = Package::create("res/models/Seymour.gpb");

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
    Material* meshMaterial = model->setMaterial("res/shaders/diffuse-specular.vsh", "res/shaders/diffuse-specular.fsh", "#define SKINNING \n #define SKINNING_JOINT_COUNT 35");
    meshMaterial->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
    meshMaterial->getParameter("u_diffuseTexture")->setValue("res/textures/seymour-diffuse.png", true);
    meshMaterial->getParameter("u_lightDirection")->setValue(lightDirection);
    meshMaterial->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    meshMaterial->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    meshMaterial->getParameter("u_specularExponent")->setValue(50.0f);
    meshMaterial->setParameterAutoBinding("u_worldMatrix", RenderState::WORLD_MATRIX);
    meshMaterial->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
    meshMaterial->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);
    meshMaterial->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_POSITION);
    meshMaterial->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
    
    loadCharacterAnimations();

    // Load the font
    Package* fontPackage = Package::create("res/fonts/arial16.gpb");
    _font = fontPackage->loadFont("arial16");

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