#include "SandboxGame.h"

// Define this to 1 to draw the rope's individual rigid bodies.
#define DRAW_ROPE_RIGID_BODIES 0

// Declare our game instance
SandboxGame game;

static float f = 0.5f, l = 0.1f, a = 0.5f;
static float ACCELEROMETER_SCALE = 0.75f;
static const float FORCE_VALUE = 40.0f;
static const float TORQUE_VALUE = 20.0f;

// Simple helper function for setting the material on a node using a global direction light.
void setMaterialHelper(Node* node, const char* material, const Vector3& lightDirection)
{
    node->getModel()->setMaterial(material);
    node->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
}

// Creates a socket constraint between two of the rope's rigid bodies.
void createSocketConstraintForRope(Node* a, Node* b)
{
    const BoundingBox& boxA = a->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInA(0.5f * (boxA.max.x - boxA.min.x), 0.0f, 0.0f);
    const BoundingBox& boxB = b->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInB(-0.5f * (boxB.max.x - boxB.min.x), 0.0f, 0.0f);

    Game::getInstance()->getPhysicsController()->createSocketConstraint(a->getPhysicsRigidBody(), pivotInA, b->getPhysicsRigidBody(), pivotInB);
}

SandboxGame::~SandboxGame()
{
    // DUMMY FUNCTION
}

void SandboxGame::touch(int x, int y, int touchEvent)
{
    // DUMMY FUNCTION
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
            case Input::KEY_T:
                WARN_VARG("Volleyball collides with door: %d\n", _volleyball->getPhysicsRigidBody()->collidesWith(_door->getPhysicsRigidBody()));
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

    // Load mesh from file.
    Package* meshPackage = Package::create("res/models/physics_sandbox.gpb");

    _scene = meshPackage->loadScene();

    Node* ground = _scene->findNode("ground");
    Node* walls = _scene->findNode("walls");
    Node* wallWest = _scene->findNode("wall_west");
    Node* wallEast = _scene->findNode("wall_east");
    Node* wallSouth = _scene->findNode("wall_south");
    Node* wallNorth = _scene->findNode("wall_north");
    Node* dieBottom = _scene->findNode("die_bottom");
    Node* dieTop = _scene->findNode("die_top");
    Node* rope = _scene->findNode("rope");
    Node* door = _scene->findNode("door");
    Node* basketball = _scene->findNode("basketball");
    _volleyball = _scene->findNode("volleyball");

    assert(ground);
    assert(walls);
    assert(wallWest);
    assert(wallEast);
    assert(wallSouth);
    assert(wallNorth);
    assert(dieBottom);
    assert(dieTop);
    assert(rope);
    assert(door);
    assert(basketball);
    assert(_volleyball);

    Camera* camera = _scene->getActiveCamera();
    if (!camera)
    {
        // Create the camera and node for it.
        Node* node = _scene->addNode("camera");
        assert(node);
        node->translate(0, 10.0f, 20.0f);
        float aspect = (float)getWidth() / (float)getHeight();
        Camera* camera = Camera::createPerspective(45.0f, aspect, 1.0f, 100.0f);
        assert(camera);
        node->setCamera(camera);
        _scene->addNode(node);
        _scene->setActiveCamera(camera);
    }

    Vector3 lightDirection(0.0f, 0.0f, -1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformVector(&lightDirection);

    Game::getPhysicsController()->setGravity(Vector3(0.0f, 0.0f, -9.8f));
    
    setMaterialHelper(ground, "res/materials/ground.material", lightDirection);
    setMaterialHelper(wallWest, "res/materials/wall_w_e.material", lightDirection);
    setMaterialHelper(wallEast, "res/materials/wall_w_e.material", lightDirection);
    setMaterialHelper(wallSouth, "res/materials/wall_n_s.material", lightDirection);
    setMaterialHelper(wallNorth, "res/materials/wall_n_s.material", lightDirection);
    setMaterialHelper(dieBottom, "res/materials/die.material", lightDirection);
    setMaterialHelper(dieTop, "res/materials/die.material", lightDirection);
    setMaterialHelper(rope, "res/materials/rope.material", lightDirection);
    setMaterialHelper(door, "res/materials/door.material", lightDirection);
    setMaterialHelper(basketball, "res/materials/basketball.material", lightDirection);
    setMaterialHelper(_volleyball, "res/materials/volleyball.material", lightDirection);
    
    // Set up the static physics objects.
    ground->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    wallWest->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    wallEast->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    wallSouth->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    wallNorth->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);

    // Set up the dynamic physics objects.
    dieTop->translate(0.0f, 0.0f, 10.0f);
    dieBottom->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 1.0f, f, 0.0f, l, a);
    dieTop->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 1.0f, f, 0.0f, l, a);
    door->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 1.0f, f, 0.0f, l, a);
    basketball->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_SPHERE, 1.0f, f, 0.0f, l, a);

    _volleyball->translate(0.0f, 0.0f, 10.0f);
    _volleyball->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_SPHERE, 1.0f, f, 0.0f, l, a);
    
    // Set up the spring constraint between the two dice.
    PhysicsSpringConstraint* spring = Game::getInstance()->getPhysicsController()->createSpringConstraint(dieTop->getPhysicsRigidBody(), dieBottom->getPhysicsRigidBody());
    spring->setAngularLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
    spring->setAngularUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
    spring->setLinearLowerLimit(Vector3(0.0f, 0.0f, -10.0f));
    spring->setLinearUpperLimit(Vector3(0.0f, 0.0f, 8.0f));
    spring->setLinearDampingZ(0.6f);
    spring->setLinearStrengthZ(5.0f);

    // Set up the hinge constraint for the door and allow it to be broken.
    BoundingBox doorBox = door->getModel()->getMesh()->getBoundingBox();
    float x = 0.5f * (doorBox.max.z - doorBox.min.z);
    Quaternion doorRot;
    Quaternion::createFromAxisAngle(Vector3::unitY(), MATH_DEG_TO_RAD(90.0f), &doorRot);
    PhysicsHingeConstraint* hinge = Game::getInstance()->getPhysicsController()->createHingeConstraint(door->getPhysicsRigidBody(), doorRot, Vector3(0.0f, 0.0f, x));
    hinge->setLimits(0.0f, MATH_DEG_TO_RAD(90.0f), 0.5f);
    hinge->setBreakingImpulse(80.0f);    

    // Add the joint rigid body meshes to the scene under each corresponding joint node.
    Node* joint1 = _scene->findNode("joint1");
    Node* joint2 = _scene->findNode("joint2");
    Node* joint3 = _scene->findNode("joint3");
    Node* joint4 = _scene->findNode("joint4");
    Node* joint5 = _scene->findNode("joint5");
    Node* joint6 = _scene->findNode("joint6");
    Node* joint7 = _scene->findNode("joint7");
    Node* joint8 = _scene->findNode("joint8");
    Node* joint9 = _scene->findNode("joint9");
    Node* joint10 = _scene->findNode("joint10");

    assert(joint1);
    assert(joint2);
    assert(joint3);
    assert(joint4);
    assert(joint5);
    assert(joint6);
    assert(joint7);
    assert(joint8);
    assert(joint9);
    assert(joint10);
    
    Node* joint1_body = _scene->findNode("joint1_body");
    Node* joint2_body = _scene->findNode("joint2_body");
    Node* joint3_body = _scene->findNode("joint3_body");
    Node* joint4_body = _scene->findNode("joint4_body");
    Node* joint5_body = _scene->findNode("joint5_body");
    Node* joint6_body = _scene->findNode("joint6_body");
    Node* joint7_body = _scene->findNode("joint7_body");
    Node* joint8_body = _scene->findNode("joint8_body");
    Node* joint9_body = _scene->findNode("joint9_body");
    Node* joint10_body = _scene->findNode("joint10_body");

    assert(joint1_body);
    assert(joint2_body);
    assert(joint3_body);
    assert(joint4_body);
    assert(joint5_body);
    assert(joint6_body);
    assert(joint7_body);
    assert(joint8_body);
    assert(joint9_body);
    assert(joint10_body);
    
    joint1->setModel(joint1_body->getModel());
    joint2->setModel(joint2_body->getModel());
    joint3->setModel(joint3_body->getModel());
    joint4->setModel(joint4_body->getModel());
    joint5->setModel(joint5_body->getModel());
    joint6->setModel(joint6_body->getModel());
    joint7->setModel(joint7_body->getModel());
    joint8->setModel(joint8_body->getModel());
    joint9->setModel(joint9_body->getModel());
    joint10->setModel(joint10_body->getModel());
    
