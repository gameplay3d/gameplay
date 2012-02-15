#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

unsigned int keyFlags = 0;
float _rotateY = 0.0f;
#define WALK_SPEED  7.5f
#define ANIM_SPEED 10.0f
#define BLEND_DURATION 150.0f

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
    _character = new PhysicsCharacter(_scene->findNode("BoyCharacter"), PhysicsCharacter::Capsule(1.5f, 5.0f, 2.25f));//0.0f));//2.9f));

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
    // Update character animation and movement
    if (keyFlags == 0)
    {
        _character->play("idle", PhysicsCharacter::PLAY_REPEAT, 1.0f, BLEND_DURATION);
    }
    else
    {
        // Forward motion
        if (keyFlags & 1)
        {
            _character->play("walk", PhysicsCharacter::PLAY_REPEAT, ANIM_SPEED, BLEND_DURATION);
            _character->moveForward(1.0f);
        }
        else if (keyFlags & 2)
        {
            _character->play("walk", PhysicsCharacter::PLAY_REPEAT, -ANIM_SPEED, BLEND_DURATION);
            _character->moveForward(-1.0f);
        }
        else
        {
            // Cancel forward movement
            _character->moveForward(0.0f);
        }

        // Strafing
        if (keyFlags & 4)
        {
            _character->play("walk", PhysicsCharacter::PLAY_REPEAT, ANIM_SPEED, BLEND_DURATION);
            _character->moveRight(1.0f);
        }
        else if (keyFlags & 8)
        {
            _character->play("walk", PhysicsCharacter::PLAY_REPEAT, -ANIM_SPEED, BLEND_DURATION);
            _character->moveRight(-1.0f);
        }
        else
        {
            // Cancel right movement
            _character->moveRight(0.0f);
        }
    }

    //_character->update(elapsedTime);
}

bool drawDebug = false;

void CharacterGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawScene);

    if (drawDebug)
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());

    _font->begin();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->end();
}

void drawBoundingSphere(Scene* scene, const BoundingSphere& sphere)
{
    // Draw sphere
    static Model* boundsModel = NULL;
    if (boundsModel == NULL)
    {
        Package* pkg = Package::create("res/sphere.gpb");
        Mesh* mesh = pkg->loadMesh("sphereShape");
        SAFE_RELEASE(pkg);
        boundsModel = Model::create(mesh);
        SAFE_RELEASE(mesh);
        boundsModel->setMaterial("res/shaders/solid.vsh", "res/shaders/solid.fsh");
        boundsModel->getMaterial()->getParameter("u_diffuseColor")->setValue(Vector4(0, 1, 0, 1));
        boundsModel->getMaterial()->getStateBlock()->setCullFace(false);
        boundsModel->getMaterial()->getStateBlock()->setDepthTest(true);
        boundsModel->getMaterial()->getStateBlock()->setDepthWrite(true);
    }

    static Matrix m;
    Matrix::createTranslation(sphere.center, &m);
    m.scale(sphere.radius / 0.5f);
    Matrix::multiply(scene->getActiveCamera()->getViewProjectionMatrix(), m, &m);
    boundsModel->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(m);
    boundsModel->draw(true);
}

bool CharacterGame::drawScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        model->draw(false);

        //drawBoundingSphere(_scene, node->getBoundingSphere());
    }

    return true;
}

bool cameraToggle = false;
Quaternion cr;
Vector3 ct;

void CharacterGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            keyFlags |= 1;
            break;
        case Keyboard::KEY_S:
            keyFlags |= 2;
            break;
        case Keyboard::KEY_A:
            keyFlags |= 4;
            break;
        case Keyboard::KEY_D:
            keyFlags |= 8;
            break;
        case Keyboard::KEY_P:
            drawDebug = !drawDebug;
            break;
        case Keyboard::KEY_C:
            {
                Node* c = _scene->findNode("Camera");
                cameraToggle = !cameraToggle;
                if (cameraToggle)
                {
                    cr = c->getRotation();
                    ct = c->getTranslation();
                    c->setTranslation(0, 20, 0);
                    c->setRotation(Vector3::unitX(), MATH_DEG_TO_RAD(-90.0f));
                }
                else
                {
                    c->setRotation(cr);
                    c->setTranslation(ct);
                }
            }
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            keyFlags &= ~1;
            break;
        case Keyboard::KEY_S:
            keyFlags &= ~2;
            break;
        case Keyboard::KEY_A:
            keyFlags &= ~4;
            break;
        case Keyboard::KEY_D:
            keyFlags &= ~8;
            break;
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
            _character->getNode()->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
            _scene->findNode("Camera")->rotateX(-MATH_DEG_TO_RAD(deltaY * 0.5f));
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

    _character->addAnimation("idle", _animation->getClip("idle"), 0.0f);
    _character->addAnimation("walk", _animation->getClip("walk"), WALK_SPEED);

    _character->play("idle", PhysicsCharacter::PLAY_REPEAT);
}
