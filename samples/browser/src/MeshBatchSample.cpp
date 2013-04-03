#include "MeshBatchSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Mesh Batch", MeshBatchSample, 3);
#endif

static Material* createMaterial()
{
    Material* material = Material::create("res/shaders/colored-unlit.vert", "res/shaders/colored-unlit.frag", "VERTEX_COLOR");
    GP_ASSERT(material && material->getStateBlock());
    return material;
}

static MeshBatch* createMeshBatch(Mesh::PrimitiveType primitiveType)
{
    Material* material = createMaterial();
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    unsigned int elementCount = sizeof(elements) / sizeof(VertexFormat::Element);
    MeshBatch* meshBatch = MeshBatch::create(VertexFormat(elements, elementCount), primitiveType, material, false);
    SAFE_RELEASE(material);
    return meshBatch;
}

static Vector3 randomColor()
{
    return Vector3(MATH_RANDOM_0_1(), MATH_RANDOM_0_1(), MATH_RANDOM_0_1());
}

MeshBatchSample::MeshBatchSample()
    : _font(NULL), _meshBatch(NULL), _lastTriangleAdded(0)
{
    _vertices.push_back(Vertex(Vector3(0, 50, 0), randomColor()));
    _vertices.push_back(Vertex(Vector3(-50, -50, 0), randomColor()));
    _vertices.push_back(Vertex(Vector3(50, -50, 0), randomColor()));
}

void MeshBatchSample::initialize()
{
    setMultiTouch(true);
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    Matrix::createOrthographic(getWidth(), getHeight(), -1.0f, 1.0f, &_worldViewProjectionMatrix);
    _meshBatch = createMeshBatch(Mesh::TRIANGLES);
}

void MeshBatchSample::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_DELETE(_meshBatch);
}

void MeshBatchSample::update(float elapsedTime)
{
}

void MeshBatchSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Draw all of the triangles as one mesh batch.
    _meshBatch->start();
    _meshBatch->add(&_vertices[0], (unsigned int)_vertices.size());
    _meshBatch->finish();
    _meshBatch->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    _meshBatch->draw();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    _font->start();
    char text[1024];
    sprintf(text, "Touch to add triangles (%d)", (int)(_vertices.size() / 3));
    _font->drawText(text, 10, getHeight() - _font->getSize() - 10, Vector4::one(), _font->getSize());
    _font->finish();
}

void MeshBatchSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
            addTriangle( x - (getWidth() >> 1), (getHeight() >> 1) - y);
        }
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        if (Game::getInstance()->getAbsoluteTime() - _lastTriangleAdded > 20)
        {
            addTriangle( x - (getWidth() >>1), (getHeight() >> 1) - y);
        }
        break;
    };
}

void MeshBatchSample::addTriangle(int x, int y)
{
    // Calculate the vertices of the equilateral triangle.
    // length of the side (between 40 and 120)
    float a = MATH_RANDOM_0_1() * 80.0 + 40.0f;  
    Vector3 p1(0.0f,       a / sqrtf(3.0f), 0);
    Vector3 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)), 0);
    Vector3 p3( a / 2.0f, -a / (2.0f * sqrtf(3.0f)), 0);

    // Transform each point to x,y and rotate it randomly.
    Matrix m;
    m.translate(x, y, 0);
    m.rotateZ(MATH_RANDOM_MINUS1_1() * MATH_PI);
    m.transformPoint(p1, &p1);
    m.transformPoint(p2, &p2);
    m.transformPoint(p3, &p3);
    
    // Added the triangle to the list with random vertex colors.
    _vertices.push_back(Vertex(p1, randomColor()));
    _vertices.push_back(Vertex(p2, randomColor()));
    _vertices.push_back(Vertex(p3, randomColor()));
    
    _lastTriangleAdded = Game::getInstance()->getAbsoluteTime();
}
