#include "SpriteBatchTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Graphics", "Sprite Batch", SpriteBatchTest, 8);
#endif

/**
 * Creates a triangle mesh with vertex colors.
 */
static Mesh* createTriangleMesh()
{
    // Calculate the vertices of the equilateral triangle.
    float a = 0.5f; // length of the side
    Vector2 p1(0.0f,       a / sqrtf(3.0f));
    Vector2 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)));
    Vector2 p3( a / 2.0f, -a / (2.0f * sqrtf(3.0f)));

    // Create 3 vertices.
    // Each vertex has position (x, y, z) and color (red, green, blue)
    float vertices[] =
    {
        p1.x, p1.y, 0.0f,     1.0f, 0.0f, 0.0f, // red
        p2.x, p2.y, 0.0f,     0.0f, 1.0f, 0.0f, // green
        p3.x, p3.y, 0.0f,     0.0f, 0.0f, 1.0f, // blue
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

SpriteBatchTest::SpriteBatchTest()
    : _font(NULL), _spriteBatch(NULL)
{
    
}

void SpriteBatchTest::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    // Create an orthographic projection matrix.
    float width = getWidth() / (float)getHeight();
    float height = 1.0f;
    Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_worldViewProjectionMatrix);

    _spriteBatch = SpriteBatch::create("res/common/box-diffuse.png");

}

void SpriteBatchTest::finalize()
{
    SAFE_DELETE(_spriteBatch);
    SAFE_RELEASE(_font);
}

void SpriteBatchTest::update(float elapsedTime)
{
    
}

void SpriteBatchTest::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    Rectangle dst(0, 0, 40, 40);
    Rectangle src(0, 0, 256, 256);

    _spriteBatch->start();
    // Just a sprite
    _spriteBatch->draw(dst, src);
    dst.x += dst.width;
    // Color tint
    _spriteBatch->draw(dst, src, Vector4(1, 1, 0, 1));
    dst.x += dst.width;
    // Larger destination
    _spriteBatch->draw(Rectangle(dst.x, dst.y, dst.width * 2.0f, dst.height * 2.0f), src);
    dst.x += dst.width * 2.0f;
    // Smaller destination
    _spriteBatch->draw(Rectangle(dst.x, dst.y, dst.width / 2.0f, dst.height / 2.0f), src);
    dst.x += dst.width / 2.0f;
    // Just the height is smaller
    _spriteBatch->draw(Rectangle(dst.x, dst.y, dst.width, dst.height * 2.0f), src);
    dst.x += dst.width;
    // Negative height
    _spriteBatch->draw(Rectangle(dst.x, dst.y + dst.height * 2.0, dst.width, dst.height * -2.0f), src);
    dst.x += dst.width;
    // Scale
    _spriteBatch->draw(Vector3(dst.x, dst.y, 0), src, Vector2(dst.width * 2.0f, dst.height * 2.0f));
    dst.x += dst.width * 2.0f;

    // rotate
    dst.x += dst.height / 4.0f;
    _spriteBatch->draw(Vector3(dst.x, dst.y, 0), src, Vector2(dst.width, dst.height), Vector4(0, 1, 0, 1), Vector2(0.5f, 0.5f), MATH_PIOVER4);

    _spriteBatch->finish();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SpriteBatchTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
