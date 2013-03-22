#include "PostProcessSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Post Process", PostProcessSample, 13);
#endif

#define FRAMEBUFFER_WIDTH 1024
#define FRAMEBUFFER_HEIGHT 1024

Model* PostProcessSample::_quadModel = NULL;
Material* PostProcessSample::_compositorMaterial = NULL;


PostProcessSample::Compositor* PostProcessSample::Compositor::create(FrameBuffer* srcBuffer, FrameBuffer* dstBuffer, const char* materialPath, const char* techniqueId)
{
    GP_ASSERT(srcBuffer);

    Material* material = Material::create(materialPath);
    Texture::Sampler* sampler = Texture::Sampler::create(srcBuffer->getRenderTarget()->getTexture());
    material->getParameter("u_texture")->setValue(sampler);
    SAFE_RELEASE(sampler);
    if (_quadModel == NULL)
    {
        Mesh* mesh = Mesh::createQuadFullscreen();
        _quadModel = Model::create(mesh);
        SAFE_RELEASE(mesh);
    }
    
    return new Compositor(srcBuffer, dstBuffer, material, techniqueId);
}

PostProcessSample::Compositor::Compositor(FrameBuffer* srcBuffer, FrameBuffer* dstBuffer, Material* material, const char* techniqueId)
    : _srcBuffer(srcBuffer), _dstBuffer(dstBuffer), _material(material),  _techniqueId(techniqueId)
{
}

PostProcessSample::Compositor::~Compositor()
{
    SAFE_RELEASE(_material);
}

FrameBuffer* PostProcessSample::Compositor::getSrcFrameBuffer() const 
{ 
    return _srcBuffer; 
}

FrameBuffer* PostProcessSample::Compositor::getDstFrameBuffer() const
{ 
    return _dstBuffer; 
}

const char* PostProcessSample::Compositor::getTechniqueId() const
{
    return _techniqueId;
}

Material* PostProcessSample::Compositor::getMaterial() const
{
    return _material;
}

void PostProcessSample::Compositor::blit(const Rectangle& dst)
{
    if (_compositorMaterial != _material)
    {
        _compositorMaterial = _material;
        _quadModel->setMaterial(_compositorMaterial);
    }
    _compositorMaterial->setTechnique(_techniqueId);

    _quadModel->draw();
}

PostProcessSample::PostProcessSample()
    : _font(NULL), _scene(NULL), _modelNode(NULL), _frameBuffer(NULL), _compositorIndex(0)
{
}

