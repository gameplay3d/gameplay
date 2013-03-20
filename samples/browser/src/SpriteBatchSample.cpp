#include "SpriteBatchSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Sprite Batch", SpriteBatchSample, 8);
#endif

/**
 * Creates a triangle mesh with vertex colors.
 */
static Mesh* createTriangleMesh()
{
    // Calculate the vertices of the equilateral triangle.
    float a = 0.5f;        // length of the side
    Vector2 p1(0.0f,       a / sqrtf(3.0f));
    Vector2 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)));
    Vector2 p3( a / 2.0f, -a / (2.0f * sqrtf(3.0f)));

    // Create 3 vertices.
    // Each vertex has position (x, y, z) and color (red, green, blue)
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

SpriteBatchSample::SpriteBatchSample()
    : _font(NULL), _spriteBatch(NULL)
{
}

void SpriteBatchSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

    // Create an orthographic projection matrix.
    float width = getWidth() / (float)getHeight();
    float height = 1.0f;
    Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_worldViewProjectionMatrix);
    _spriteBatch = SpriteBatch::create("res/png/box-diffuse.png");
}

void SpriteBatchSample::finalize()
{
    SAFE_DELETE(_spriteBatch);
    SAFE_RELEASE(_font);
}

void SpriteBatchSample::update(float elapsedTime)
{
}

void SpriteBatchSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    Rectangle dst(0, 0, 64, 64);
    Rectangle src(0, 0, 256, 256);

    _spriteBatch->start();

    // Just a sprite dst from src no color tint
    _spriteBatch->draw(dst, src);

    // Color tint
    _spriteBatch->draw(Rectangle( 64, 0, 64, 64), src, Vector4::fromColor(0xF68B28FF));
    _spriteBatch->draw(Rectangle(128, 0, 64, 64), src, Vector4::fromColor(0xDA2128FF));
    _spriteBatch->draw(Rectangle(192, 0, 64, 64), src, Vector4::fromColor(0xE21B52FF));
    _spriteBatch->draw(Rectangle(256, 0, 64, 64), src, Vector4::fromColor(0xE12991FF));
    _spriteBatch->draw(Rectangle(320, 0, 64, 64), src, Vector4::fromColor(0x9A258FFF));
    _spriteBatch->draw(Rectangle(384, 0, 64, 64), src, Vector4::fromColor(0x4D3F99FF));
    _spriteBatch->draw(Rectangle(448, 0, 64, 64), src, Vector4::fromColor(0x0073BCFF));
    _spriteBatch->draw(Rectangle(512, 0, 64, 64), src, Vector4::fromColor(0x00A8DFFF));
    _spriteBatch->draw(Rectangle(576, 0, 64, 64), src, Vector4::fromColor(0x00AFADFF));
    _spriteBatch->draw(Rectangle(640, 0, 64, 64), src, Vector4::fromColor(0x00A95CFF));
    _spriteBatch->draw(Rectangle(704, 0, 64, 64), src, Vector4::fromColor(0x8CC747FF));
    _spriteBatch->draw(Rectangle(768, 0, 64, 64), src, Vector4::fromColor(0xFFE710FF));

    // Negative height draw over top of the first one
    _spriteBatch->draw(Rectangle(0, 0 , 64 * 2.0f, 64 * -2.0f), src);

    // Scale
    _spriteBatch->draw(Vector3(0, 64, 0), src, Vector2(dst.width * 2.0f, dst.height * 2.0f));
    // rotate 90
    _spriteBatch->draw(Vector3(128, 64, 0), src, Vector2(128, 128), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(90));
    _spriteBatch->draw(Vector3(256, 64, 0), src, Vector2(128, 128), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(180));
    _spriteBatch->draw(Vector3(384, 64, 0), src, Vector2(128, 128), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(270));
    _spriteBatch->draw(Vector3(512, 64, 0), src, Vector2(128, 128), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(360));
    _spriteBatch->draw(Vector3(640, 64, 0), src, Vector2(128, 128), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(0));
    
    // Lots of them now small
    unsigned int pointCount = 16;
    unsigned int x = 0;
    unsigned int y = 192;
    for (unsigned int i = 0; i < pointCount; i++)
    {
        for (unsigned int j = 0; j < pointCount; j++)
        {
            _spriteBatch->draw(Rectangle(x, y, 32, 32), src); 
            x += 32;    
        }
        x = 0;
        y += 32;
    }
    _spriteBatch->finish();

    // Draw a second batch to ensure no problems
    _spriteBatch->start();

    // 50% transparent
    _spriteBatch->draw(Rectangle(x + 512, y - 512, 512, 512), src, Vector4(1, 1, 1, 0.5f)); 
    _spriteBatch->finish();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SpriteBatchSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
