#include "CharacterGame.h"

// Declare our game instance
CharacterGame game; 

unsigned int keyFlags = 0;
float _rotateY = 0.0f;
#define WALK_SPEED  7.5f
#define ANIM_SPEED 10.0f
#define BLEND_DURATION 150.0f
#define CAMERA_FOCUS_RANGE 16.0f

bool drawDebug = false;
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
    // Display the gameplay splash screen for at least 2.4 seconds.
    displaySplash(this, &CharacterGame::drawSplash, NULL, 2400L);

    // Load the font.
    _font = Font::create("res/arial40.gpb");

    // Load scene.
    _scene = Scene::load("res/scene.scene");

    // Store character node.
	Node* node = _scene->findNode("BoyCharacter");
	node->setCollisionObject(PhysicsCollisionObject::CHARACTER, PhysicsCollisionShape::capsule(1.2f, 2.75f, Vector3(0, 2.25f, 0), true));
	_character = static_cast<PhysicsCharacter*>(node->getCollisionObject());
    _character->addCollisionListener(this);

	//node = _scene->findNode("Easel");
	//node->setCollisionObject(PhysicsCollisionObject::CHARACTER, PhysicsCollisionShape::capsule());
	//static_cast<PhysicsCharacter*>(node->getCollisionObject())->setPhysicsEnabled(false);

    // Initialize scene.
    _scene->visit(this, &CharacterGame::initScene);

    // Load animations clips.
    loadAnimationClips();
}

void CharacterGame::initMaterial(Scene* scene, Node* node, Material* material)
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
    // Update character animation and movement
    if (keyFlags == 0)
    {
        _character->play("idle", PhysicsCharacter::ANIMATION_REPEAT, 1.0f, BLEND_DURATION);
    }
    else
    {
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
    _scene->visit(this, &CharacterGame::drawScene);

    if (drawDebug)
		_scene->drawDebug(Scene::DEBUG_BOXES);
        //Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());

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
		case Keyboard::KEY_B:
			moveBall = !moveBall;
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

void CharacterGame::loadAnimationClips()
{
    _animation = Game::getInstance()->getAnimationController()->getAnimation("movements");
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
		if (collisionPair.objectB == _scene->findNode("PlayTable")->getCollisionObject())
        {
            int i = 0;
            i = 1;
            ++i;
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
