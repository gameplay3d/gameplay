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

Mesh* createTorus(float torusRadius = 2.0f, float ringRadius = 1.0f, unsigned int rings = 30, unsigned int precision = 20)
{
    GP_ASSERT(torusRadius > 0.0f && ringRadius > 0.0f);
    GP_ASSERT(rings >= 3 && precision >= 3);
    GP_ASSERT((precision * 2 + 2) <= UINT16_MAX); // In that rare moment where someone decides to have a precision of 32768 or more

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3)
    };

    unsigned int vertexCount = rings * precision;
    unsigned int partIndexCount = precision * 2 + 2;

    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    mesh->setPrimitiveType(Mesh::TRIANGLE_STRIP);

    // Set bounds
    float radius = torusRadius + ringRadius * 2.0f;
    mesh->setBoundingBox(BoundingBox(-radius, -ringRadius, -radius, radius, ringRadius, radius));
    mesh->setBoundingSphere(BoundingSphere(Vector3::zero(), radius));

    // Generate verticies and normals
    Vector3* verts = new Vector3[vertexCount * 2];
    float degreesPerPrecision = 360.0f / precision;
    float degreesPerRing = 360.0f / rings;
    // Generate first ring
    for (unsigned vert = 0; vert < precision; vert++)
    {
        float sinAngle = std::sinf(MATH_DEG_TO_RAD(vert * degreesPerPrecision));
        float cosAngle = std::cosf(MATH_DEG_TO_RAD(vert * degreesPerPrecision));

        // Position
        verts[vert * 2 + 0] = Vector3(ringRadius * cosAngle, ringRadius * sinAngle, 0.0f) + Vector3(torusRadius, 0.0f, 0.0f);

        // Normal
        Vector3::cross(Vector3(sinAngle, -cosAngle, 0.0f), Vector3(0.0f, 0.0f, -1.0f), &verts[vert * 2 + 1]);

        // UV
        //uv = Vector2(0, (float)vert / (precision - 1));
    }
    // Generate the rest of the rings
#define ROTATE_Y(original) Vector3(original.x * cosAngle + original.z * sinAngle, original.y, -original.x * sinAngle + original.z * cosAngle)
    for (unsigned int ring = 1; ring < rings; ring++)
    {
        //float uvS = 2.0f * ring / (precision - 1);

        for (unsigned vert = 0; vert < precision; vert++)
        {
            float sinAngle = std::sinf(MATH_DEG_TO_RAD(ring * degreesPerRing));
            float cosAngle = std::cosf(MATH_DEG_TO_RAD(ring * degreesPerRing));

            unsigned int baseIndex = (ring * precision * 2) + (vert * 2);

            // Position
            const Vector3& originPosition = verts[vert * 2 + 0];
            verts[baseIndex + 0] = ROTATE_Y(originPosition);

            // Normal
            const Vector3& originNormal = verts[vert * 2 + 1];
            verts[baseIndex + 1] = ROTATE_Y(originNormal);

            // UV
            //const Vector2& originUv = ?
            //uv = Vector2(uvS, originUv.y);
        }
    }
#undef ROTATE_Y
    mesh->setVertexData((const float*)verts);
    SAFE_DELETE_ARRAY(verts);

    // Generate indicies (one per ring)
    uint16_t* indicie = new uint16_t[partIndexCount];
    for (unsigned int ring = 0; ring < rings; ring++)
    {
        unsigned int baseIndex1 = ring * precision;
        unsigned int baseIndex2 = ((ring + 1) * precision) % vertexCount;

        // First and last triangle portion
        indicie[0] = indicie[partIndexCount - 2] = baseIndex1;
        indicie[1] = indicie[partIndexCount - 1] = baseIndex2;

        // Remaining portion of strip
        for (unsigned vert = 1, i = 2; vert < precision; vert++, i += 2)
        {
            indicie[i + 0] = baseIndex1 + vert;
            indicie[i + 1] = baseIndex2 + vert;
        }

        // Add part to mesh
        mesh->addPart(Mesh::TRIANGLE_STRIP, Mesh::INDEX16, partIndexCount, false)->setIndexData(indicie, 0, 0); // 0 offset, 0 count means it is a zero offset of partIndexCount indicies
    }
    SAFE_DELETE_ARRAY(indicie);

    return mesh;
}

Material* setTextureUnlitMaterial(Model* model, const char* texturePath, bool mipmap = true)
{
    Material* material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag");
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

Material* TextureSample::setCubemapUnlitMaterial(Model* model, Node* camera)
{
    // Colored shader params
    Material* material = model->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "CUBEMAP_REFLECTION");
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
    material->getParameter("u_diffuseColor")->setVector4(Vector4::one());

    // Color cubemap params
    material->setParameterAutoBinding("u_inverseTransposeWorldMatrix", "INVERSE_TRANSPOSE_WORLD_MATRIX");
    material->setParameterAutoBinding("u_worldMatrix", "WORLD_MATRIX");
    material->getParameter("u_cameraWorldPosition")->bindValue(camera, &Node::getTranslationWorld);

    Texture* tex = Texture::create(Texture::RGB, 128, 128, NULL, false, Texture::TEXTURE_CUBE);
    Texture::Sampler* texSampler = Texture::Sampler::create(tex);

    material->getParameter("u_cubemapTexture")->setValue(texSampler);

    _fBuffer = FrameBuffer::create("cubemap_framebuffer", tex->getWidth(), tex->getHeight());
    _rTarget = RenderTarget::create("cubemap_rendertarget", tex);

    SAFE_RELEASE(texSampler);
    SAFE_RELEASE(tex);

    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);
    return material;
}