#if DRAW_ROPE_RIGID_BODIES
    setMaterialHelper(joint1, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint2, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint3, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint4, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint5, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint6, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint7, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint8, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint9, "res/materials/rigidbodydebug.material", lightDirection);
    setMaterialHelper(joint10, "res/materials/rigidbodydebug.material", lightDirection);
#endif

    joint1->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint2->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint3->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint4->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint5->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint6->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint7->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint8->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint9->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint10->setPhysicsRigidBody(PhysicsRigidBody::SHAPE_BOX, 0.1f, f, 0.0, l, a);

    // Create the socket constraint that binds the top of the rope in place.
    const BoundingBox& boxA = joint1->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInA(-0.5f * (boxA.max.x - boxA.min.x), 0.0f, 0.0f);
    Game::getPhysicsController()->createSocketConstraint(joint1->getPhysicsRigidBody(), pivotInA);

    // Create the socket constraints for all the individual links in the rope.
    createSocketConstraintForRope(joint1, joint2);
    createSocketConstraintForRope(joint2, joint3);
    createSocketConstraintForRope(joint3, joint4);
    createSocketConstraintForRope(joint4, joint5);
    createSocketConstraintForRope(joint5, joint6);
    createSocketConstraintForRope(joint6, joint7);
    createSocketConstraintForRope(joint7, joint8);
    createSocketConstraintForRope(joint8, joint9);
    createSocketConstraintForRope(joint9, joint10);
    
    // Create the socket constraint that binds the basketball to the end of the rope.
    BoundingBox b = joint10->getModel()->getMesh()->getBoundingBox();
    BoundingSphere s = basketball->getModel()->getMesh()->getBoundingSphere();
    Game::getInstance()->getPhysicsController()->createSocketConstraint(joint10->getPhysicsRigidBody(), Vector3(0.5f * (b.max.x - b.min.x), 0.0f, 0.0f),
        basketball->getPhysicsRigidBody(), Vector3(0.0f, -s.radius, 0.0f));
    
    // Load the font.
    Package* fontPackage = Package::create("res/fonts/arial16.gpb");
    _font = fontPackage->loadFont("arial16");

    // Register the game as a listener for physics status events
    // and for collisions between the volleyball and the door.
    Game::getInstance()->getPhysicsController()->addStatusListener((SandboxGame*)Game::getInstance());
    _volleyball->getPhysicsRigidBody()->addCollisionListener((SandboxGame*)Game::getInstance());

    _door = door;

    SAFE_RELEASE(fontPackage);
    SAFE_RELEASE(meshPackage);
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
    _scene->visit(this, &SandboxGame::visitNode);

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
