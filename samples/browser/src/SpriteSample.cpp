#include "SpriteSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Sprite", SpriteSample, 6);
#endif

SpriteSample::SpriteSample()
    : _font(NULL), _scene(NULL), _cameraNode(NULL),
      _floorTileSet(NULL), _floorNode(NULL),
      _backgroundSprite(NULL), _backgroundNode(NULL),
      _playerSprite(NULL), _playerNode(NULL), _playerAnimation(NULL), _playerMovement(0),
      _rocketSprite(NULL), _rocketNode(NULL),
      _waterSprite(NULL), _waterNode(NULL),
      _text(NULL), _textNode(NULL)
{
}

void SpriteSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Create an orthographic projection matrix.
    float width = (float)getWidth();
    float height = (float)getHeight();
    float aspectRatio = width / height;
    _scene = Scene::create();
    _cameraNode = _scene->addNode("camera");
    Camera* camera = Camera::createOrthographic(width, height, aspectRatio, 0, 100);
    _cameraNode->setCamera(camera);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);
    _cameraNode->translateX(width / 2);
    _cameraNode->translateY(height / 2);

    // Background sprite image
    _backgroundSprite = Sprite::create("res/common/sprites/background.png", getWidth() * 5, getHeight());
    _backgroundNode = _scene->addNode("background");
    _backgroundNode->setSprite(_backgroundSprite);
    
    // Level floor tile set
    _floorTileSet = TileSet::create("res/common/sprites/level.png", 70, 70, 3, 7);
    _floorTileSet->setTileSource(0, 0, Vector2(568, 284));
    _floorTileSet->setTileSource(1, 0, Vector2(568, 284));
    _floorTileSet->setTileSource(2, 0, Vector2(568, 284));
    _floorTileSet->setTileSource(3, 0, Vector2(568, 284));
    _floorTileSet->setTileSource(4, 0, Vector2(497, 284));
    
    _floorTileSet->setTileSource(0, 1, Vector2(568, 0));
    _floorTileSet->setTileSource(1, 1, Vector2(568, 0));
    _floorTileSet->setTileSource(2, 1, Vector2(568, 0));
    _floorTileSet->setTileSource(3, 1, Vector2(568, 0));
    _floorTileSet->setTileSource(4, 1, Vector2(710, 142));
    _floorTileSet->setTileSource(5, 1, Vector2(497, 284));
    
    _floorTileSet->setTileSource(0, 2, Vector2(568, 0));
    _floorTileSet->setTileSource(1, 2, Vector2(568, 0));
    _floorTileSet->setTileSource(2, 2, Vector2(568, 0));
    _floorTileSet->setTileSource(3, 2, Vector2(568, 0));
    _floorTileSet->setTileSource(4, 2, Vector2(568, 0));
    _floorTileSet->setTileSource(5, 2, Vector2(710, 142));
    _floorTileSet->setTileSource(6, 2, Vector2(497, 284));
    
    _floorNode = _scene->addNode("floor");
    _floorNode->setTileSet(_floorTileSet);
    
    // Idle[0]
    _playerSprite = Sprite::create("res/common/sprites/player1.png", 72.0f, 97.0f, Rectangle(67, 196, 66, 92), 13);
    //_playerSprite->computeFrames(3, 1);
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
    _playerNode = _scene->addNode("player");
    _playerNode->setSprite(_playerSprite);
    _playerNode->translateY(_floorTileSet->getHeight());
    
    // The player animation clips
    unsigned int keyTimes[4] = {0, 1, 11, 12};
    float keyValues[4] =  { 0, 1, 11, 12 };
    _playerAnimation = _playerSprite->createAnimation("player-animations", Sprite::ANIMATE_KEYFRAME, 4, keyTimes, keyValues,
                                                Curve::LINEAR);
    _playerAnimation->createClip("idle", 0, 0);
    _playerAnimation->createClip("walk", 1, 11)->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
    // Set the speed to 24 FPS
    _playerAnimation->getClip("walk")->setSpeed(24.0f/1000.0f);
    _playerAnimation->play("idle");
    
    // Rocket
    _rocketSprite = Sprite::create("res/common/sprites/rocket.png", 128, 128);
    _rocketSprite->setBlendMode(Sprite::BLEND_ADDITIVE);
    _rocketSprite->setAnchor(Vector2(0.5f, 0.3f));
    _rocketSprite->setOffset(Sprite::OFFSET_ANCHOR);
    _rocketNode = _scene->addNode("rocket");
    _rocketNode->setSprite(_rocketSprite);
    _rocketNode->translate(Vector3(getWidth(), 0,  0));
    _rocketNode->rotateZ(MATH_DEG_TO_RAD(-45));
    
    // Custom Effect in sprite
    Effect* waterEffect = Effect::createFromFile("res/shaders/sprite.vert", "res/common/sprites/water2d.frag");
    _waterSprite = Sprite::create("res/common/sprites/water2d.png", getWidth() * 5, getHeight() / 3, waterEffect);
    _waterSprite->setAnchor(Vector2::zero());
    _waterSprite->setOpacity(0.5f);
    _waterNode = _scene->addNode("water");
    _waterNode->setSprite(_waterSprite);
    Material* waterMaterial = _waterSprite->getMaterial();
    Texture::Sampler* noiseSampler = Texture::Sampler::create("res/common/sprites/water2d-noise.png");
    waterMaterial->getParameter("u_texture_noise")->setValue(noiseSampler);
    SAFE_RELEASE(noiseSampler);
    waterMaterial->getParameter("u_time")->bindValue(this, &SpriteSample::getTime);
    _waterNode->translateY(-50);
    
    // Text node.
    _text = Text::create("res/ui/arial.gpb", "P1", Vector4(0, 0, 1, 1), 18);
    _textNode = Node::create("text");
    _playerNode->addChild(_textNode);
    _text->setWidth(_playerNode->getSprite()->getWidth());
    _textNode->setText(_text);
    _text->setJustify(Font::ALIGN_TOP_HCENTER);
    _textNode->translateY(_playerNode->getSprite()->getHeight());
}

void SpriteSample::finalize()
{
    SAFE_RELEASE(_playerSprite);
    SAFE_RELEASE(_rocketSprite);
    SAFE_RELEASE(_waterSprite);
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
    Sprite* sprite = node->getSprite();
    if (sprite)
        sprite->draw();
    TileSet* tileset = node->getTileSet();
    if (tileset)
        tileset->draw();
    Text* text = node->getText();
    if (text)
        text->draw();
    
    return true;
}

float SpriteSample::getTime() const
{
    float angle = Game::getGameTime() * 0.001 * MATH_PIX2;
    if (angle > MATH_PIX2)
        angle -= MATH_PIX2;
    return angle;
}
