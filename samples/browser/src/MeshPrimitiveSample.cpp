#include "MeshPrimitiveSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Mesh Primitives", MeshPrimitiveSample, 2);
#endif

/**
 * Creates a triangle mesh with vertex colors.
 */
static Mesh* createTriangleMesh()
{
    // Calculate the vertices of the equilateral triangle.
    float a = 0.25f;        // Length of side
    Vector2 p1(0.0f,       a / sqrtf(3.0f));
    Vector2 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)));
    Vector2 p3( a / 2.0f, -a / (2.0f * sqrtf(3.0f)));

    // Create 3 vertices. Each vertex has position (x, y, z) and color (red, green, blue)
    float vertices[] =
    {
        p1.x, p1.y, 0.0f,     1.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f,     0.0f, 1.0f, 0.0f,
        p3.x, p3.y, 0.0f,     0.0f, 0.0f, 1.0f,
    };
    unsigned int vertexCount = 3;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    mesh->setVertexData(vertices, 0, vertexCount);
    return mesh;
}

static Mesh* createTriangleStripMesh()
{
    float scale = 0.02f;
    unsigned int vertexCount = 20;

    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);

    float x = -0.2f;
    float y = -0.05f;

    float step = fabs(x) * 2.0f / (float)vertexCount;

    for (unsigned int i = 0; i < vertexCount; ++i)
    {
        // x, y, z, r, g, b
        vertices.push_back(x);
        vertices.push_back(y + MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale * 2); 
        vertices.push_back(MATH_RANDOM_0_1()); 
        vertices.push_back(MATH_RANDOM_0_1()); 
        vertices.push_back(MATH_RANDOM_0_1()); 
        x += step;
        y *= -1.0f;
    }
    
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLE_STRIP);
    // 
    mesh->setVertexData(&vertices[0], 0, vertexCount);
    return mesh;
}

static Mesh* createLineStripMesh()
{
    float a = 0.1f;
    float vertices[] = 
    {
        0,  0,  0,    1, 0, 0,
        a,  0, -a,    0, 1, 0,
        0, -a,  a,    0, 0, 1,
       -a,  0, -a,    1, 0, 1,
        0,  a,  a,    0, 1, 1,
    };

    unsigned int vertexCount = 5;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINE_STRIP);
    mesh->setVertexData(vertices, 0, vertexCount);
    return mesh;
}

static Mesh* createLinesMesh()
{
    float scale = 0.2f; 
    unsigned int vertexCount = 40;

    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);
    for (unsigned int i = 0; i < vertexCount; ++i)
    {
        // x, y, z, r, g, b
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1());
    }
    
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    // 
    mesh->setVertexData(&vertices[0], 0, vertexCount);
    return mesh;
}


static Mesh* createPointsMesh()
{
    float scale = 0.2f;
    unsigned int vertexCount = 100;

    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);
    for (unsigned int i = 0; i < vertexCount; ++i)
    {
        // x, y, z, r, g, b
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1()); 
    }
    
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::POINTS);
    mesh->setVertexData(&vertices[0], 0, vertexCount);
    return mesh;
}

MeshPrimitiveSample::MeshPrimitiveSample()
    : _font(NULL), _triangles(NULL), _triangleStrip(NULL), _lineStrip(NULL), _lines(NULL), _points(NULL)
{
}

void MeshPrimitiveSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    // Create an orthographic projection matrix.
    float width = getWidth() / (float)getHeight();
    float height = 1.0f;
    Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_viewProjectionMatrix);

    // Create a model for the triangle mesh. A model is an instance of a Mesh that can be drawn with a specified material.
    Mesh* triangleMesh = createTriangleMesh();
    _triangles = Model::create(triangleMesh);
    SAFE_RELEASE(triangleMesh);

    // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
    // This sample doesn't use lighting so the unlit shader is used.
    // This sample uses vertex color so VERTEX_COLOR is defined. Look at the shader source files to see the supported defines.
    _triangles->setMaterial("res/shaders/colored-unlit.vert", "res/shaders/colored-unlit.frag", "VERTEX_COLOR");
    
    Mesh* triangleStripMesh = createTriangleStripMesh();
    _triangleStrip = Model::create(triangleStripMesh);
    SAFE_RELEASE(triangleStripMesh);
    Material* material = _triangleStrip->setMaterial("res/shaders/colored-unlit.vert", "res/shaders/colored-unlit.frag", "VERTEX_COLOR");
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    Mesh* lineStripMesh = createLineStripMesh();
    _lineStrip = Model::create(lineStripMesh);
    SAFE_RELEASE(lineStripMesh);
    _lineStrip->setMaterial("res/shaders/colored-unlit.vert", "res/shaders/colored-unlit.frag", "VERTEX_COLOR");
    
    Mesh* lineMesh = createLinesMesh();
    _lines = Model::create(lineMesh);
    SAFE_RELEASE(lineMesh);
    _lines->setMaterial("res/shaders/colored-unlit.vert", "res/shaders/colored-unlit.frag", "VERTEX_COLOR");
}

void MeshPrimitiveSample::finalize()
{
    // Model and font are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_triangles);
    SAFE_RELEASE(_triangleStrip);
    SAFE_RELEASE(_lineStrip);
    SAFE_RELEASE(_lines);
    SAFE_RELEASE(_points);
    SAFE_RELEASE(_font);
    for (std::list<Font::Text*>::iterator it = _text.begin(); it != _text.end(); ++it)
    {
        SAFE_DELETE(*it);
    }
    _text.clear();
}

void MeshPrimitiveSample::update(float elapsedTime)
{
    if (_touchPoint.x == -1.0f && _touchPoint.y == -1.0f)
    {
        _tilt.x *= powf(0.99f, elapsedTime);
        _tilt.y *= powf(0.99f, elapsedTime);
    }
}

void MeshPrimitiveSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    Matrix wvp;
    wvp.rotateY(_tilt.x * 0.01f);
    wvp.rotateX(_tilt.y * 0.01f);
    Matrix::multiply(wvp, _viewProjectionMatrix, &wvp);

    Matrix m;
    float offset = 0.5f;
    
    // Bind the view projection matrix to the model's paramter. This will transform the vertices when the model is drawn.
    m.setIdentity();
    m.translate(-offset, offset, 0);
    Matrix::multiply(m, wvp, &m);
    _triangles->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(m);
    _triangles->draw();

    m.setIdentity();
    m.translate(0, offset, 0);
    Matrix::multiply(m, wvp, &m);
    _triangleStrip->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(m);
    _triangleStrip->draw();

    m.setIdentity();
    m.translate(-offset, -offset, 0);
    Matrix::multiply(m, wvp, &m);
    _lineStrip->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(m);
    _lineStrip->draw();

    m.setIdentity();
    m.translate(0, -offset, 0);
    Matrix::multiply(m, wvp, &m);
    _lines->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(m);
    _lines->draw();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void MeshPrimitiveSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        else
        {
            _touchPoint.set(x, y);
        }
        break;
    case Touch::TOUCH_RELEASE:
        _touchPoint.set(-1.0f, -1.0f);
        break;
    case Touch::TOUCH_MOVE:
        if (_touchPoint.x > 0.0f && _touchPoint.y > 0.0f)
        {
            float deltaX = x - _touchPoint.x;
            float deltaY = y - _touchPoint.y;
            _tilt.x -= deltaX;
            _tilt.y += deltaY;
            _touchPoint.set(x, y);
        }
        break;
    };
}
