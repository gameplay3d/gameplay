#ifndef MESHPRIMITIVESAMPLE_H_
#define MESHPRIMITIVESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample programmatically creating and drawing the supported mesh primitives.
 */
class MeshPrimitiveSample : public Sample
{
public:

    MeshPrimitiveSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    static Material* createMaterial();

    static MeshBatch* createMeshBatch(Mesh::PrimitiveType primitiveType);

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
