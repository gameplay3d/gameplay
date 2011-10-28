/*
 * CatapultGame
 */

#include "CatapultGame.h"

CatapultGame game;
static float f = 0.5f, l = 0.04f, a = 0.1f;
static const float FORCE_VALUE = 20.0f;
static const float TORQUE_VALUE = 10.0f;

void setColoredMaterial(Node* node, const Vector3& lightDirection)
{
    node->getModel()->setMaterial("res/shaders/colored.vsh", "res/shaders/colored.fsh");
    node->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    node->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    node->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    node->getModel()->getMaterial()->getParameter("u_diffuseColor")->setValue(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
}

CatapultGame::~CatapultGame()
{
}

void CatapultGame::touch(int x, int y, int touchEvent)
{
}

void CatapultGame::keyPress(int key, int keyEvent)
{
    static Vector3 force;
    static Vector3 torque;

    switch (key)
    {
    case Input::KEY_LEFT:
        force.set(-FORCE_VALUE, 0.0f, 0.0f);
        torque.set(0.0f, -TORQUE_VALUE, 0.0f);
        _volleyball->getPhysicsRigidBody()->applyForce(force);
        _volleyball->getPhysicsRigidBody()->applyTorque(torque);
        break;
    case Input::KEY_RIGHT:
        force.set(FORCE_VALUE, 0.0f, 0.0f);
        torque.set(0.0f, TORQUE_VALUE, 0.0f);
        _volleyball->getPhysicsRigidBody()->applyForce(force);
        _volleyball->getPhysicsRigidBody()->applyTorque(torque);
        break;
    case Input::KEY_UP:
        force.set(0.0f, FORCE_VALUE, 0.0f);
        torque.set(-TORQUE_VALUE, 0.0f, 0.0f);
        _volleyball->getPhysicsRigidBody()->applyForce(force);
        _volleyball->getPhysicsRigidBody()->applyTorque(torque);
        break;
    case Input::KEY_DOWN:
        force.set(0.0f, -FORCE_VALUE, 0.0f);
        torque.set(TORQUE_VALUE, 0.0f, 0.0f);
        _volleyball->getPhysicsRigidBody()->applyForce(force);
        _volleyball->getPhysicsRigidBody()->applyTorque(torque);
        break;
    case Input::KEY_ESCAPE:
        Game::getInstance()->exit();
        break;
    }
}

void bringJointsOver(Node* node, Scene* scene)
{
    Model* model = node->getModel();
    std::vector<Joint*>* joints = &model->getSkin()->_joints;
    for (unsigned int i = 0; i < joints->size(); i++)
    {
        const char* id = joints->at(i)->getId();
        joints->at(i) = (Joint*)scene->findNode(id);
    }
}

void CatapultGame::initialize()
{
    // Initialize GL state.
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /*
    // Load in rigid body meshes from file.
    Package* package = Package::create("res/models/physics_rope_joint_rigidbodies.gpb");

    Scene* rbScene = package->loadScene();
    Node* joint1_body = rbScene->findNode("joint1_body");
    Node* joint2_body = rbScene->findNode("joint2_body");
    Node* joint3_body = rbScene->findNode("joint3_body");
    Node* joint4_body = rbScene->findNode("joint4_body");
    Node* joint5_body = rbScene->findNode("joint5_body");
    Node* joint6_body = rbScene->findNode("joint6_body");
    Node* joint7_body = rbScene->findNode("joint7_body");
    Node* joint8_body = rbScene->findNode("joint8_body");
    Node* joint9_body = rbScene->findNode("joint9_body");
    Node* joint10_body = rbScene->findNode("joint10_body");

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

    SAFE_RELEASE(package);
    //*/

    // Load mesh from file.
    Package* meshPackage = Package::create("res/models/physics_test.gpb");

    _scene = meshPackage->loadScene();

    _ground = _scene->findNode("ground");
    _walls = _scene->findNode("walls");
    _wallWest = _scene->findNode("wall_west");
    _wallEast = _scene->findNode("wall_east");
    _wallSouth = _scene->findNode("wall_south");
    _wallNorth = _scene->findNode("wall_north");
    _dieBottom = _scene->findNode("die_bottom");
    _dieTop = _scene->findNode("die_top");
    _rope = _scene->findNode("rope");
    _basketball = _scene->findNode("basketball");
    _volleyball = _scene->findNode("volleyball");

    assert(_ground);
    assert(_walls);
    assert(_wallWest);
    assert(_wallEast);
    assert(_wallSouth);
    assert(_wallNorth);
    assert(_dieBottom);
    assert(_dieTop);
    assert(_rope);
    assert(_basketball);
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

    Vector3 lightDirection(0.0f, 0.0f, 1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformNormal(&lightDirection);

    Game::getPhysicsController()->setGravity(Vector3(0.0f, 0.0f, -9.8f));
    
    _ground->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _ground->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _ground->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _ground->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _ground->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/ground.png", false);

    _wallWest->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _wallWest->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _wallWest->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _wallWest->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _wallWest->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/wall_w_e.png", false);

    _wallEast->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _wallEast->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _wallEast->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _wallEast->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _wallEast->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/wall_w_e.png", false);

    _wallSouth->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _wallSouth->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _wallSouth->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _wallSouth->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _wallSouth->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/wall_n_s.png", false);

    _wallNorth->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _wallNorth->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _wallNorth->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _wallNorth->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _wallNorth->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/wall_n_s.png", false);

    _dieBottom->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _dieBottom->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _dieBottom->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _dieBottom->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _dieBottom->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/die.png", false);

    _dieTop->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _dieTop->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _dieTop->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _dieTop->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _dieTop->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/die.png", false);
    
    /*
    _rope->getModel()->setMaterial("res/shaders/skinning.vsh", "res/shaders/skinning.fsh", "#define JOINT_COUNT 11");
    _rope->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _rope->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _rope->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _rope->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/rope_texture.png", false);
    _rope->getModel()->getMaterial()->getParameter("u_specularExponent")->setValue(20.0f);
    _rope->getModel()->getMaterial()->getParameter("u_matrixPalette")->bindValue(_rope->getModel()->getSkin(), &MeshSkin::getMatrixPalette, &MeshSkin::getMatrixPaletteSize);
    */

    _basketball->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _basketball->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _basketball->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _basketball->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _basketball->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/basketball.png", false);

    _volleyball->getModel()->setMaterial("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh");
    _volleyball->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    _volleyball->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    _volleyball->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    _volleyball->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue("res/textures/volleyball.png", false);

    /*
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
    //*/

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

    /*
    joint1->setModel(joint1_body->getModel());// bringJointsOver(joint1, _scene);
    joint2->setModel(joint2_body->getModel());// bringJointsOver(joint2, _scene);
    joint3->setModel(joint3_body->getModel());// bringJointsOver(joint3, _scene);
    joint4->setModel(joint4_body->getModel());// bringJointsOver(joint4, _scene);
    joint5->setModel(joint5_body->getModel());// bringJointsOver(joint5, _scene);
    joint6->setModel(joint6_body->getModel());// bringJointsOver(joint6, _scene);
    joint7->setModel(joint7_body->getModel());// bringJointsOver(joint7, _scene);
    joint8->setModel(joint8_body->getModel());// bringJointsOver(joint8, _scene);
    joint9->setModel(joint9_body->getModel());// bringJointsOver(joint9, _scene);
    joint10->setModel(joint10_body->getModel());// bringJointsOver(joint10, _scene);
    */

    /*
    setColoredMaterial(joint1, lightDirection);
    setColoredMaterial(joint2, lightDirection);
    setColoredMaterial(joint3, lightDirection);
    setColoredMaterial(joint4, lightDirection);
    setColoredMaterial(joint5, lightDirection);
    setColoredMaterial(joint6, lightDirection);
    setColoredMaterial(joint7, lightDirection);
    setColoredMaterial(joint8, lightDirection);
    setColoredMaterial(joint9, lightDirection);
    setColoredMaterial(joint10, lightDirection);
    //*/

    setColoredMaterial(joint1_body, lightDirection);
    setColoredMaterial(joint2_body, lightDirection);
    setColoredMaterial(joint3_body, lightDirection);
    setColoredMaterial(joint4_body, lightDirection);
    setColoredMaterial(joint5_body, lightDirection);
    setColoredMaterial(joint6_body, lightDirection);
    setColoredMaterial(joint7_body, lightDirection);
    setColoredMaterial(joint8_body, lightDirection);
    setColoredMaterial(joint9_body, lightDirection);
    setColoredMaterial(joint10_body, lightDirection);

    // Set up the static physics objects.
    _ground->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    _wallWest->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    _wallEast->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    _wallSouth->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);
    _wallNorth->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.0, 0.5, 0.0, 0.025f, 0.16f);

    // Set up the dynamic physics objects.
    _dieBottom->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 1.0f, f, 0.0f, l, a);
    _dieTop->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 1.0f, f, 0.0f, l, a);

    _basketball->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_SPHERE, 1.0f, f, 0.0f, l, a);

    _volleyball->translate(0.0f, 0.0f, 10.0f);
    _volleyball->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_SPHERE, 1.0f, f, 0.0f, l, a);
    
    // Load the font.
    Package* fontPackage = Package::create("res/fonts/arial16.gpb");
    _font = fontPackage->loadFont("arial16");

    SAFE_RELEASE(fontPackage);
    SAFE_RELEASE(meshPackage);

    _scale = -10.0f;
}

