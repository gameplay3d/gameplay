#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

float _rotateY = 0.0f;
bool _wDown = false;
bool _sDown = false;
bool _aDown = false;
bool _dDown = false;
#define WALK_SPEED 0.06f
#define ANIM_SPEED 1.5f

int kcount = 0;

CharacterGame::CharacterGame()
    : _font(NULL), _scene(NULL), _character(NULL), _animation(NULL), _animationState(0), _rotateX(0)
{
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::initialize()
{
    // Load the font.
    _font = Font::create("res/arial40.gpb");

    // Load scene.
    _scene = Scene::load("res/scene.scene");

    // Store character node.
    _character = _scene->findNode("Boy");

    // Initialize scene.
    _scene->visit(this, &CharacterGame::initScene);

    // Load animations clips.
    loadAnimationClips();
}

void initMaterial(Scene* scene, Node* node, Material* material)
{
    if (material)
    {
        Node* lightNode = scene->findNode("SunLight");

        material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
        material->getParameter("u_lightColor")->bindValue(lightNode->getLight(), &Light::getColor);
        material->getParameter("u_ambientColor")->bindValue(scene, &Scene::getAmbientColor);
    }
}

bool CharacterGame::initScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        if (model->getMaterial())
        {
            initMaterial(_scene, node, model->getMaterial());
        }
        for (unsigned int i = 0; i < model->getMeshPartCount(); ++i)
        {
            if (model->hasPartMaterial(i))
            {
                initMaterial(_scene, node, model->getMaterial(i));
            }
        }
    }

    return true;
}

void CharacterGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void CharacterGame::update(long elapsedTime)
{
    Node* node = _character;//_scene->findNode("Camera");
    if (_wDown)
        node->translateForward(-WALK_SPEED);
    else if (_sDown)
        node->translateForward(WALK_SPEED);
    if (_aDown)
        node->translateLeft(-WALK_SPEED);
    else if (_dDown)
        node->translateLeft(WALK_SPEED);

    node->setTranslationY(0);
}

void CharacterGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawScene);

    _font->begin();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->end();
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

void CharacterGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    Animation* a = Game::getInstance()->getAnimationController()->getAnimation("movements");
    if (a == NULL)
        return;
    AnimationClip* walk = a->getClip("walk");
    AnimationClip* idle = a->getClip("idle");

    if (evt == Keyboard::KEY_PRESS)
    {
        kcount++;
        if (key == Keyboard::KEY_W)
            _wDown = true;
        else if (key == Keyboard::KEY_S)
            _sDown = true;
        else if (key == Keyboard::KEY_A)
            ;//_aDown = true;
        else if (key == Keyboard::KEY_D)
            ;//_dDown = true;

        if (_wDown)
        {
            if (!walk->isPlaying())
            {
                idle->stop();
                walk->setSpeed(ANIM_SPEED);
                walk->play();
            }
            else if (walk->getSpeed() < 0)
            {
                walk->setSpeed(ANIM_SPEED);
            }
        }
        else if (_sDown)
        {
            if (!walk->isPlaying())
            {
                idle->stop();
                walk->setSpeed(-ANIM_SPEED);
                walk->play();
            }
            else if (walk->getSpeed() > 0)
            {
                walk->setSpeed(-ANIM_SPEED);
            }
        }
       
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        if (key == Keyboard::KEY_W)
            _wDown = false;
        else if (key == Keyboard::KEY_S)
            _sDown = false;
        else if (key == Keyboard::KEY_A)
            _aDown = false;
        else if (key == Keyboard::KEY_D)
            _dDown = false;

        if (!_wDown && !_sDown && walk->isPlaying())
        {
            walk->stop();
            idle->play();
        }
    }
}

void CharacterGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        {
            _rotateX = x;
            _rotateY = y;
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
    case Touch::TOUCH_RELEASE:
        {
            _rotateX = 0;
            _rotateY = 0;
        }
        break;
    case Touch::TOUCH_MOVE:
        {
            int deltaX = x - _rotateX;
            int deltaY = y - _rotateY;
            _rotateX = x;
            _rotateY = y;
            _character->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
            //_character->rotateX(-MATH_DEG_TO_RAD(deltaY * 0.5f));
            //_character->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
        }
        break;
    default:
        break;
    };
}

void CharacterGame::loadAnimationClips()
{
    _animation = Game::getInstance()->getAnimationController()->getAnimation("movements");
    _animation->createClips("res/boy.animation");
    AnimationClip* clip = _animation->getClip("idle");
    clip->play();
}
