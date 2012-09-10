#ifndef MeshBatchTest_H_
#define MeshBatchTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class MeshBatchTest : public Test
{
public:
    MeshBatchTest();
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
    MeshBatch* _meshBatch;
    Matrix _worldViewProjectionMatrix;
    std::vector<Vertex> _vertices;
};

#endif
