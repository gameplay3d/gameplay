#ifndef TriangleBatchTest_H_
#define TriangleBatchTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class TriangleBatchTest : public Test
{
public:
    TriangleBatchTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    void addTriangle(int x, int y);

private:
    struct Vertex
    {
        Vector3 position;
        Vector3 color;

        Vertex() {}
        Vertex(const Vector3& position, const Vector3& color)
            : position(position), color(color) {}
    };

    Font* _font;
    MeshBatch* _triangleBatch;
    Matrix _worldViewProjectionMatrix;
    std::vector<Vertex> _vertices;
};

#endif
