#ifndef MeshPrimitiveTest_H_
#define MeshPrimitiveTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class MeshPrimitiveTest : public Test
{
public:
    MeshPrimitiveTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    static Material* createMaterial();
    static MeshBatch* createMeshBatch(Mesh::PrimitiveType primitiveType);

private:
    Font* _font;
    Model* _triangles;
    Model* _triangleStrip;
    Model* _lineStrip;
    Model* _lines;
    Model* _points;

    Matrix _viewProjectionMatrix;
    Vector2 _touchPoint;
    Vector2 _tilt;

    std::list<Font::Text*> _text;
};

#endif
