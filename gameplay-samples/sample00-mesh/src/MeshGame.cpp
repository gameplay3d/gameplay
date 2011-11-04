/*
 * MeshGame
 */

#include "MeshGame.h"

MeshGame game;


MeshGame::~MeshGame()
{
}

void MeshGame::initialize()
{
    _touched = false;

    // Initialize the render state
    glClearColor(0, 0, 0, 1);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // Load mesh/scene from file
    Package* meshPackage = Package::create("res/models/duck.gpb");
    _scene = meshPackage->loadScene();
    assert(_scene);

    Camera* camera = _scene->getActiveCamera();
    if (!camera)
    {
        createDefaultCamera(_scene);
    }
    
    _scene->visit(this, &MeshGame::getModelNode);
    assert(_modelNode);

    _modelNode->getModel()->setMaterial("res/materials/duck.material");
    Vector3 lightDirection(0.0f, 0.0f, 1.0f);
    _scene->getActiveCamera()->getViewMatrix().transformNormal(&lightDirection);
    _modelNode->getModel()->getMaterial()->getParameter("u_lightDirection")->setValue(lightDirection);

    // Load font
    Package* fontPackage = Package::create("res/fonts/arial16.gpb");
    _font = fontPackage->loadFont("arial16");

    SAFE_RELEASE(fontPackage);
    SAFE_RELEASE(meshPackage);
}

void MeshGame::finalize()
{
}

void MeshGame::update(long elapsedTime)
{
    if (!_touched)
    {
        _modelNode->rotateY(MATH_DEG_TO_RAD(0.5f));
    }
}

void MeshGame::render(long elapsedTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our scene
    _scene->visit(this, &MeshGame::visitNode);

    // Draw the FPS
    char fps[10];
    sprintf(fps, "%u", Game::getFrameRate()); 
    _font->begin();
    _font->drawText(fps, 5, 5, Vector4(1, 0, 0, 1));
    _font->end();
}

void MeshGame::touch(int x, int y, int touchEvent)
{
    switch (touchEvent)
    {
    case Input::TOUCHEVENT_PRESS:
        {
            _touched = true;
            _prevX = x;
            _prevY = y;
        }
        break;
    case Input::TOUCHEVENT_RELEASE:
        {
            _touched = false;
            _prevX = 0;
            _prevY = 0;
        }
        break;
    case Input::TOUCHEVENT_MOVE:
        {
            int deltaX = x - _prevX;
            int deltaY = y - _prevY;
            _prevX = x;
            _prevY = y;
            _modelNode->rotateY(MATH_DEG_TO_RAD(deltaX * 0.5f));
            //_modelNode->rotateX(MATH_DEG_TO_RAD(deltaY * 0.5f));
        }
        break;
    default:
        break;
    };
}

void MeshGame::visitNode(Node* node, long cookie)
{
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw();
    }
}

void MeshGame::getModelNode(Node* node, long cookie)
{
    if (!_modelNode)
    {
        Model* model = node->getModel();
        if (model)
        {
            _modelNode = node;
        }
    }
}

void MeshGame::createDefaultCamera(Scene* scene)
{
    // create the camera and node for it
    Node* node = scene->addNode("camera");
    Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 1.0f, 100.0f);
    node->setCamera(camera);
    node->translate(0, 5.0f, 20.0f);
    scene->setActiveCamera(camera);
}