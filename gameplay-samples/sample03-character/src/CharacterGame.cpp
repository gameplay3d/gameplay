/*
 * CharacterGame
 */

#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

CharacterGame::CharacterGame()
    : _animationState(0)
{
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::initialize()
{
    // Load mesh from file
    Package* pkg = Package::create("res/models/Seymour.gpb");
    _scene = pkg->loadScene();
    SAFE_RELEASE(pkg);

    Camera* camera = _scene->getActiveCamera();
    if (!camera)
    {
        createDefaultCamera(_scene);
    }

    _scene->visit(this, &CharacterGame::getModelNode);
    assert(_modelNode);

    Model* model = _modelNode->getModel();
    assert(model);

    // Get spot light position and direction.
    Node* spotLightNode = _scene->findNode("spotLight1");

    // Load character's material from a .material file
    Material* meshMaterial = model->setMaterial("res/materials/character.material");
    meshMaterial->getParameter("u_spotLightPosition")->bindValue(spotLightNode, &Node::getTranslationView);
    meshMaterial->getParameter("u_spotLightDirection")->bindValue(spotLightNode, &Node::getForwardVectorView);
    meshMaterial->getParameter("u_spotLightInnerAngleCos")->bindValue(spotLightNode->getLight(), &Light::getInnerAngleCos);
    meshMaterial->getParameter("u_spotLightOuterAngleCos")->bindValue(spotLightNode->getLight(), &Light::getOuterAngleCos);

    // Load character animations.
    loadCharacterAnimations();

    // Load plane.
    Node* planeNode = _scene->findNode("pPlane1");
    assert(planeNode);
    Model* planeModel = planeNode->getModel();
    assert(planeModel);

    // Load material from a .material file.
    Material* planeMaterial = planeModel->setMaterial("res/materials/plane.material");
    planeMaterial->getParameter("u_spotLightPosition")->bindValue(spotLightNode, &Node::getTranslationView);
    planeMaterial->getParameter("u_spotLightDirection")->bindValue(spotLightNode, &Node::getForwardVectorView);
    planeMaterial->getParameter("u_spotLightInnerAngleCos")->bindValue(spotLightNode->getLight(), &Light::getInnerAngleCos);
    planeMaterial->getParameter("u_spotLightOuterAngleCos")->bindValue(spotLightNode->getLight(), &Light::getOuterAngleCos);

    // Load the font
    _font = Font::create("res/fonts/arial16.gpb");
}

void CharacterGame::finalize()
{
}

void CharacterGame::update(long elapsedTime)
{
}

void CharacterGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

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

void CharacterGame::touch(int x, int y, int touchEvent)
{
    switch (touchEvent)
    {
    case Input::TOUCHEVENT_PRESS:
        {
            _rotateX = x;
            switch (_animationState)
            {
                case 0:
                    _animation->stop("head");
                    _animation->play("right_arm");
                    break;
                case 1:
                    _animation->stop("right_arm");
                    _animation->play("left_arm");
                    break;
                case 2:
                    _animation->stop("left_arm");
                    _animation->play("right_leg");
                    break;
                case 3:
                    _animation->stop("right_leg");
                    _animation->play("left_leg");
                    break;
                case 4:
                    _animation->stop("left_leg");
                    _animation->play("head");
                    break;
            }
            _animationState = (_animationState + 1) % 5;
        }
        break;
    case Input::TOUCHEVENT_RELEASE:
        {
            _rotateX = 0;
        }
        break;
    case Input::TOUCHEVENT_MOVE:
        {
            int deltaX = x - _rotateX;
            _rotateX = x;
            _modelNode->rotateY(MATH_DEG_TO_RAD(deltaX * 0.5f));
        }
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
