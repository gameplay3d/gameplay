#ifndef TRIANGLETEST_H_
#define TRIANGLETEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Test creating and draw a single triangle.
 */
class TriangleTest : public Test
{
public:

    TriangleTest();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    Font* _font;
    Model* _model;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
};

#endif
