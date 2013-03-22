#include "PhysicsCollisionObjectSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Physics", "Collision Objects", PhysicsCollisionObjectSample, 1);
#endif

PhysicsCollisionObjectSample::PhysicsCollisionObjectSample()
    : _font(NULL), _scene(NULL), _lightNode(NULL), _form(NULL), _objectType(SPHERE), _throw(true), _drawDebug(0), _wireFrame(false)
{
    const char* paths[] = {"res/common/physics.physics#ball","res/common/physics.physics#box", "res/common/physics.physics#capsule", "res/common/physics.physics#duck"};
    _collisionObjectPaths.assign(paths, paths + 4);
    const char* nodeIds[] = {"sphere", "box", "capsule", "duck"};
    _nodeIds.assign(nodeIds, nodeIds + 4);
    const char* nodeNames[] = {"Sphere", "Box", "Capsule", "Duck"};
    _nodeNames.assign(nodeNames, nodeNames + 4);
    Vector4 colors[] = {Vector4(1, 0, 0, 1), Vector4(0.1f, 0.6f, 0.1f, 1), Vector4(0, 0, 1, 1), Vector4(1, 1, 0, 1)};
    _colors.assign(colors, colors + 4);
}

void PhysicsCollisionObjectSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    _scene = Scene::load("res/common/physics.scene");
    // Use the aspect ratio of the display instead of the aspect ratio defined in the scene file.
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    _lightNode = _scene->findNode("directionalLight");
    _scene->visit(this, &PhysicsCollisionObjectSample::bindLights);

    _form = Form::create("res/common/physics.form");
    static_cast<Button*>(_form->getControl("wireframeButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("drawDebugButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("throwButton"))->addListener(this, Control::Listener::CLICK);
    Button* shapeButton = static_cast<Button*>(_form->getControl("shapeButton"));
    shapeButton->addListener(this, Control::Listener::CLICK);
    shapeButton->setTextColor(_colors[_objectType]);
}

void PhysicsCollisionObjectSample::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
}

void PhysicsCollisionObjectSample::update(float elapsedTime)
{
}

void PhysicsCollisionObjectSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &PhysicsCollisionObjectSample::drawScene);

    if (_drawDebug == 1)
    {
        // Draw the physics debug information.
        getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
    }

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _form->draw();
}

void PhysicsCollisionObjectSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner.
            setVsync(!isVsync());
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

void PhysicsCollisionObjectSample::keyEvent(Keyboard::KeyEvent evt, int key)
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
            toggleWireframe();
            break;
        }
    }
}

bool PhysicsCollisionObjectSample::drawScene(Node* node)
{
    Model* model = node->getModel();
    if (model)
        model->draw(_wireFrame);
    return true;
}

bool PhysicsCollisionObjectSample::bindLights(Node* node)
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

void PhysicsCollisionObjectSample::fireProjectile(const Ray& ray)
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

void PhysicsCollisionObjectSample::incrementDebugDraw()
{
    _drawDebug = (_drawDebug + 1) % 2;
    static_cast<Button*>(_form->getControl("drawDebugButton"))->setText(_drawDebug == 0 ? "Normal" : "Debug");
}

void PhysicsCollisionObjectSample::toggleWireframe()
{
    _wireFrame = !_wireFrame;
    static_cast<Button*>(_form->getControl("wireframeButton"))->setText(_wireFrame ? "Wireframe" : "Solid");
}

void PhysicsCollisionObjectSample::controlEvent(Control* control, EventType evt)
{
    Button* button = static_cast<Button*>(control);

    if (strcmp(button->getId(), "wireframeButton") == 0)
    {
        toggleWireframe();
        
    }
    else if (strcmp(button->getId(), "drawDebugButton") == 0)
    {
        incrementDebugDraw();
    }
    else if (strcmp(button->getId(), "throwButton") == 0)
    {
        _throw = !_throw;
        button->setText(_throw ? "Throw" : "Drop");
    }
    else if (strcmp(button->getId(), "shapeButton") == 0)
    {
        if (_objectType == SPHERE)
            _objectType = BOX;
        else if (_objectType == BOX)
            _objectType = CAPSULE;
        else if (_objectType == CAPSULE)
            _objectType = DUCK;
        else
            _objectType = SPHERE;

        button->setText(_nodeNames[_objectType]);
        button->setTextColor(_colors[_objectType]);
    }
}