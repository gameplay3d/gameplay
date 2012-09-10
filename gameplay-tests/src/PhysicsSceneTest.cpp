#include "PhysicsSceneTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Physics", "Physics Scene", PhysicsSceneTest, 1);
#endif

PhysicsSceneTest::PhysicsSceneTest()
    : _font(NULL), _scene(NULL), _lightNode(NULL), _objectType(SPHERE), _throw(true), _drawDebug(0), _wireFrame(false)
{
    const char* paths[] = {"res/common/physics.physics#ball","res/common/physics.physics#box", "res/common/physics.physics#capsule"};
    _collisionObjectPaths.assign(paths, paths + 3);
    const char* nodeIds[] = {"sphere", "box", "capsule"};
    _nodeIds.assign(nodeIds, nodeIds + 3);
    Vector4 colors[] = {Vector4(1, 0, 0, 1), Vector4(0.1f, 0.6f, 0.1f, 1), Vector4(0, 0, 1, 1)};
    _colors.assign(colors, colors + 3);
}

void PhysicsSceneTest::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    _scene = Scene::load("res/common/physics.scene");
    // Use the aspect ratio of the display instead of the aspect ratio defined in the scene file.
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    _lightNode = _scene->findNode("directionalLight");
    _scene->visit(this, &PhysicsSceneTest::bindLights);
}

void PhysicsSceneTest::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}

void PhysicsSceneTest::update(float elapsedTime)
{
}

void PhysicsSceneTest::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &PhysicsSceneTest::drawScene);

    if (_drawDebug == 1)
    {
        // Draw the physics debug information.
        getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
    }

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    int y = getHeight() - _font->getSize() * 4;
    _font->start();
    _font->drawText(_wireFrame ? "Wireframe" : "Solid", 0, y, Vector4::one(), _font->getSize());
    _font->drawText(_drawDebug == 0 ? "Normal" : "Debug", 0, y += _font->getSize(), Vector4::one(), _font->getSize());
    _font->drawText(_throw ? "Throw" : "Drop", 0, y += _font->getSize(), Vector4::one(), _font->getSize());
    _font->drawText(_nodeIds[_objectType], 0, getHeight() - _font->getSize(), _colors[_objectType], _font->getSize());
    _font->finish();
}

void PhysicsSceneTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    static const int optionWidth = 100;
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner.
            setVsync(!isVsync());
        }
        else if (x < optionWidth && y > (int)(getHeight() - _font->getSize()))
        {
            // Toggle the type of object to fire if the user touches the bottom left corner.
            if (_objectType == SPHERE)
                _objectType = BOX;
            else if (_objectType == BOX)
                _objectType = CAPSULE;
            else
                _objectType = SPHERE;
        }
        else if (x < optionWidth && y > (int)(getHeight() - _font->getSize() * 2))
        {
            // Toggle the throw type if the user touches the throw text in the bottom left corner.
            _throw = !_throw;
        }
        else if (x < optionWidth && y > (int)(getHeight() - _font->getSize() * 3))
        {
            incrementDebugDraw();
        }
        else if (x < optionWidth && y > (int)(getHeight() - _font->getSize() * 4))
        {
            _wireFrame = !_wireFrame;
        }
        else
        {
            // Fire a physics object into the scene.
            Ray ray;
            Camera* camera = _scene->getActiveCamera();
            camera->pickRay(getViewport(), x, y, &ray);
            fireProjectile(ray);
        }
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

void PhysicsSceneTest::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_B:
        case Keyboard::KEY_CAPITAL_B:
            incrementDebugDraw();
            break;
        case Keyboard::KEY_M:
        case Keyboard::KEY_CAPITAL_M:
            _wireFrame = !_wireFrame;
            break;
        }
    }
}

bool PhysicsSceneTest::drawScene(Node* node)
{
    Model* model = node->getModel();
    if (model)
        model->draw(_wireFrame);
    return true;
}

bool PhysicsSceneTest::bindLights(Node* node)
{
    Model* model = node->getModel();
    if (model)
    {
        Material* material = model->getMaterial();
        if (material)
        {
            MaterialParameter* ambientColorParam = material->getParameter("u_ambientColor");
            if (ambientColorParam)
            {
                ambientColorParam->setValue(_scene->getAmbientColor());
            }
            if (_lightNode && _lightNode->getLight())
            {
                MaterialParameter* lightDirectionParam = material->getParameter("u_lightDirection");
                MaterialParameter* lightColorParam = material->getParameter("u_lightColor");
                if (lightDirectionParam)
                {
                    lightDirectionParam->bindValue(_lightNode, &Node::getForwardVectorView);
                }
                if (lightColorParam)
                {
                    lightColorParam->setValue(_lightNode->getLight()->getColor());
                }
            }
        }
    }
    return true;
}

void PhysicsSceneTest::fireProjectile(const Ray& ray)
{
    Node* clone = _scene->findNode(_nodeIds[_objectType])->clone();
    clone->setRotation(Quaternion::identity());
    if (_throw)
    {
        clone->setTranslation(ray.getOrigin());
    }
    else
    {
        // Find the position where the pick ray intersects with the floor.
        float distance = ray.intersects(Plane(0, 1, 0, -0.5f));
        if (distance != Ray::INTERSECTS_NONE)
        {
            Vector3 position((ray.getDirection() * distance) + ray.getOrigin());
            position.y += 8.0f;
            clone->setTranslation(position);
        }
    }
    // It is important to set the transform before attaching the collision object because this rigid body is not kinematic.
    // Once the non-kinematic rigid body is attached, you should only move the object using forces.
    PhysicsCollisionObject* collisionObject = clone->setCollisionObject(_collisionObjectPaths[_objectType]);
    _scene->addNode(clone);
    if (_throw)
    {
        PhysicsRigidBody* rigidBody = static_cast<PhysicsRigidBody*>(collisionObject);
        Vector3 impulse(ray.getDirection());
        impulse.normalize();
        impulse.scale(50.0f * rigidBody->getMass());
        rigidBody->applyImpulse(impulse);
    }
    // Release the new cloned node because the scene now holds the reference to it.
    clone->release();
}

void PhysicsSceneTest::incrementDebugDraw()
{
    _drawDebug = (_drawDebug + 1) % 2;
}