void CatapultGame::finalize()
{
}

void CatapultGame::update(long elapsedTime)
{
    // Query the accelerometer.
    static float pitch, roll;
    static float lastPitch = 0, lastRoll = 0;
    Input::getAccelerometerPitchAndRoll(&pitch, &roll);

    //*
    // Apply force to the volleyball.
    static Vector3 force;
    static Vector3 torque;
    if (pitch - lastPitch > MATH_EPSILON || roll - lastRoll > MATH_EPSILON)
    {
        force.x = _scale * sinf(2.0f * MATH_DEG_TO_RAD(roll));
        force.y = _scale * sinf(2.0f * MATH_DEG_TO_RAD(pitch));
        force.z = 0.0;

        torque.x = -0.5 * force.y;
        torque.y = 0.5 * force.x;
    }
	_volleyball->getPhysicsRigidBody()->applyForce(force);
    _volleyball->getPhysicsRigidBody()->applyTorque(torque);
    //*/
    lastPitch = pitch;
    lastRoll = roll;
}

void CatapultGame::render(long elapsedTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our scene
    _scene->visit(this, &CatapultGame::visitNode);

    // Draw the FPS
    char fps[3];
    sprintf(fps, "%u", Game::getFrameRate()); 
    _font->begin();
    _font->drawText(fps, 5, 5, Color::red());
    _font->end();
}

void CatapultGame::visitNode(Node* node, long cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        model->draw();
    }
}
