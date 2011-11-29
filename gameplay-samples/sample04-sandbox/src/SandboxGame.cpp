#include "SandboxGame.h"

// Declare our game instance.
SandboxGame game;

static float f = 0.5f, l = 0.1f, a = 0.5f;
static float ACCELEROMETER_SCALE = 0.75f;
static const float FORCE_VALUE = 40.0f;
static const float TORQUE_VALUE = 20.0f;
static const float KINEMATIC_MOVEMENT_AMOUNT = 0.1f;

static bool debugDrawing = false;
static bool drawNormal = true;

SandboxGame::~SandboxGame()
{
    // Unused
}

void SandboxGame::touch(int x, int y, int touchEvent)
{
    // Unused
}

void SandboxGame::keyPress(int key, int keyEvent)
{
    static Vector3 force;
    static Vector3 torque;

    if (keyEvent == gameplay::Input::KEYEVENT_DOWN)
    {
        switch (key)
        {
            case Input::KEY_LEFT_ARROW:
                force.set(-FORCE_VALUE, 0.0f, 0.0f);
                torque.set(0.0f, -TORQUE_VALUE, 0.0f);
                _volleyball->getPhysicsRigidBody()->applyForce(force);
                _volleyball->getPhysicsRigidBody()->applyTorque(torque);
                break;
            case Input::KEY_RIGHT_ARROW:
                force.set(FORCE_VALUE, 0.0f, 0.0f);
                torque.set(0.0f, TORQUE_VALUE, 0.0f);
                _volleyball->getPhysicsRigidBody()->applyForce(force);
                _volleyball->getPhysicsRigidBody()->applyTorque(torque);
                break;
            case Input::KEY_UP_ARROW:
                force.set(0.0f, FORCE_VALUE, 0.0f);
                torque.set(-TORQUE_VALUE, 0.0f, 0.0f);
                _volleyball->getPhysicsRigidBody()->applyForce(force);
                _volleyball->getPhysicsRigidBody()->applyTorque(torque);
                break;
            case Input::KEY_DOWN_ARROW:
                force.set(0.0f, -FORCE_VALUE, 0.0f);
                torque.set(TORQUE_VALUE, 0.0f, 0.0f);
                _volleyball->getPhysicsRigidBody()->applyForce(force);
                _volleyball->getPhysicsRigidBody()->applyTorque(torque);
                break;
            case Input::KEY_SPACE:
                if (Game::getInstance()->getState() == Game::PAUSED)
                    Game::getInstance()->resume();
                else
                    Game::getInstance()->pause();
                break;
            case Input::KEY_K:
                _volleyball->getPhysicsRigidBody()->setKinematic(!_volleyball->getPhysicsRigidBody()->isKinematic());
                break;
            case Input::KEY_T:
                WARN_VARG("Volleyball collides with door: %d\n", _volleyball->getPhysicsRigidBody()->collidesWith(_door->getPhysicsRigidBody()));
                break;
            case Input::KEY_W:
                _volleyball->translateY(KINEMATIC_MOVEMENT_AMOUNT);
                break;
            case Input::KEY_S:
                _volleyball->translateY(-KINEMATIC_MOVEMENT_AMOUNT);
                break;
            case Input::KEY_A:
                _volleyball->translateX(-KINEMATIC_MOVEMENT_AMOUNT);
                break;
            case Input::KEY_D:
                _volleyball->translateX(KINEMATIC_MOVEMENT_AMOUNT);
                break;
            case Input::KEY_B:
                debugDrawing = !debugDrawing;
                break;
            case Input::KEY_N:
                drawNormal = !drawNormal;
                break;
            case Input::KEY_Q:
            case Input::KEY_ESCAPE:
                Game::getInstance()->exit();
                break;
        }
    }
}

void SandboxGame::initialize()
{
    // Depending on the orientation, possibly invert the accelerometer scale.
    if (Platform::getOrientationAngle() == 0)
        ACCELEROMETER_SCALE *= -1.0f;

    _scene = Scene::load("res/sandbox.scene");

    Vector3 lightDirection(0.0f, 0.0f, -1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformVector(&lightDirection);

    _scene->findNode("ground")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("wallWest")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("wallEast")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("wallSouth")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("wallNorth")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("die_bottom")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("die_top")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("rope")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _scene->findNode("basketball")->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);

    _door = _scene->findNode("door");
    _door->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _volleyball = _scene->findNode("volleyball");
    _volleyball->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);

    // Load the font.
    _font = Font::create("res/arial40.gpb");

    // Register the game as a listener for physics status events
    // and for collisions between the volleyball and the door.
    Game::getInstance()->getPhysicsController()->addStatusListener((SandboxGame*)Game::getInstance());
    _volleyball->getPhysicsRigidBody()->addCollisionListener((SandboxGame*)Game::getInstance());
}

void SandboxGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void SandboxGame::update(long elapsedTime)
{
    // Query the accelerometer.
    static float pitch, roll;
    static float lastPitch = 0, lastRoll = 0;
    Input::getAccelerometerPitchAndRoll(&pitch, &roll);

    // Apply force to the volleyball.
    static Vector3 force;
    static Vector3 torque;
    if (pitch - lastPitch > MATH_EPSILON || roll - lastRoll > MATH_EPSILON)
    {
        force.x = ACCELEROMETER_SCALE * FORCE_VALUE * sinf(2.0f * MATH_DEG_TO_RAD(roll));
        force.y = ACCELEROMETER_SCALE * FORCE_VALUE * sinf(2.0f * MATH_DEG_TO_RAD(pitch));
        force.z = 0.0;

        torque.x = -0.5 * force.y;
        torque.y = 0.5 * force.x;
    }
    _volleyball->getPhysicsRigidBody()->applyForce(force);
    _volleyball->getPhysicsRigidBody()->applyTorque(torque);

    lastPitch = pitch;
    lastRoll = roll;
}

void SandboxGame::render(long elapsedTime)
{
    this->clear(CLEAR_COLOR_DEPTH, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0.0f);

    // Draw our scene.
    if (drawNormal)
        _scene->visit(this, &SandboxGame::visitNode);

    // Debug draw the physics.
    if (debugDrawing)
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());

    // Draw the FPS.
    char fps[3];
    sprintf(fps, "%u", Game::getFrameRate()); 
    _font->begin();
    _font->drawText(fps, 5, 5, Vector4(0, 0.5f, 1, 1));
    _font->end();
}

void SandboxGame::visitNode(Node* node, long cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        model->draw();
    }
}

void SandboxGame::statusEvent(PhysicsController::Listener::EventType type)
{
    switch (type)
    {
    case PhysicsController::Listener::ACTIVATED:
        WARN("Physics is active.");
        break;
    case PhysicsController::Listener::DEACTIVATED:
        WARN("Physics is inactive.");
        break;
    }
}

void SandboxGame::collisionEvent(const PhysicsRigidBody::CollisionPair& pair, const Vector3& point)
{
    WARN_VARG("Collision between rigid bodies %s and %s at point (%f, %f, %f).",
        pair._rbA->getNode()->getId(), pair._rbB->getNode()->getId(), point.x, point.y, point.z);
}
