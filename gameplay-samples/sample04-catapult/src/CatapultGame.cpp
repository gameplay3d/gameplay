/*
 * CatapultGame
 */

#include "CatapultGame.h"

CatapultGame game;
static float f = 0.5f, l = 0.1f, a = 0.5f;//l = 0.04f, a = 0.1f;
static const float FORCE_VALUE = 20.0f;
static const float TORQUE_VALUE = 10.0f;

void setMaterialHelper(const char* vs, const char* fs, Node* node, const Vector3& lightDirection, bool skinning, unsigned int jointCount)
{
    if (skinning)
    {
        char defines[64] = {'\0'};
        sprintf(defines, "#define SKINNING \n #define SKINNING_JOINT_COUNT %d", jointCount);
        node->getModel()->setMaterial(vs, fs, defines);
        node->getModel()->getMaterial()->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
    }
    else
        node->getModel()->setMaterial(vs, fs);

    node->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);
    node->getModel()->getMaterial()->getParameter("u_lightColor")->setValue(Vector3(0.75f, 0.75f, 0.75f));
    node->getModel()->getMaterial()->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
    node->getModel()->getMaterial()->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
    node->getModel()->getMaterial()->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);
}

void setColoredMaterial(Node* node, const Vector3& lightDirection, Vector4 color = Vector4(1.0f, 0.0f, 0.0f, 1.0f), bool skinning = false, unsigned int jointCount = 0)
{
    setMaterialHelper("res/shaders/colored.vsh", "res/shaders/colored.fsh", node, lightDirection, skinning, jointCount);
    node->getModel()->getMaterial()->getParameter("u_diffuseColor")->setValue(color);
}

void setDiffuseMaterial(Node* node, const Vector3& lightDirection, const char* texturePath,
    bool skinning = false, unsigned int jointCount = 0)
{
    setMaterialHelper("res/shaders/diffuse.vsh", "res/shaders/diffuse.fsh", node, lightDirection, skinning, jointCount);
    node->getModel()->getMaterial()->getParameter("u_diffuseTexture")->setValue(texturePath, false);
}

void addP2PConstraint(Node* a, Node* b)
{
    const BoundingBox& boxA = a->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInA(0.5f * (boxA.max.x - boxA.min.x), 0.0f, 0.0f);
    const BoundingBox& boxB = b->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInB(-0.5f * (boxB.max.x - boxB.min.x), 0.0f, 0.0f);

    PhysicsController* physics = Game::getInstance()->getPhysicsController();
    physics->createSocketConstraint(a->getPhysicsRigidBody(), pivotInA, b->getPhysicsRigidBody(), pivotInB);
}

Vector3 getWorldCenterOfMass(Model* model)
{
    Vector3 center;

    const BoundingBox& box = model->getMesh()->getBoundingBox();
    if (!(box.min.isZero() && box.max.isZero()))
    {
        Vector3 bMin, bMax;
        model->getNode()->getWorldMatrix().transformPoint(box.min, &bMin);
        model->getNode()->getWorldMatrix().transformPoint(box.max, &bMax);
        center.set(bMin, bMax);
        center.scale(0.5f);
        center.add(bMin);
    }
    else
    {
        const BoundingSphere& sphere = model->getMesh()->getBoundingSphere();
        if (!(sphere.center.isZero() && sphere.radius == 0))
        {
            model->getNode()->getWorldMatrix().transformPoint(sphere.center, &center);
        }
    }

    // TODO: This case needs to be fixed (maybe return an error somehow?).
    return center;
}

Vector3 getVectorBetweenObjects(Node* a, Node* b)
{
    Vector3 cA, cB;

    if (!a->getModel() || !b->getModel())
        return Vector3();
    
    return Vector3(getWorldCenterOfMass(a->getModel()), getWorldCenterOfMass(b->getModel()));
}

Vector3 getMidpointBetweenObjects(Node* a, Node* b)
{
    if (!a->getModel())
        return Vector3();

    Vector3 tA, tB;
    a->getWorldMatrix().getTranslation(&tA);
    b->getWorldMatrix().getTranslation(&tB);
    Vector3 d(tA, tB);
    //Vector3 c = getWorldCenterOfMass(a->getModel());
    //Vector3 d = getVectorBetweenObjects(a, b);

    d.scale(0.5f);
    Vector3 c(tA);
    c.add(d);

    return c;
}

Vector3 worldToObject(const Vector3& v, const float world[16])
{
    float offs[3];
	
    offs[0] = v.x - world[12];
    offs[1] = v.y - world[13];
    offs[2] = v.z - world[14];
	
    float result[3] = {0};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i] += world[i * 4 + j] * offs[j];
		}
	}
	return Vector3(result[0], result[1], result[2]);
}