TextureSample::TextureSample()
    : _font(NULL), _scene(NULL), _zOffset(0.0f), _fBuffer(NULL), _rTarget(NULL)
{   
}

void TextureSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

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
        setTextureUnlitMaterial(node->getModel(), "res/png/logo.png", false);
        node->setId("mipmap off");
        node->setTranslation(-25.5f, -2.5f, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("MipMap: Off", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
    // Mipmapping On
    {
        Node* node = addQuadModelAndNode(_scene, 0, 0, cubeSize, cubeSize);
        setTextureUnlitMaterial(node->getModel(), "res/png/logo.png");
        node->setId("mipmap on");
        node->setTranslation(-5.5f, -2.5f, 0);
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld(), &x, &y);
        _text.push_back(_font->createText("MipMap: On", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
    // Texture cube
    {
        Camera* cam = Camera::createPerspective(90, 1, 1, 1000);
        Node* camNode = _scene->addNode("cubemap_camera");
        camNode->setCamera(cam);
        camNode->setTranslation(13.5f, 1.5f, 15);
        SAFE_RELEASE(cam);
        Mesh* mesh = createTorus();
        Node* node = addQuadModelAndNode(_scene, mesh);
        SAFE_RELEASE(mesh);
        setCubemapUnlitMaterial(node->getModel(), cameraNode);
        node->setId("texture cube");
        node->setTranslation(13.5f, 1.5f, 15);
        node->rotateX(MATH_DEG_TO_RAD(45));
        const BoundingSphere& bounds = node->getModel()->getMesh()->getBoundingSphere();
        _scene->getActiveCamera()->project(getViewport(), node->getTranslationWorld() - Vector3(bounds.radius, bounds.radius, 0), &x, &y);
        _text.push_back(_font->createText("Texture Cube", Rectangle(x, y, textWidth, fontSize), Vector4::one(), fontSize, Font::ALIGN_HCENTER, false));
    }
}

void TextureSample::finalize()
{
    // Model, font, and frame buffers are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_fBuffer);
    SAFE_RELEASE(_rTarget);
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
    // rotate torus
    Node* n3 = _scene->findNode("texture cube");
    n3->rotateX((elapsedTime / 1500.0f) * MATH_PI);
}

void TextureSample::render(float elapsedTime)
{
    // Draw cubemap

    Node* torus = _scene->findNode("texture cube");
    Node* torusCamera = _scene->findNode("cubemap_camera");
    Camera* previousCamera = _scene->getActiveCamera();
    _scene->setActiveCamera(torusCamera->getCamera());

    Rectangle defaultViewport = getViewport();
    setViewport(Rectangle(_fBuffer->getWidth(), _fBuffer->getHeight()));
    FrameBuffer* previousFrameBuffer = _fBuffer->bind();

    for (unsigned int i = 0; i < 6; i++)
    {
        Texture::CubeFace face = static_cast<Texture::CubeFace>(i);

        // Set render target (which face of the cube to draw to)
        _fBuffer->setRenderTarget(_rTarget, face);

        // Setup camera to face in proper direction
        torusCamera->setRotation(Quaternion::identity());
        switch (face)
        {
            case Texture::POS_X:
                torusCamera->rotateY(MATH_DEG_TO_RAD(90));
                torusCamera->rotateX(MATH_DEG_TO_RAD(180));
                break;
            case Texture::NEG_X:
                torusCamera->rotateY(MATH_DEG_TO_RAD(-90));
                torusCamera->rotateX(MATH_DEG_TO_RAD(180));
                break;
            case Texture::POS_Y:
                torusCamera->rotateX(MATH_DEG_TO_RAD(90));
                break;
            case Texture::NEG_Y:
                torusCamera->rotateX(MATH_DEG_TO_RAD(-90));
                break;
            case Texture::POS_Z:
                torusCamera->rotateX(MATH_DEG_TO_RAD(180));
                break;
            case Texture::NEG_Z:
                torusCamera->rotateY(MATH_DEG_TO_RAD(180));
                torusCamera->rotateX(MATH_DEG_TO_RAD(180));
                break;
        }

        // Draw
        clear(CLEAR_COLOR_DEPTH, Vector4(0.2f, 0.2f, 0.2f, 1), 1.0f, 0);
        _scene->visit(this, &TextureSample::drawScene, torus);
    }

    // Draw entire scene

    setViewport(defaultViewport);
    _scene->setActiveCamera(previousCamera);
    previousFrameBuffer->bind();

    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &TextureSample::drawScene, (Node*)NULL);

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

bool TextureSample::drawScene(Node* node, Node* ignore)
{
    if (node == ignore)
        return true;
    Model* model = node->getModel();
    if (model)
        model->draw();
    return true;
}
