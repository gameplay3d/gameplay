#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

unsigned int keyFlags = 0;
float _rotateY = 0.0f;
#define WALK_SPEED  7.5f
#define ANIM_SPEED 10.0f
#define BLEND_DURATION 150.0f
#define CAMERA_FOCUS_RANGE 16.0f

int drawDebug = 0;
bool moveBall = false;

CharacterGame::CharacterGame()
    : _font(NULL), _scene(NULL), _character(NULL), _animation(NULL), _animationState(0), _rotateX(0)
{
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::initialize()
{
    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &CharacterGame::drawSplash, NULL, 1000L);

    // Load the font.
    _font = Font::create("res/arial40.gpb");

    // Load scene.
    _scene = Scene::load("res/scene.scene");

    // Store character node.
    Node* node = _scene->findNode("BoyCharacter");
    PhysicsRigidBody::Parameters p;
    p.mass = 20.0f;
    node->setTranslationY(5.0f);
    node->setCollisionObject(PhysicsCollisionObject::CHARACTER, PhysicsCollisionShape::capsule(1.2f, 5.0f, Vector3(0, 2.5, 0), true), &p);
    _character = static_cast<PhysicsCharacter*>(node->getCollisionObject());
    _character->setMaxStepHeight(0.0f);
    _character->addCollisionListener(this);

    // Initialize scene.
    _scene->visit(this, &CharacterGame::initScene);

    // Load animations clips.
    loadAnimationClips(node);

    // Initialize the gamepad.
	_gamepad = new Gamepad("res/gamepad.png", 1, 1);

	Gamepad::Rect leftRegionInner = {130.0f, this->getHeight() - 130.0f, 47.0f, 47.0f};
    Gamepad::Rect leftTexRegionInner = {10.0f, 188.0f, 47.0f, 47.0f};
    Gamepad::Rect leftRegionOuter = {120.0f, this->getHeight() - 130.0f, 170.0f, 170.0f};
    Gamepad::Rect leftTexRegionOuter = {0.0f, 0.0f, 170.0f, 170.0f};
    _gamepad->setJoystick(JOYSTICK, &leftRegionInner, &leftTexRegionInner, &leftRegionOuter, &leftTexRegionOuter, 45.0f);

	Gamepad::Rect regionOnScreen = {this->getWidth() - 120.0f, this->getHeight() - 130.0f, 47.0f, 47.0f};
	Gamepad::Rect defaultRegion = {10.0f, 188.0f, 50.0f, 47.0f};
	Gamepad::Rect focusRegion = {69.0f, 188.0f, 50.0f, 47.0f};
	_gamepad->setButton(BUTTON_1, &regionOnScreen, &defaultRegion, &focusRegion);
}

void CharacterGame::initMaterial(Scene* scene, Node* node, Material* material)
{
    std::string id = node->getId();
    if (material &&
        (id == "Basketball" || id.find("GreenChair") != id.npos || id.find("BlueChair") != id.npos || 
        id == "Easel" || id == "BoyMesh" || id == "BoyShadow" || id == "Rainbow"))
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
            if (model->hasMaterial(i))
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
    Gamepad::ButtonState buttonOneState = _gamepad->getButtonState(BUTTON_1);
	Vector2 joystickVec = _gamepad->getJoystickState(JOYSTICK);
	keyFlags = 0;

	if (joystickVec.x > 0)
	{
		keyFlags |= 8;
	}
	else if (joystickVec.x < 0)
	{
		keyFlags |= 4;
	}
	
	if (joystickVec.y > 0)
	{
		keyFlags |= 1;
	}
	else if (joystickVec.y < 0)
	{
		keyFlags |= 2;
	}	
	
	/*
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
		case Keyboard::KEY_B:
			moveBall = !moveBall;
			break;
        }
	*/
    // Update character animation and movement
    if (joystickVec.isZero())
    {
        _character->play("idle", PhysicsCharacter::ANIMATION_REPEAT, 1.0f, BLEND_DURATION);
		_character->setForwardVelocity(0.0f);
    }
    else
    {
		float angle = atan2(joystickVec.x, joystickVec.y);
		_character->setRotation(Vector3::unitY(), angle);

		_character->setForwardVelocity(joystickVec.length());

        // Forward motion
        if (keyFlags & 1)
        {
            if (moveBall)
            {
                static_cast<PhysicsRigidBody*>(_scene->findNode("Basketball")->getCollisionObject())->applyForce(Vector3(0, 0, -WALK_SPEED));
            }
            else
            {
                _character->play("walk", PhysicsCharacter::ANIMATION_REPEAT, ANIM_SPEED, BLEND_DURATION);
                _character->setForwardVelocity(1.0f);
            }
        }
        else if (keyFlags & 2)
        {
            if (moveBall)
            {
                static_cast<PhysicsRigidBody*>(_scene->findNode("Basketball")->getCollisionObject())->applyForce(Vector3(0, 0, WALK_SPEED));
            }
            else
            {
                _character->play("walk", PhysicsCharacter::ANIMATION_REPEAT, -ANIM_SPEED, BLEND_DURATION);
                _character->setForwardVelocity(-1.0f);
            }
        }
        else
        {
            // Cancel forward movement
            _character->setForwardVelocity(0.0f);
        }

        // Strafing
        if (keyFlags & 4)
        {
            if (moveBall)
            {
                static_cast<PhysicsRigidBody*>(_scene->findNode("Basketball")->getCollisionObject())->applyForce(Vector3(-WALK_SPEED, 0, 0));
            }
            else
            {
                _character->play("walk", PhysicsCharacter::ANIMATION_REPEAT, ANIM_SPEED, BLEND_DURATION);
                _character->setRightVelocity(1.0f);
            }
        }
        else if (keyFlags & 8)
        {
            if (moveBall)
            {
                static_cast<PhysicsRigidBody*>(_scene->findNode("Basketball")->getCollisionObject())->applyForce(Vector3(WALK_SPEED, 0, 0));
            }
            else
            {
                _character->play("walk", PhysicsCharacter::ANIMATION_REPEAT, -ANIM_SPEED, BLEND_DURATION);
                _character->setRightVelocity(-1.0f);
            }
        }
        else
        {
            // Cancel right movement
            _character->setRightVelocity(0.0f);
        }
    }

    if (!moveBall)
    {
        fixCamera(elapsedTime);
    }
}

void CharacterGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawScene, (void*)0);
    _scene->visit(this, &CharacterGame::drawScene, (void*)1);

    switch (drawDebug)
    {
    case 1:
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
        break;
    case 2:
        _scene->drawDebug(Scene::DEBUG_BOXES);
        break;
    case 3:
        _scene->drawDebug(Scene::DEBUG_SPHERES);
        break;
    }

    _gamepad->draw();

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
        switch ((int)cookie)
        {
        case 0: // opaque objects
            if (!node->isTransparent())
                model->draw();
            break;

        case 1: // transparent objects
            if (node->isTransparent())
                model->draw();
            break;
        }
    }

    return true;
}

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
            drawDebug++;
            if (drawDebug > 3)
                drawDebug = 0;
            break;
        case Keyboard::KEY_B:
            moveBall = !moveBall;
            break;
        case Keyboard::KEY_ESCAPE:
            end();
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
            //_scene->findNode("Camera")->rotateX(-MATH_DEG_TO_RAD(deltaY * 0.5f));
            //_character->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
        }
        break;
    default:
        break;
    };
}

void CharacterGame::loadAnimationClips(Node* node)
{
    _animation = node->getAnimation("movements");
    _animation->createClips("res/boy.animation");

    _character->addAnimation("idle", _animation->getClip("idle"), 0.0f);
    _character->addAnimation("walk", _animation->getClip("walk"), WALK_SPEED);

    _character->play("idle", PhysicsCharacter::ANIMATION_REPEAT);
}

void CharacterGame::collisionEvent(
    PhysicsCollisionObject::CollisionListener::EventType type,
    const PhysicsCollisionObject::CollisionPair& collisionPair,
    const Vector3& contactPointA, const Vector3& contactPointB)
{
    if (collisionPair.objectA == _character)
    {
        if (collisionPair.objectB->getType() == PhysicsCollisionObject::RIGID_BODY)
        {
            PhysicsCharacter* c = static_cast<PhysicsCharacter*>(collisionPair.objectA);
            //c->setve
        }
    }
}

void CharacterGame::fixCamera(long elapsedTime)
{
    static float cameraOffset = 0.0f;

#define RAY_STEP_SIZE 0.1f

    Node* node = _scene->getActiveCamera()->getNode();
    
    Vector3 cameraForward = node->getForwardVectorWorld();
    cameraForward.normalize();

    Vector3 cameraPosition = node->getTranslationWorld();
    Vector3 focalPoint = cameraPosition + (cameraForward * CAMERA_FOCUS_RANGE);

    Ray cameraRay(cameraPosition, cameraPosition - focalPoint);

    float d = cameraRay.getOrigin().distanceSquared(focalPoint);

    Vector3 collisionPoint;
    PhysicsCollisionObject* cameraOcclusion = Game::getInstance()->getPhysicsController()->rayTest(cameraRay, CAMERA_FOCUS_RANGE, &collisionPoint);
    bool cameraCollision = false;
    if (cameraOcclusion)
    {
        Vector3 rayStep = cameraRay.getDirection() * RAY_STEP_SIZE;
        do
        {
            float d2 = cameraRay.getOrigin().distanceSquared(collisionPoint);
            if (d2 > d)
                break; // collision point is past the character (not obstructing the view)

            cameraCollision = true;

            // Step along the camera ray closer to the character
            cameraRay.setOrigin(cameraRay.getOrigin() - rayStep);

            // Prevent camera from moving past character
            if (cameraRay.getOrigin().distanceSquared(focalPoint) < (RAY_STEP_SIZE*RAY_STEP_SIZE))
                break;
        }
        while ((cameraOcclusion = Game::getInstance()->getPhysicsController()->rayTest(cameraRay, CAMERA_FOCUS_RANGE, &collisionPoint)));
    }

    if (cameraCollision)
    {
        // Move camera
        float moveDistance = cameraPosition.distance(cameraRay.getOrigin());
        //node->setTranslation(0, 0, 0);
        node->translateForward(moveDistance);
        cameraOffset += moveDistance;
    }
    else
    {
        // Reset camera
        if (cameraOffset != 0.0f)
        {
            node->translateForward(-cameraOffset);
            cameraOffset = 0.0f;

            // Call updateCamera again to ensure that moving back didn't cause a different
            // object to obstruct the view.
            fixCamera(elapsedTime);
        }
    }
}

void CharacterGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->begin();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->end();
    SAFE_DELETE(batch);
}