Vector3 getTranslationOffset(Node* a, const Vector3& d, bool negate = false)
{
    Quaternion q;
    a->getWorldMatrix().getRotation(&q);
    Vector3 v;
    a->getWorldMatrix().getTranslation(&v);
    Transform t(Vector3::one(), q, v);
    Matrix w1(t.getMatrix());
    Matrix w1i; w1.invert(&w1i);
    Vector3 to(d);
    
    if (negate)
        to.negate();

    w1i.transformVector(&to);
    //to.x *= 1.0f / a->getScaleX();
    //to.y *= 1.0f / a->getScaleY();
    //to.z *= 1.0f / a->getScaleZ();

    return to;
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

    if (keyEvent == gameplay::Input::KEYEVENT_DOWN)
    {
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
            case Input::KEY_SPACE:
                if (Game::getInstance()->getState() == Game::PAUSED)
                    Game::getInstance()->resume();
                else
                    Game::getInstance()->pause();
                break;
            case Input::KEY_ESCAPE:
                Game::getInstance()->exit();
                break;
            case Input::KEY_C:
            {
                /*
                Vector3 d = getVectorBetweenObjects(_dieTop, _dieBottom);
                d.scale(0.5f);

                Vector3 toA(d);
                Vector3 toB(d);
                toB.negate();
                //toA = getTranslationOffset(_dieTop, d, true);
                //toB = getTranslationOffset(_dieBottom, d);

                //toA.set(0.0f, 0.0f, -3.0f);
                //toB.set(0.0f, 0.0f, 3.0f);

                PhysicsSpringConstraint* springConstraint = Game::getInstance()->getPhysicsController()->createSpringConstraint(_dieTop->getPhysicsRigidBody(), Quaternion::identity(),
                    Vector3(), _dieBottom->getPhysicsRigidBody(), Quaternion::identity(), Vector3());
    
                //springConstraint->setLinearStrengthZ(5.0f);
                //d.scale(2.0f);
                //springConstraint->setLinearLowerLimit(d);
                //d.scale(2.0f);
                //springConstraint->setLinearUpperLimit(d);
                //springConstraint->setAngularLowerLimit(Vector3());
                //springConstraint->setAngularUpperLimit(Vector3());
                //*/
            }
        }
    }
}

void CatapultGame::initialize()
{
    // Initialize GL state.
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

    Vector3 lightDirection(0.0f, 0.0f, -1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformVector(&lightDirection);

    Game::getPhysicsController()->setGravity(Vector3(0.0f, 0.0f, -9.8f));
    
    setDiffuseMaterial(_ground, lightDirection, "res/textures/ground.png");
    setDiffuseMaterial(_wallWest, lightDirection, "res/textures/wall_w_e.png");
    setDiffuseMaterial(_wallEast, lightDirection, "res/textures/wall_w_e.png");
    setDiffuseMaterial(_wallSouth, lightDirection, "res/textures/wall_n_s.png");
    setDiffuseMaterial(_wallNorth, lightDirection, "res/textures/wall_n_s.png");
    setDiffuseMaterial(_dieBottom, lightDirection, "res/textures/die.png");
    setDiffuseMaterial(_dieTop, lightDirection, "res/textures/die.png");
    //setDiffuseMaterial(_rope, lightDirection, "res/textures/rope_texture.png", true, 11);
    setDiffuseMaterial(_basketball, lightDirection, "res/textures/basketball.png");
    setDiffuseMaterial(_volleyball, lightDirection, "res/textures/volleyball.png");
    
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
    
    setColoredMaterial(joint1, lightDirection, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
    setColoredMaterial(joint2, lightDirection);
    setColoredMaterial(joint3, lightDirection);
    setColoredMaterial(joint4, lightDirection);
    setColoredMaterial(joint5, lightDirection);
    setColoredMaterial(joint6, lightDirection);
    setColoredMaterial(joint7, lightDirection);
    setColoredMaterial(joint8, lightDirection);
    setColoredMaterial(joint9, lightDirection);
    setColoredMaterial(joint10, lightDirection, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

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

    joint1->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint2->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint3->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint4->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint5->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint6->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint7->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint8->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint9->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);
    joint10->setPhysicsRigidBody(PhysicsRigidBody::PHYSICS_SHAPE_BOX, 0.1f, f, 0.0, l, a);

    const BoundingBox& boxA = joint1->getModel()->getMesh()->getBoundingBox();
    Vector3 pivotInA(-0.5f * (boxA.max.x - boxA.min.x), 0.0f, 0.0f);
    Game::getPhysicsController()->createSocketConstraint(joint1->getPhysicsRigidBody(), pivotInA);

    addP2PConstraint(joint1, joint2);
    addP2PConstraint(joint2, joint3);
    addP2PConstraint(joint3, joint4);
    addP2PConstraint(joint4, joint5);
    addP2PConstraint(joint5, joint6);
    addP2PConstraint(joint6, joint7);
    addP2PConstraint(joint7, joint8);
    addP2PConstraint(joint8, joint9);
    addP2PConstraint(joint9, joint10);
    
    Game::getInstance()->getPhysicsController()->createFixedConstraint(joint10->getPhysicsRigidBody(), _basketball->getPhysicsRigidBody());
    
    //PhysicsSpringConstraint* spring = Game::getInstance()->getPhysicsController()->createSpringConstraint(_dieTop->getPhysicsRigidBody(), _dieBottom->getPhysicsRigidBody());
    //spring->setLinearDampingX(0.2f);
    //spring->setLinearStrengthX(2.0f);


    _volleyball = _basketball;
    
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
    // TODO: Remove this!
    static int d = 0;
    d++;
    if (d == 4)
    {
        //pause();
    }

    // Query the accelerometer.
    static float pitch, roll;
    static float lastPitch = 0, lastRoll = 0;
    Input::getAccelerometerPitchAndRoll(&pitch, &roll);

    /*
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
    _font->drawText(fps, 5, 5, Vector4(1, 0, 0, 1));
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
