#include "SpriteSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Sprite", SpriteSample, 6);
#endif

SpriteSample::SpriteSample()
    : _font(NULL), _scene(NULL), _cameraNode(NULL),
      _playerSprite(NULL), _playerNode(NULL), _playerAnimation(NULL), _playerMovement(0)
{
}

void SpriteSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Load sprite scene
    _scene = Scene::load("res/common/sprites/sprite.scene");
    _cameraNode = _scene->findNode("camera");

    // Setup the player
    _playerNode = _scene->findNode("player");
    _playerSprite = dynamic_cast<Sprite*>(_playerNode->getDrawable());

    // Idle [0] - Set at load time
    // Walk [1 - 11]
    _playerSprite->setFrameSource(1, Rectangle( 0, 0, 72, 92));
    _playerSprite->setFrameSource(2, Rectangle(73, 0, 72, 97));
    _playerSprite->setFrameSource(3, Rectangle(146, 0, 72, 97));
    _playerSprite->setFrameSource(4, Rectangle(0, 98, 72, 97));
    _playerSprite->setFrameSource(5, Rectangle(73, 98, 72, 97));
    _playerSprite->setFrameSource(6, Rectangle(146, 98, 72, 97));
    _playerSprite->setFrameSource(7, Rectangle(219, 0, 72, 97));
    _playerSprite->setFrameSource(8, Rectangle(292, 0, 72, 97));
    _playerSprite->setFrameSource(9, Rectangle(219, 98, 72, 97));
    _playerSprite->setFrameSource(10, Rectangle(365, 0, 72, 97));
    _playerSprite->setFrameSource(11, Rectangle(292, 98, 72, 97));
    // Jump[12]
    _playerSprite->setFrameSource(12, Rectangle(438, 93, 67, 94));

    // The player animation clips
    unsigned int keyTimes[4] = {0, 1, 11, 12};
    float keyValues[4] =  { 0, 1, 11, 12 };
    _playerAnimation = _playerSprite->createAnimation("player-animations", Sprite::ANIMATE_KEYFRAME, 4, keyTimes, keyValues, Curve::LINEAR);
    _playerAnimation->createClip("idle", 0, 0);
    _playerAnimation->createClip("walk", 1, 11)->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
    // Set the speed to 24 FPS
    _playerAnimation->getClip("walk")->setSpeed(24.0f/1000.0f);
    _playerAnimation->play("idle");

    // Setup player text
    Node* playerTextNode = _scene->findNode("text");
    playerTextNode->addRef();
    _scene->removeNode(playerTextNode);
    //XXX This is because SceneLoader doesn't support loading child nodes for other nodes
    _playerNode->addChild(playerTextNode);

    playerTextNode->translateY(_playerSprite->getHeight());
    Text* playerText = dynamic_cast<Text*>(playerTextNode->getDrawable());
    playerText->setJustify(Font::ALIGN_TOP_HCENTER);
    playerText->setWidth(_playerSprite->getWidth());
    SAFE_RELEASE(playerTextNode);


    // Custom Effect in sprite
    Effect* waterEffect = Effect::createFromFile("res/shaders/sprite.vert", "res/common/sprites/water2d.frag");
    Sprite* waterSprite = Sprite::create("res/common/sprites/water2d.png", getWidth() * 5, getHeight() / 3, waterEffect);
    SAFE_RELEASE(waterEffect);
    waterSprite->setAnchor(Vector2::zero());
    waterSprite->setOpacity(0.5f);
    _scene->findNode("water")->setDrawable(waterSprite);
    Material* waterMaterial = waterSprite->getMaterial();
    SAFE_RELEASE(waterSprite);
    Texture::Sampler* noiseSampler = Texture::Sampler::create("res/common/sprites/water2d-noise.png");
    waterMaterial->getParameter("u_texture_noise")->setValue(noiseSampler);
    SAFE_RELEASE(noiseSampler);
    waterMaterial->getParameter("u_time")->bindValue(this, &SpriteSample::getTime);
}

void SpriteSample::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void SpriteSample::update(float elapsedTime)
{
    if ((_playerMovement & WALK_FORWARD) == WALK_FORWARD)
    {
        float moveX = 0.25 * elapsedTime;
        _cameraNode->translateX(moveX);
        _playerSprite->setFlip(Sprite::FLIP_NONE);
        _playerNode->translateX(moveX);
        if (!_playerAnimation->getClip("walk")->isPlaying())
        {
            _playerAnimation->stop("idle");
            _playerAnimation->play("walk");
        }
    }
    else if ((_playerMovement &  WALK_BACKWARD) == WALK_BACKWARD)
    {
        float moveX = -0.25 * elapsedTime;
        _cameraNode->translateX(moveX);
        _playerSprite->setFlip(Sprite::FLIP_HORIZONTAL);
        _playerNode->translateX(moveX);
        if (!_playerAnimation->getClip("walk")->isPlaying())
        {
            _playerAnimation->stop("idle");
            _playerAnimation->play("walk");
        }
    }
    else
    {
        // IDLE
        if (!_playerAnimation->getClip("idle")->isPlaying())
        {
            _playerAnimation->stop("walk");
            _playerAnimation->play("idle");
        }
    }
}

void SpriteSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    
    // Visit all the nodes in the scene, drawing the sprites
    _scene->visit(this, &SpriteSample::drawScene);
    
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SpriteSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
            case Keyboard::KEY_A:
                _playerMovement |= WALK_BACKWARD;
                break;
            case Keyboard::KEY_D:
                _playerMovement |= WALK_FORWARD;
                break;
            case Keyboard::KEY_C:
                Node* clone = _playerNode->clone();
                _scene->addNode(clone);
                clone->translateZ(-1);
                break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
            case Keyboard::KEY_A:
                _playerMovement &= ~WALK_BACKWARD;
                break;
            case Keyboard::KEY_D:
                _playerMovement &= ~WALK_FORWARD;
                break;
        }
    }
}

void SpriteSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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

bool SpriteSample::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    
    return true;
}

float SpriteSample::getTime() const
{
    float angle = Game::getGameTime() * 0.001 * MATH_PIX2;
    if (angle > MATH_PIX2)
        angle -= MATH_PIX2;
    return angle;
}