void PostProcessSample::initialize()
{
    _font = Font::create("res/common/arial18.gpb");

    // Load game scene from file
    _scene = Scene::load("res/common/duck.gpb");

    // Get light node
    Node* lightNode = _scene->findNode("directionalLight1");
    Light* light = lightNode->getLight();

    // Initialize box model
    _modelNode = _scene->findNode("duck");
    Model* model = _modelNode->getModel();
    Material* material = model->setMaterial("res/common/duck.material");
    material->getParameter("u_lightColor")->setValue(light->getColor());
    material->getParameter("u_lightDirection")->setValue(lightNode->getForwardVectorView());

    // Create one frame buffer for the full screen compositerss.
    _frameBuffer = FrameBuffer::create("PostProcessSample", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    DepthStencilTarget* dst = DepthStencilTarget::create("PostProcessSample", DepthStencilTarget::DEPTH_STENCIL, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    _frameBuffer->setDepthStencilTarget(dst);
    SAFE_RELEASE(dst);

    // Create our compositors that all output to the default framebuffer.
    Compositor* compositor = NULL;

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Passthrough");
    _compositors.push_back(compositor);

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Grayscale");
    _compositors.push_back(compositor);

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Sepia");
    _compositors.push_back(compositor);

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Pixelate");
    _compositors.push_back(compositor);

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Sobel Edge");
    _compositors.push_back(compositor);
    compositor->getMaterial()->getParameter("u_width")->setValue((float)FRAMEBUFFER_WIDTH / 2.0f);
    compositor->getMaterial()->getParameter("u_height")->setValue((float)FRAMEBUFFER_HEIGHT / 2.0f);

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Gaussian Blur");
    _compositors.push_back(compositor);
    compositor->getMaterial()->getParameter("u_length")->setValue(1.0f / ((float)FRAMEBUFFER_WIDTH / 2.0f));

    compositor = Compositor::create(_frameBuffer, NULL, "res/common/postprocess/postprocess.material", "Old Film");
    _compositors.push_back(compositor);
    compositor->getMaterial()->getParameter("u_sepiaValue")->setValue(0.8f);
    compositor->getMaterial()->getParameter("u_noiseValue")->setValue(0.4f);
    compositor->getMaterial()->getParameter("u_scratchValue")->setValue(0.4f);
    compositor->getMaterial()->getParameter("u_innerVignetting")->setValue(0.9f);
    compositor->getMaterial()->getParameter("u_outerVignetting")->setValue(0.9f);
}

void PostProcessSample::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
    for (std::vector<Compositor*>::iterator it = _compositors.begin(); it != _compositors.end(); ++it)
    {
        delete *it;
    }
    _compositors.clear();
    SAFE_RELEASE(_quadModel);
    SAFE_RELEASE(_frameBuffer);
}

void PostProcessSample::update(float elapsedTime)
{
    _modelNode->rotateY(elapsedTime * MATH_DEG_TO_RAD(0.25f));

    // Only the last sample will check live updating of material parameters
    if (_compositorIndex == _compositors.size() - 1)
    {
        Compositor* compositor = _compositors[_compositorIndex];
        MaterialParameter* elapsedParam  = compositor->getMaterial()->getParameter("u_elapsedTime");
        if (elapsedParam)
            elapsedParam->setValue(elapsedTime);
        MaterialParameter* randomParam = compositor->getMaterial()->getParameter("u_random");
        if (randomParam)
            randomParam->setValue(MATH_RANDOM_0_1());
    }
}

void PostProcessSample::render(float elapsedTime)
{
    Rectangle defaultViewport = Game::getInstance()->getViewport();
    
    // Draw into the framebuffer
    Game::getInstance()->setViewport(Rectangle(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT));
    FrameBuffer* previousFrameBuffer = _frameBuffer->bind();
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    _scene->visit(this, &PostProcessSample::drawScene);

    // Bind the current compositor
    Game::getInstance()->setViewport(defaultViewport);
    Compositor* compositor = _compositors[_compositorIndex];

    FrameBuffer* compositorDstFrameBuffer = compositor->getDstFrameBuffer();
    FrameBuffer* prevToCompositeFrameBuffer = NULL;
    if (compositorDstFrameBuffer)
    {
        prevToCompositeFrameBuffer = compositorDstFrameBuffer->bind();
    }
    else
    {
        prevToCompositeFrameBuffer = previousFrameBuffer->bind();
    }

    Game::getInstance()->clear(CLEAR_COLOR, Vector4(0, 0, 0, 1), 1.0f, 0);
    compositor->blit(defaultViewport);
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    drawTechniqueId(compositor->getTechniqueId());

    previousFrameBuffer->bind();

    // Draw the pass through compositor at index 0 at quarter of the size and bottom right. dont clear the dest just draw last on top
    float quarterWidth = getWidth() / 4;
    float quarterHeight = getHeight() / 4;
    Rectangle offsetViewport = Rectangle(getWidth() - quarterWidth, 0, quarterWidth, quarterHeight);
    Game::getInstance()->setViewport(offsetViewport);
    compositor = _compositors[0];
    compositor->blit(offsetViewport);
    Game::getInstance()->setViewport(defaultViewport);

}

bool PostProcessSample::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw();
    }
    return true;
}

void PostProcessSample::drawTechniqueId(const char* techniqueId)
{
    char buffer[128];
    sprintf(buffer, "%s", techniqueId);
    _font->start();
    _font->drawText(buffer, Rectangle(0, 10, getWidth(), getHeight()), Vector4::one(), _font->getSize(), Font::ALIGN_TOP_HCENTER);
    _font->finish();
}

void PostProcessSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        {
            _compositorIndex++;
            if (_compositorIndex == _compositors.size())
                _compositorIndex = 0;
        }
        break;

    default:
        break;
    };
}

