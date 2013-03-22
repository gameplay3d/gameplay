#include "TextureSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Textures", TextureSample, 6);
#endif

Node* addQuadModelAndNode(Scene* scene, Mesh* mesh)
{
    Model* model = Model::create(mesh);
    Node* node = scene->addNode();
    node->setModel(model);
    SAFE_RELEASE(model);
    return node;
}

Node* addQuadModelAndNode(Scene* scene, float x, float y, float width, float height, 
                          float s1 = 0.0f, float t1 = 0.0f, float s2 = 1.0f, float t2 = 1.0f)
{
    Mesh* mesh = Mesh::createQuad(x, y, width, height, s1, t1, s2, t2);
    Node* node = addQuadModelAndNode(scene, mesh);
    SAFE_RELEASE(mesh);
    return node;
}

Material* setTextureUnlitMaterial(Model* model, const char* texturePath, bool mipmap = true)
{
    Material* material = model->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag");
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");

    // Load the texture from file.
    Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue(texturePath, mipmap);
    if (mipmap)
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    else
        sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
    sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);
    return material;
}

TextureSample::TextureSample()
    : _font(NULL), _scene(NULL), _zOffset(0.0f)
{   
}

void TextureSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    // Create an empty scene.
    _scene = Scene::create();

    // Create a camera and add it to the scene as the active camera.
    Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1, 1000);
    Node* cameraNode = _scene->addNode("camera");
    cameraNode->setCamera(camera);
    _scene->setActiveCamera(camera);
    cameraNode->translate(0, 0, 50);
    SAFE_RELEASE(camera);

    const float fontSize = _font->getSize();
    const float cubeSize = 10.0f;
    float x, y, textWidth;
    // Find the width of the cube in screen space
    _scene->getActiveCamera()->project(getViewport(), Vector3(cubeSize, 0, 0), &x, &y);
    textWidth = x - (getWidth() >> 1);
    // Textured quad mesh
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setTranslation(-25, cubeSize, 0);
        // Find the position of the node in screen space
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("Quad: Textured", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_TOP_HCENTER, false));
    }
    // Textured quad points
    {
        Mesh* mesh = Mesh::createQuad(Vector3(0, cubeSize, 0), Vector3(0, 0, 0), Vector3(cubeSize, cubeSize, 0), Vector3(cubeSize, 0, 0));
        Node* node = addQuadModelAndNode(_scene, mesh);
        SAFE_RELEASE(mesh);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setTranslation(-14, cubeSize, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("Quad: Points", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_TOP_HCENTER, false));
    }
    // Texture clamp
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize, -1, -1, 2, 2);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setId("clamp");
        node->setTranslation(-3, cubeSize, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("Wrap: Clamp", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_TOP_HCENTER, false));
    }
    // Texture wrapped+repeat
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize, -1, -1, 2, 2);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setId("repeat");
        Texture::Sampler* sampler = node->getModel()->getMaterial()->getParameter("u_diffuseTexture")->getSampler();
        if (sampler)
        {
            sampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
        }
        node->setTranslation(8, cubeSize, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("Wrap: Repeat", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
    // Mipmapping Off
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize);
        setTextureUnlitMaterial(node->getModel(), "res/png/box-diffuse.png", false);
        node->setId("mipmap off");
        node->setTranslation(-25.5f, -2.5f, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("MipMap: Off", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
    // Mipmapping On
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize);
        setTextureUnlitMaterial(node->getModel(), "res/png/box-diffuse.png");
        node->setId("mipmap on");
        node->setTranslation(-5.5f, -2.5f, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("MipMap: On", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
}

void TextureSample::finalize()
{
    // Model and font are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    for (std::list<Font::Text*>::iterator it = _text.begin(); it != _text.end(); ++it)
    {
        SAFE_DELETE(*it);
    }
    _text.clear();
}

void TextureSample::update(float elapsedTime)
{
    Node* n1 = _scene->findNode("mipmap on");
    Node* n2 = _scene->findNode("mipmap off");
    // move these nodes between 0 and -900 using a sine wave
    float z = -(sin((getAbsoluteTime() / 1500.0L) * MATH_PI) + 1)  * 900.0f / 2.0f;
    n1->setTranslationZ(z);
    n2->setTranslationZ(z);
}

void TextureSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &TextureSample::drawScene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _font->start();
    for (std::list<Font::Text*>::const_iterator it = _text.begin(); it != _text.end(); ++it)
    {
        _font->drawText(*it);
    }
    _font->finish();
}

void TextureSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

bool TextureSample::drawScene(Node* node)
{
    Model* model = node->getModel();
    if (model)
        model->draw();
    return true;
}
