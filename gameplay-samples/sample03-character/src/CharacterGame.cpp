#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

CharacterGame::CharacterGame()
    : _font(NULL), _scene(NULL), _modelNode(NULL), _animation(NULL), _animationState(0), _rotateX(0)
{
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::initialize()
{
    // Load the font.
    _font = Font::create("res/arial40.gpb");
    
    // Load mesh from file.
    Package* pkg = Package::create("res/seymour.gpb");
    _scene = pkg->loadScene();
    SAFE_RELEASE(pkg);

    _modelNode = _scene->findNode("boyShape");

    // Get directional light node.
    Node* lightNode = _scene->findNode("directionalLight1");

    // Load character's material from a .material file.
    Material* meshMaterial = _modelNode->getModel()->setMaterial("res/seymour.material");
    meshMaterial->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
    
    // Load character animations clips.
    loadAnimationClips();

    // Get plane.
    Node* planeNode = _scene->findNode("floor");

    // Load planes material from a .material file.
    Material* planeMaterial = planeNode->getModel()->setMaterial("res/floor.material");
    planeMaterial->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
}

void CharacterGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void CharacterGame::update(long elapsedTime)
{
}

void CharacterGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawScene);
}

bool CharacterGame::drawScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        model->draw();
    }
    return true;
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

void CharacterGame::loadAnimationClips()
{
    _animation = Game::getInstance()->getAnimationController()->getAnimation("movements");
    _animation->createClips("res/seymour-clips.animation");
    AnimationClip* clip = _animation->getClip("right_arm");
    clip->setActiveDuration(AnimationClip::REPEAT_INDEFINITE);
}
