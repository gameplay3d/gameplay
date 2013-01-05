#include "TextureTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Graphics", "Textures", TextureTest, 6);
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

TextureTest::TextureTest()
    : _font(NULL), _scene(NULL), _zOffset(0.0f)
{   
}

void TextureTest::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    // Create an empty scene.
    _scene = Scene::create();

    // Create a camera and add it to the scene as the active camera.
    Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1, 700);
    Node* cameraNode = _scene->addNode("camera");
    cameraNode->setCamera(camera);
    _scene->setActiveCamera(camera);
    cameraNode->translate(0, 0, 50);
    SAFE_RELEASE(camera);

    // Textured quad mesh
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, 10, 10);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setTranslation(-36, 10, 0);
        _text.push_back(_font->createText("Textured Quad", Rectangle(15, 190, 175, _font->getSize()), Vector4::one(), _font->getSize(), Font::ALIGN_TOP_HCENTER, false));
    }
    {
        Mesh* mesh = Mesh::createQuad(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(10, 10, 0), Vector3(10, 0, 0));
        Node* node = addQuadModelAndNode(_scene, mesh);
        SAFE_RELEASE(mesh);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setTranslation(-25, 10, 0);
        _text.push_back(_font->createText("createQuad points", Rectangle(205, 190, 175, _font->getSize()), Vector4::one(), _font->getSize(), Font::ALIGN_TOP_HCENTER, false));
    }
    // Texture wrap clamp test
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, 10, 10, -1, -1, 2, 2);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setId("clamp");
        node->setTranslation(-14, 10, 0);
        _text.push_back(_font->createText("Wrap: Clamp", Rectangle(396, 190, 175, _font->getSize()), Vector4::one(), _font->getSize(), Font::ALIGN_TOP_HCENTER, false));
    }
    // Texture wrap repeat test
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, 10, 10, -1, -1, 2, 2);
        setTextureUnlitMaterial(node->getModel(), "res/png/color-wheel.png");
        node->setId("repeat");
        Texture::Sampler* sampler = node->getModel()->getMaterial()->getParameter("u_diffuseTexture")->getSampler();
        if (sampler)
        {
            sampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
        }
        node->setTranslation(-3, 10, 0);
        _text.push_back(_font->createText("Wrap: Repeat", Rectangle(586, 190, 175, _font->getSize()), Vector4::one(), _font->getSize(), Font::ALIGN_HCENTER, false));
    }
    // mipmap test
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, 10, 10);
        setTextureUnlitMaterial(node->getModel(), "res/png/box-diffuse.png", false);
        node->setId("mipmap off");
        node->setTranslation(-10.5f, -1.5f, 0);
    }
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, 10, 10);
        setTextureUnlitMaterial(node->getModel(), "res/png/box-diffuse.png");
        node->setId("mipmap on");
        node->setTranslation(0.5f, -1.5f, 0);
    }
}

void TextureTest::finalize()
{
    // Model and font are reference counted and should be released before closing this test.
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    for (std::list<Font::Text*>::iterator it = _text.begin(); it != _text.end(); ++it)
    {
        SAFE_DELETE(*it);
    }
    _text.clear();
}

void TextureTest::update(float elapsedTime)
{
    Node* n1 = _scene->findNode("mipmap on");
    Node* n2 = _scene->findNode("mipmap off");
    // move these nodes between 0 and -600 using a sine wave
    float z = -(sin((getAbsoluteTime() / 1000.0L) * MATH_PI) + 1)  * 600.0f / 2.0f;
    n1->setTranslationZ(z);
    n2->setTranslationZ(z);
}

void TextureTest::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &TextureTest::drawScene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _font->start();
    for (std::list<Font::Text*>::const_iterator it = _text.begin(); it != _text.end(); ++it)
    {
        _font->drawText(*it);
    }
    _font->finish();
}

void TextureTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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

bool TextureTest::drawScene(Node* node)
{
    Model* model = node->getModel();
    if (model)
        model->draw();
    return true;
}
